#pragma once

#include <chrono>
#include <vector>
#include <algorithm>
#include "physicsobject.hpp"

class PhysicsHandler {
    private:
    double mElasticity;
    double mElapsedSimTime = 0;

    protected:
    std::vector<PhysicsObject *> mObjects;
    void applyCollisions();
    virtual std::vector<glm::dvec3> calculateNetForces() = 0;
    virtual std::vector<glm::dvec3> calculateNetForces(std::vector<PhysicsObject> objects) = 0;


    public:
    PhysicsHandler();
    void setElasticity(double elasticity);
    double getElapsedSimTime();
    void addObject(PhysicsObject *object);
    void removeObject(PhysicsObject object);
    void removeObject(unsigned long long uid);
    void renderObjects(Renderer *renderer, Shader *shader);
    void eulerUpdate(double timeStep);
    void rk4Update(double timeStep);
    
};

PhysicsHandler::PhysicsHandler() {

}

void PhysicsHandler::setElasticity(double elasticity) {
    mElasticity = elasticity;
}

double PhysicsHandler::getElapsedSimTime() {
    return mElapsedSimTime;
}

void PhysicsHandler::addObject(PhysicsObject *object) {
    mObjects.push_back(object);
}

void PhysicsHandler::removeObject(PhysicsObject object) {
    removeObject(object.getUID());
}

void PhysicsHandler::removeObject(unsigned long long uid) {
    assert("TODO: IMPLEMENT REMOVAL OF OBJECTS" && false);
}

void PhysicsHandler::eulerUpdate(double timeStep) {
    mElapsedSimTime += timeStep;
    
    applyCollisions();

    auto forces = calculateNetForces();
    
    // Update Accelerations
    // std::cout << "Accelerations:" << std::endl;
    for (long int i = 0; i < mObjects.size(); i++) {
        glm::dvec3 acceleration = forces[i] / (mObjects[i]->getMass());
        // std::cout << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << std::endl;
        mObjects[i]->setAcceleration(acceleration);
    }

    // Update Velocities
    // std::cout << "Velocities:" << std::endl;
    for (long int i = 0; i < mObjects.size(); i++) {
        glm::dvec3 velocity = mObjects[i]->getAcceleration() * timeStep;
        // std::cout << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
        mObjects[i]->updateVelocity(velocity);
    }

    // Update Positions
    for (long int i = 0; i < mObjects.size(); i++) {
        glm::dvec3 position = mObjects[i]->getVelocity() * timeStep;
        mObjects[i]->updatePosition(position);
    }
}

void PhysicsHandler::applyCollisions() {
    for (int j = 0; j < mObjects.size(); j++) {
		for (int i = j + 1; i < mObjects.size(); i++) {
            // Static Collision Calculations
            glm::dvec3 positionDifference = mObjects[i]->getPosition() - mObjects[j]->getPosition();
            double distance = glm::length(positionDifference);
            double overlap = distance - (mObjects[i]->getRadius() + mObjects[j]->getRadius());

            if (overlap > 0) continue;

            std::cout << "COLLISION DETECTED" << std::endl;

            glm::dvec3 offset = overlap * 0.5 * positionDifference / distance;

            mObjects[i]->updatePosition(-offset);
            mObjects[j]->updatePosition(offset);

            // Dynamic Collision Calculations
            positionDifference = mObjects[i]->getPosition() - mObjects[j]->getPosition();

            glm::dvec3 normal = glm::normalize(-positionDifference);

            double dotProduct = glm::dot((mObjects[i]->getVelocity() - mObjects[j]->getVelocity()), normal);

            double impulse = (mObjects[i]->getMass() * mObjects[j]->getMass()) / (mObjects[i]->getMass() + mObjects[j]->getMass());
            impulse *= (1 + mElasticity) * dotProduct;

            mObjects[i]->updateVelocity((-impulse / mObjects[i]->getMass()) * normal);
            mObjects[j]->updateVelocity((impulse / mObjects[j]->getMass()) * normal);
        }
    }
}

