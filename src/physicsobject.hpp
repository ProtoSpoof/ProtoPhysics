#pragma once

#include <glm/glm.hpp>
#include "physicsenvironment.hpp"

class PhysicsObject {
    protected:
    unsigned long long int mUID;
    double mMass;
    glm::vec3 mPosition;
    glm::vec3 mVelocity;
    glm::vec3 mAcceleration;

    public:
    virtual void tick();
    void setPosition(glm::vec3 position);
    void setVelocity(glm::vec3 velocity);
    void setAcceleration(glm::vec3 acceleration);
    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    glm::vec3 getAcceleration();

};

PhysicsObject::~PhysicsObject() {}

void PhysicsObject::setPosition(glm::vec3 position) {
    mPosition = position;
}

void PhysicsObject::setVelocity(glm::vec3 velocity) {
    mVelocity = velocity;
}

void PhysicsObject::setAcceleration(glm::vec3 acceleration) {
    mAcceleration = acceleration;
}

glm::vec3 PhysicsObject::getPosition() {
    return mPosition;
}

glm::vec3 PhysicsObject::getVelocity() {
    return mVelocity;
}

glm::vec3 PhysicsObject::getAcceleration() {
    return mAcceleration;
}