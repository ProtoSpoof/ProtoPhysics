#include <iostream>
#include <nano/nano.hpp>
#include <ProtoGL/display.hpp>
#include <ProtoGL/renderer.hpp>

int main(void) {
    Display display("ProtoPhysics"); 
    Renderer renderer;
    GLFWwindow* window = display.getWindow();
    glm::mat4 mvp;

    unsigned int frames = 0;
	unsigned int framerate;
	double elapsedTime;
	double timeStart = ns() / 1000000000.0;
	double frameStart;
	double frameEnd;
	double deltaTimeS;

    while (!glfwWindowShouldClose(window)) {
        frameStart = ns() / 1000000000.0;

        renderer.clear();
        glfwSwapBuffers(window);
        glfwPollEvents();

        elapsedTime = ns() / 1000000000.0 - timeStart;

        if (elapsedTime >= 1) {
            framerate = frames;
            timeStart = ns() / 1000000000.0;
            frames = 0;
            std::cout << "FPS: " << framerate << std::endl;
        }

        frames++;
        frameEnd = ns() / 1000000000.0;
        deltaTimeS = frameEnd - frameStart;
    }
    
    glfwTerminate();
    return 0;
}