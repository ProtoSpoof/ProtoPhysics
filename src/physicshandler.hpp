#pragma once

#include <chrono>
#include <vector>
#include <algorithm>
#include <ProtoGL\indexbuffer.hpp>
#include <ProtoGL\vertexbuffer.hpp>
#include <ProtoGL\renderer.hpp>
#include "physicsobject.hpp"

class PhysicsHandler {
    private:
    IndexBuffer mObjectIndexBuffer = IndexBuffer(nullptr, 0);
    VertexBuffer mObjectVertexBuffer = VertexBuffer(nullptr, 0);
    VertexArray mObjectVertexArray;
    VertexBufferLayout mObjectVertexBufferLayout;
    double mElasticity;

    protected:
    std::vector<PhysicsObject *> mObjects;
    std::vector<glm::vec3> mObjectForces;
    void eulerUpdate(double timeStep);
    void applyCollisions();
    virtual std::vector<glm::vec3> calculateNetForces() = 0;


    public:
    PhysicsHandler();
    void setElasticity(double elasticity);
    void addObject(PhysicsObject *object);
    void removeObject(PhysicsObject object);
    void removeObject(unsigned long long uid);
    void renderObjects(Renderer *renderer, Shader *shader);
    virtual void simulate(double timeStep) = 0;
    
};

PhysicsHandler::PhysicsHandler() {
	mObjectVertexBufferLayout.push(GL_FLOAT,2);
	mObjectVertexBufferLayout.push(GL_FLOAT,4);
	mObjectVertexArray.addBuffer(mObjectVertexBuffer, mObjectVertexBufferLayout);
	mObjectVertexArray.unbind();
    mObjectIndexBuffer.unbind();
    mObjectVertexBuffer.unbind();
}

void PhysicsHandler::setElasticity(double elasticity) {
    mElasticity = elasticity;
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
    applyCollisions();

    auto forces = calculateNetForces();
    
    // Update Accelerations
    for (long int i = 0; i < mObjects.size(); i++) {
        glm::vec3 acceleration = forces[i] / (float)(mObjects[i]->getMass());
        mObjects[i]->setAcceleration(acceleration);
    }

    // Update Velocities
    for (long int i = 0; i < mObjects.size(); i++) {
        glm::vec3 velocity = mObjects[i]->getAcceleration() * (float)timeStep;
        mObjects[i]->updateVelocity(velocity);
    }

    // Update Positions
    for (long int i = 0; i < mObjects.size(); i++) {
        glm::vec3 position = mObjects[i]->getVelocity() * (float)timeStep;
        mObjects[i]->updatePosition(position);
    }
}

void PhysicsHandler::applyCollisions() {
    for (int j = 0; j < mObjects.size(); j++) {
		for (int i = j + 1; i < mObjects.size(); i++) {
            // Static Collision Calculations
            glm::vec3 positionDifference = mObjects[i]->getPosition() - mObjects[j]->getPosition();
            double distance = glm::length(positionDifference);
            double overlap = distance - (mObjects[i]->getRadius() + mObjects[j]->getRadius());

            if (overlap > 0) continue;

            glm::vec3 offset = (float)overlap * 0.5f * positionDifference / (float)distance;

            mObjects[i]->updatePosition(-offset);
            mObjects[j]->updatePosition(offset);

            // Dynamic Collision Calculations
            positionDifference = mObjects[i]->getPosition() - mObjects[j]->getPosition();

            glm::vec3 normal = glm::normalize(-positionDifference);

            double dotProduct = glm::dot((mObjects[i]->getVelocity() - mObjects[j]->getVelocity()), normal);

            double impulse = (mObjects[i]->getMass() * mObjects[j]->getMass()) / (mObjects[i]->getMass() + mObjects[j]->getMass());
            impulse *= (1 + mElasticity) * dotProduct;

            mObjects[i]->updateVelocity((float)(-impulse / mObjects[i]->getMass()) * normal);
            mObjects[j]->updateVelocity((float)(impulse / mObjects[j]->getMass()) * normal);
        }
    }
}


