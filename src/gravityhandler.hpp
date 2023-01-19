#pragma once

#include "physicshandler.hpp"

class GravityHandler: public PhysicsHandler {
    private:
    std::vector<glm::dvec3> calculateNetForces();
    std::vector<glm::dvec3> calculateNetForces(std::vector<PhysicsObject> objects);

};

std::vector<glm::dvec3> GravityHandler::calculateNetForces() {
    std::vector<glm::dvec3> netForces;
    for (long int i = 0; i < mObjects.size(); i++) {
        // Calculate the forces acting on the particle
        glm::dvec3 netForce(0, 0, 0);
        for (long int j = 0; j < mObjects.size(); j++) {
            if (j == i) continue;

            // Calculate the distance between the objects
            auto positionDifference = mObjects[i]->getPosition() - mObjects[j]->getPosition();
            double distance = glm::length(positionDifference); 
            double forceMagnitude = (6.6743e-11) * (mObjects[i]->getMass() * mObjects[j]->getMass() / (distance * distance));

            netForce += -glm::normalize(positionDifference) * forceMagnitude;
        }
        netForces.push_back(netForce);
    }
    return netForces;
}

std::vector<glm::dvec3> GravityHandler::calculateNetForces(std::vector<PhysicsObject> objects) {
    std::vector<glm::dvec3> netForces;
    for (long int i = 0; i < objects.size(); i++) {
        // Calculate the forces acting on the particle
        glm::dvec3 netForce(0, 0, 0);
        for (long int j = 0; j < objects.size(); j++) {
            if (j == i) continue;

            // Calculate the distance between the objects
            auto positionDifference = objects[i].getPosition() - mObjects[j]->getPosition();
            double distance = glm::length(positionDifference); 
            double forceMagnitude = (6.6743e-11) * (objects[i].getMass() * mObjects[j]->getMass() / (distance * distance));

            netForce += -glm::normalize(positionDifference) * forceMagnitude;
        }
        netForces.push_back(netForce);
    }
    return netForces;
}