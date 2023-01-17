#pragma once

#define GLEW_STATIC
#include <GL/glew.h>


class IndexBuffer {
    private:
    unsigned int mCount;
    unsigned int mRendererID;

    public:
    IndexBuffer();
    IndexBuffer(const IndexBuffer& indexBuffer);
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();

    void setIndices(const unsigned int *data, unsigned int count);
    void bind() const;
    void unbind() const;
    unsigned int getCount() const;

};

IndexBuffer::IndexBuffer(){}

IndexBuffer::IndexBuffer(const IndexBuffer& indexBuffer) {
    mRendererID = indexBuffer.mRendererID;
}

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : mCount(count) {
    glGenBuffers(1, &mRendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::setIndices(const unsigned int *data, unsigned int count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW);
	mCount = count;
}

unsigned int IndexBuffer::getCount() const {
    return mCount;
}