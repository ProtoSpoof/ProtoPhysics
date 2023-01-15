#pragma once

#include "physicsobject.hpp"


class ClothParticle: PhysicsObject {
    private:

    
    public:
        ClothParticle(double mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration);

};

ClothParticle::ClothParticle(double mass, glm::vec3 position = glm::vec3(0,0,0), glm::vec3 velocity = glm::vec3(0,0,0), glm::vec3 acceleration = glm::vec3(0,0,0)) {
    mMass = mass;
    mPosition = position;
    mVelocity = velocity;
    mAcceleration = acceleration;
}