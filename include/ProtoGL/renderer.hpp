#pragma once

#include <GL/glew.h>
#include "vertexarray.hpp"
#include "indexbuffer.hpp"
#include "shader.hpp"


class Renderer{

    public:
	void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
	void clear();

};

void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const {
    shader.bind();
    vertexArray.bind();
    indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}