#pragma once

#include <iostream>
#include <glm/glm.hpp>


class PhysicsObject {
    private:
    unsigned long long int mUID;
    double mMass;
    double mRadius;
    glm::dvec3 mPosition;
    glm::dvec3 mVelocity;
    glm::dvec3 mAcceleration;
    glm::dvec3 mColor;

    public:
    PhysicsObject(unsigned long long int *uid);
    PhysicsObject(const PhysicsObject &object);
    unsigned long long int getUID();
    glm::dvec3 getColor();
    void setColor(glm::dvec3 color);
    double getRadius();
    void setRadius(double radius);
    double getMass();
    void setMass(double mass);
    glm::dvec3 getPosition();
    glm::dvec3 getVelocity();
    glm::dvec3 getAcceleration();
    void setPosition(glm::dvec3 position);
    void setVelocity(glm::dvec3 velocity);
    void setAcceleration(glm::dvec3 acceleration);
    void updatePosition(glm::dvec3 position);
    void updateVelocity(glm::dvec3 velocity);
    void updateAcceleration(glm::dvec3 acceleration);
    void print();

};

PhysicsObject::PhysicsObject(unsigned long long int *uid) {
    mUID = (*uid)++;
    mPosition = glm::dvec3(0, 0, 0);
    mVelocity = glm::dvec3(0, 0, 0);
    mAcceleration = glm::dvec3(0, 0, 0);
    mMass = 1;
}

PhysicsObject::PhysicsObject(const PhysicsObject &object) {
    mUID = object.mUID;
    mMass = object.mMass;
    mRadius = object.mRadius;
    mPosition = object.mPosition;
    mVelocity = object.mVelocity;
    mAcceleration = object.mAcceleration;
    mColor = object.mColor;
}

glm::dvec3 PhysicsObject::getColor() {
    return mColor;
}

void PhysicsObject::setColor(glm::dvec3 color) {
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

glm::dvec3 PhysicsObject::getPosition() {
    return mPosition;
}

glm::dvec3 PhysicsObject::getVelocity() {
    return mVelocity;
}

glm::dvec3 PhysicsObject::getAcceleration() {
    return mAcceleration;
}

void PhysicsObject::setPosition(glm::dvec3 position) {
    mPosition = position;
}

void PhysicsObject::setVelocity(glm::dvec3 velocity) {
    mVelocity = velocity;
}

void PhysicsObject::setAcceleration(glm::dvec3 acceleration) {
    mAcceleration = acceleration;
}

void PhysicsObject::updatePosition(glm::dvec3 position) {
    mPosition += position;
}

void PhysicsObject::updateVelocity(glm::dvec3 velocity) {
    mVelocity += velocity;
}

void PhysicsObject::updateAcceleration(glm::dvec3 acceleration) {
    mAcceleration += acceleration;
}

void PhysicsObject::print() {
    std::cout << "Object : " << mUID << std::endl;
    std::cout << "Position: [ " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z << " ]" << std::endl;
    std::cout << "Velocity: [ " << mVelocity.x << ", " << mVelocity.y << ", " << mVelocity.z << " ]" << std::endl;
    std::cout << "Acceleration: [ " << mAcceleration.x << ", " << mAcceleration.y << ", " << mAcceleration.z << " ]" << std::endl;
}