void PhysicsHandler::rk4Update(double timeStep) {
    mElapsedSimTime += timeStep;

    applyCollisions();

    // Runge-Kutta 4
    // yn1 = yn + (h/6)(k1 + k2 + k3 + k4)
    //
    // k1 = f(tn, yn)
    // k2 = f(tn + h/2, yn + k1 * (h/2))
    // k3 = f(tn + h/2, yn + k2 * (h/2))
    // k4 = f(tn + h/2, yn + k3 * (h))

    // Step 1: 
    // a) Calculate k1v for all objects in the system.
    // This is the same as calculating the velocity in the euler method.

    // Get the forces acting on all of the objects in the system
    std::vector<glm::dvec3> forces = calculateNetForces();

    // Calculate the accelerations on the objects due to the forces. This is k1v for all objects.
    std::vector<glm::dvec3> k1v;
    for (long int i = 0; i < mObjects.size(); i++) {
        k1v.push_back(forces[i] / (mObjects[i]->getMass()));
    }

    // b) Calculate k1r for all objects in the system.
    // This is just the velocities of the objects in the system. 
    std::vector<glm::dvec3> k1r;
    for (long int i = 0; i < mObjects.size(); i++) {
        k1r.push_back(mObjects[i]->getVelocity());
    }

    // Step 2:
    // a) Calculate k2v for all objects in the system
    // This is a little more involed since we have to pretend the particles have moved in the system
    std::vector<glm::dvec3> k2v;
    std::vector<PhysicsObject> futureObjects;
    for (long int i = 0; i < mObjects.size(); i++) {
        futureObjects.push_back(*mObjects[i]);
        futureObjects[i].updatePosition(k1r[i] * (timeStep / 2));
    }

    // Calculate the forces and accelerations at these future positions
    forces = calculateNetForces(futureObjects);
    for (long int i = 0; i < mObjects.size(); i++) {
        k2v.push_back(forces[i] / (mObjects[i]->getMass()));
    }

    // b) Calculate k2r for all objects in the system
    std::vector<glm::dvec3> k2r;
    for (long int i = 0; i < mObjects.size(); i++) {
        k2r.push_back(mObjects[i]->getVelocity() + (k1v[i] * (timeStep / 2)));
    }

    // Step 3:
    // Calculate k3v and k3r, this is the same as step 2, but we substitute the values of k1v/r for k2v/r
    // a) Calculate k2v for all objects in the system
    std::vector<glm::dvec3> k3v;
    futureObjects.clear();
    for (long int i = 0; i < mObjects.size(); i++) {
        futureObjects.push_back(*mObjects[i]);
        futureObjects[i].updatePosition(k2r[i] * (timeStep / 2));
    }

    // Calculate the forces and accelerations at these future positions
    forces = calculateNetForces(futureObjects);
    for (long int i = 0; i < mObjects.size(); i++) {
        k3v.push_back(forces[i] / (mObjects[i]->getMass()));
    }

    // b) Calculate k2r for all objects in the system
    std::vector<glm::dvec3> k3r;
    for (long int i = 0; i < mObjects.size(); i++) {
        k3r.push_back(mObjects[i]->getVelocity() + (k2v[i] * (timeStep / 2)) * timeStep);
    }

    // Step 4:
    // Calculate k4v and k4r, similar to previous step just substitue k3v/r and dont divide timestep by 2
    // a) Calculate k2v for all objects in the system
    std::vector<glm::dvec3> k4v;
    futureObjects.clear();
    for (long int i = 0; i < mObjects.size(); i++) {
        futureObjects.push_back(*mObjects[i]);
        futureObjects[i].updatePosition(k3r[i] * timeStep);
    }

    // Calculate the forces and accelerations at these future positions
    forces = calculateNetForces(futureObjects);
    for (long int i = 0; i < mObjects.size(); i++) {
        k4v.push_back(forces[i] / (mObjects[i]->getMass()));
    }

    // b) Calculate k2r for all objects in the system
    std::vector<glm::dvec3> k4r;
    for (long int i = 0; i < mObjects.size(); i++) {
        k4r.push_back(mObjects[i]->getVelocity() + (k3v[i] * timeStep));
    }

    // Step 5:
    // calculate and update positions and velocities using the k's found in previous steps
    for (long int i = 0; i < mObjects.size(); i++) {
        mObjects[i]->updateVelocity((timeStep / 6) * (k1v[i] + (2.0 * k2v[i]) + (2.0 * k3v[i]) + k4v[i]));
        mObjects[i]->updatePosition((timeStep / 6) * (k1r[i] + (2.0 * k2r[i]) + (2.0 * k3r[i]) + k4r[i]));
    }


}


