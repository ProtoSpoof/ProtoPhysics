#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
        }
        return 0;
    }
};

class VertexBufferLayout {
    private:
    std::vector<VertexBufferElement> mElements;
    unsigned int mStride;
    
    public:
    VertexBufferLayout();
    void push(unsigned int type, unsigned int count);
    inline const std::vector<VertexBufferElement> getElements() const;
    inline unsigned int getStride() const;

};

VertexBufferLayout::VertexBufferLayout() : mStride(0) {}

void VertexBufferLayout::push(unsigned int type, unsigned int count) {
    switch (type) {
        case GL_FLOAT:
            mElements.push_back({GL_FLOAT, count, GL_FALSE});
            mStride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
            return;
        case GL_UNSIGNED_INT:
            mElements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
            mStride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
            return;
    }
}

inline const std::vector<VertexBufferElement> VertexBufferLayout::getElements() const {
    return mElements;
}

inline unsigned int VertexBufferLayout::getStride() const {
    return mStride;
}