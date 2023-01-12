#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include "vertexbufferlayout.hpp"
#include "vertexbuffer.hpp"

class VertexArray {
    private:
    unsigned int mRendererID;
    
    public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;
    void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout);
};

VertexArray::VertexArray(){
	glGenVertexArrays(1,&mRendererID);
}

VertexArray::~VertexArray(){
	glDeleteVertexArrays(1,&mRendererID);
}

void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout){
	bind();
	vertexBuffer.bind();
	const auto& elements = vertexBufferLayout.getElements();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < elements.size();i++ ){
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,element.count, element.type, element.normalized, vertexBufferLayout.getStride(), (const void*) offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(mRendererID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}