#pragma once

#include <iostream>
#include <glm/glm.hpp>


class PhysicsObject {
    private:
    unsigned long long int mUID;
    double mMass;
    double mRadius;
    glm::vec3 mPosition;
    glm::vec3 mVelocity;
    glm::vec3 mAcceleration;
    glm::vec3 mColor;

    public:
    PhysicsObject(unsigned long long int *uid);
    unsigned long long int getUID();
    glm::vec3 getColor();
    void setColor(glm::vec3 color);
    double getRadius();
    void setRadius(double radius);
    double getMass();
    void setMass(double mass);
    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    glm::vec3 getAcceleration();
    void setPosition(glm::vec3 position);
    void setVelocity(glm::vec3 velocity);
    void setAcceleration(glm::vec3 acceleration);
    void updatePosition(glm::vec3 position);
    void updateVelocity(glm::vec3 velocity);
    void updateAcceleration(glm::vec3 acceleration);
    void print();

};

PhysicsObject::PhysicsObject(unsigned long long int *uid) {
    mUID = (*uid)++;
    mPosition = glm::vec3(0, 0, 0);
    mVelocity = glm::vec3(0, 0, 0);
    mAcceleration = glm::vec3(0, 0, 0);
    mMass = 1;
}

glm::vec3 PhysicsObject::getColor() {
    return mColor;
}

void PhysicsObject::setColor(glm::vec3 color) {
    mColor = color;
}

double PhysicsObject::getRadius() {
    return mRadius;
}

void PhysicsObject::setRadius(double radius) {
    mRadius = radius;
}

double PhysicsObject::getMass() {
    return mMass;
}

void PhysicsObject::setMass(double mass) {
    mMass = mass;
}

unsigned long long int PhysicsObject::getUID() {
    return mUID;
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

void PhysicsObject::setPosition(glm::vec3 position) {
    mPosition = position;
}

void PhysicsObject::setVelocity(glm::vec3 velocity) {
    mVelocity = velocity;
}

void PhysicsObject::setAcceleration(glm::vec3 acceleration) {
    mAcceleration = acceleration;
}

void PhysicsObject::updatePosition(glm::vec3 position) {
    mPosition += position;
}

void PhysicsObject::updateVelocity(glm::vec3 velocity) {
    mVelocity += velocity;
}

void PhysicsObject::updateAcceleration(glm::vec3 acceleration) {
    mAcceleration += acceleration;
}

void PhysicsObject::print() {
    std::cout << "Object : " << mUID << std::endl;
    std::cout << "Position: [ " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z << " ]" << std::endl;
    std::cout << "Velocity: [ " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << " ]" << std::endl;
    std::cout << "Acceleration: [ " << mAcceleration.x << ", " << mAcceleration.y << ", " << mAcceleration.z << " ]" << std::endl;
}