#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "error.hpp"

class Display {
    private:
    GLFWwindow* mWindow;
    const GLFWvidmode* mDisplayMode;
    unsigned int mRefreshRate;
    std::string mTitle;
    glm::vec2 mDimensions;
    glm::mat4 mProjection;
    glm::mat4 mView;

    public:
    Display(std::string title, glm::vec2 dimensions, bool debug);
    ~Display();
    GLFWwindow* getWindow();
    unsigned int getRefreshRate();
    glm::vec2 getDimensions();
    glm::mat4* getProjection();
    glm::mat4* getView();

};

Display::Display(std::string title, glm::vec2 dimensions = glm::vec2(1920, 1080), bool debug = false) : mTitle(title), mDimensions(dimensions){
    if (!glfwInit()) throw "Error: GLFW could not initialize.";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Enable debugging
    if(debug) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    mWindow = glfwCreateWindow(mDimensions.x, mDimensions.y, mTitle.c_str(), NULL, NULL);

    if (!mWindow) {
        glfwTerminate();
        throw "Error: GLFW window could not be created.";
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) throw "Error: GLEW could not be initialized.";

    GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		std::cout << "** DEBUG MODE **" << std::endl;
	}
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.1, 0.1, 0.1, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    mProjection = glm::ortho(-mDimensions.x/2.0f, mDimensions.x/2.0f,-mDimensions.y/2.0f, mDimensions.y/2.0f, -1.0f, 1.0f);
	mView = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f,-0.0f,0));

}

Display::~Display(){}

GLFWwindow* Display::getWindow() {
    return mWindow;
}

glm::vec2 Display::getDimensions() {
    return mDimensions;
}

unsigned int Display::getRefreshRate() {
    return mRefreshRate;
}

glm::mat4* Display::getProjection() {
    return &mProjection;
}

glm::mat4* Display::getView() {
    return &mView;
}