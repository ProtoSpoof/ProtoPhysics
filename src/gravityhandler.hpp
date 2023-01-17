#pragma once

#include "physicshandler.hpp"

class GravityHandler: public PhysicsHandler {
    private:
    std::vector<glm::vec3> calculateNetForces();

    public:
    void simulate(double timeStep);

};


void GravityHandler::simulate(double timeStep) {
    timeStep /= 1000000000.0f;
    eulerUpdate(timeStep);
}

std::vector<glm::vec3> GravityHandler::calculateNetForces() {
    std::vector<glm::vec3> netForces;
    for (long int i = 0; i < mObjects.size(); i++) {
        // Calculate the forces acting on the particle
        glm::vec3 netForce(0, 0, 0);
        for (long int j = 0; j < mObjects.size(); j++) {
            if (j == i) continue;

            // Calculate the distance between the objects
            auto positionDifference = mObjects[i]->getPosition() - mObjects[j]->getPosition();
            double distance = glm::length(positionDifference); 
            double forceMagnitude = (6.6743e-11) * (mObjects[i]->getMass() * mObjects[j]->getMass() / (distance * distance));

            netForce += -glm::normalize(positionDifference) * (float)forceMagnitude;
        }
        netForces.push_back(netForce);
    }
    return netForces;
}