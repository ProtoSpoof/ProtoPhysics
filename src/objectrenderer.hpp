#pragma once

#include <cmath>
#include <ProtoGL/renderer.hpp>
#include "physicsobject.hpp"

class ObjectRenderer: public Renderer {
    private:
    std::vector<PhysicsObject *> mObjects;
    IndexBuffer mObjectIndexBuffer = IndexBuffer(nullptr, 0);
    VertexBuffer mObjectVertexBuffer = VertexBuffer(nullptr, 0);
    VertexArray mObjectVertexArray;
    VertexBufferLayout mObjectVertexBufferLayout;
    unsigned int mObjectResolution;
    float* calculatePoints();
    unsigned int* calculateIndices();
    float *mUnitPoints = nullptr;

    public:
    ObjectRenderer();
    void addObject(PhysicsObject *object);
    void removeObject(PhysicsObject object);
    void removeObject(unsigned long long uid);
    void render(Shader *shader);
    void setObjectResolution(unsigned int resolution);

};

ObjectRenderer::ObjectRenderer() {
    mObjectVertexBufferLayout.push(GL_FLOAT,2);
	mObjectVertexBufferLayout.push(GL_FLOAT,4);
	mObjectVertexArray.addBuffer(mObjectVertexBuffer, mObjectVertexBufferLayout);
	mObjectVertexArray.unbind();
    mObjectIndexBuffer.unbind();
    mObjectVertexBuffer.unbind();
}

void ObjectRenderer::render(Shader *shader) {

    // std::cout << "RENDERING..." << std::endl;

    unsigned int testIndices[6] = {0,1,2,0,2,3};

    float testPoints[4 * 6] = {
        -1, -1, 1, 0, 0, 1,
        -1,  1, 0, 1, 0, 1,
         1,  1, 0, 0, 1, 1,
         1, -1, 1, 1, 1, 1,
    };

    unsigned int *indices = calculateIndices();
    float *points = calculatePoints();

    

    mObjectIndexBuffer.setIndices(indices, mObjects.size() * (mObjectResolution - 2) * 3);
    mObjectVertexBuffer.setPoints(points, sizeof(float) * mObjects.size() * mObjectResolution * 6);
    draw(mObjectVertexArray, mObjectIndexBuffer, *shader);
    free(points);
    free(indices);

    // std::cout << "FINISHED!" << std::endl;
}

unsigned int* ObjectRenderer::calculateIndices() {
    unsigned int *indices = (unsigned int*)malloc(sizeof(unsigned int) * mObjects.size() * (mObjectResolution - 2) * 3);

    for (int i = 0; i < mObjects.size(); i++) {
        for (int j = 0; j < 3 * (mObjectResolution - 2); j++) {
            // Absolute black magic to avoid an if statement... why am i like this
            indices[j + (i * 3 * (mObjectResolution - 2))] = i * mObjectResolution + (j % 3) + (bool(j % 3) * j / 3);
        }
    }

    return indices;
}

float* ObjectRenderer::calculatePoints() {
    float *points = (float *)malloc(sizeof(float) * mObjects.size() * mObjectResolution * 6);

    unsigned int count = 0;
    for (PhysicsObject* object : mObjects) {
		for (int j = 0; j < mObjectResolution; j++) {
			points[6 * (j + mObjectResolution * count) + 0] = mUnitPoints[2*j] * object->getRadius() * 500 / (500 - object->getPosition().z) + object->getPosition().x;
			points[6 * (j + mObjectResolution * count) + 1] = mUnitPoints[2*j+1] * object->getRadius() * 500 / (500 - object->getPosition().z) + object->getPosition().y;
			points[6 * (j + mObjectResolution * count) + 2] = object->getColor()[0] + object->getPosition().z / 100;
			points[6 * (j + mObjectResolution * count) + 3] = object->getColor()[1] + object->getPosition().z / 100;
			points[6 * (j + mObjectResolution * count) + 4] = object->getColor()[2] + object->getPosition().z / 100;
			points[6 * (j + mObjectResolution * count) + 5] = 0.5;
		}
        count++;
	}

    
    return points;
}


void ObjectRenderer::addObject(PhysicsObject *object) {
    mObjects.push_back(object);
}

void ObjectRenderer::removeObject(PhysicsObject object) {
    removeObject(object.getUID());
}

void ObjectRenderer::removeObject(unsigned long long uid) {
    assert("TODO: IMPLEMENT REMOVAL OF OBJECTS" && false);
}

void ObjectRenderer::setObjectResolution(unsigned int resolution) {
    mObjectResolution = resolution;

    if (mUnitPoints != nullptr) {
        free(mUnitPoints);
    } 

    mUnitPoints = (float *)malloc(sizeof(float) * mObjectResolution * 2);

    int index = 0;
	float step = 2.0f * (M_PI) / mObjectResolution;
	for(float i = 0; i < 2 * (M_PI); i += step) {
		mUnitPoints[index] = cos(i);
		mUnitPoints[index + 1] = sin(i);
        std::cout << mUnitPoints[index] << " " << mUnitPoints[index + 1] << std::endl;
		index += 2;
	}

    std::cout << std::endl;
}