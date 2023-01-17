#include <iostream>
#include <chrono>
#include <algorithm>
#include <nano/nano.hpp>
#include <ProtoGL/display.hpp>
#include "objectrenderer.hpp"
#include "physicsobject.hpp"
#include "gravityhandler.hpp"

Display display("ProtoPhysics");
const double SIMULATIONTIMESTEP(1000000000.0 / 1000.0);

void scale(GLFWwindow* window, double xoffset, double yoffset) {
	glm::mat4 *view = display.getView();

	float scale = 1 / (*view)[0][0];
	float panx = (*view)[3][0];
	float pany = (*view)[3][1];

	glm::vec3 offset (panx + 0/scale, pany + 0/scale, 0);

	*view = glm::translate(glm::mat4(1.0f), offset) * glm::scale(glm::mat4(1.0f), glm::vec3( (yoffset == -1 ? 0.9 : 1/0.9 ), (yoffset == -1 ? 0.9 : 1/0.9 ), 0)) * glm::translate(glm::mat4(1.0f),-1.0f * offset) * (*view);
}

int main(void) {
    GLFWwindow* window = display.getWindow();
	
	ObjectRenderer objectRenderer;
	GravityHandler gravityHandler;

	unsigned long long int nextUID = 0;
	
	PhysicsObject sun(&nextUID), earth(&nextUID), moon(&nextUID);

	glm::mat4 *view = display.getView();
	// for(int i = 0; i < 110; i++) {
	// 	*view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3( 0.9, 0.9, 0)) * glm::translate(glm::mat4(1.0f),-1.0f * glm::vec3(0, 0, 0)) * (*view);
	// }

	sun.setPosition(glm::vec3(-5, 20, 0));
	sun.setVelocity(glm::vec3(25, 0, 0));
	sun.setMass(7.9e14);
	sun.setRadius(15);
	sun.setColor(glm::vec3(1, 0.75, 0));

	earth.setPosition(glm::vec3(0, -30, 0));
	earth.setVelocity(glm::vec3(-15, 0, 0));
	earth.setMass(7.34767309e14);
	earth.setRadius(10);
	earth.setColor(glm::vec3(0, 0.5, 1));

	moon.setPosition(glm::vec3(55, -10, 0));
	moon.setVelocity(glm::vec3(10, 40, 0));
	moon.setMass(73.4767309e10);
	moon.setRadius(6);
	moon.setColor(glm::vec3(0.75, 0.75, 0.75));

	gravityHandler.addObject(&sun);
	gravityHandler.addObject(&earth);
	gravityHandler.addObject(&moon);
	gravityHandler.setElasticity(0.5);

	objectRenderer.addObject(&sun);
	objectRenderer.addObject(&earth);
	objectRenderer.addObject(&moon);

	objectRenderer.setObjectResolution(20);


    glm::mat4 mvp;

	Shader shader("../res/shaders/basic/vertex.vsh","../res/shaders/basic/fragment.fsh");
	shader.bind();
	shader.setUniformMat4f("u_MVP",mvp);

	glfwSetScrollCallback(window, scale);

	// Event Loop
	double simulationTime = 0;
    while (!glfwWindowShouldClose(window)) {
		double frameStartTime = ns();

		/***********************************************Simulate Physics***********************************************/
		// In the event 1 loop takes longer than the simulation's timestep the simulation must catch up before rendering
		// further frames...
		for (; simulationTime >= SIMULATIONTIMESTEP; simulationTime -= SIMULATIONTIMESTEP){
			std::cout << "CATCHING UP..." << std::endl;
			gravityHandler.simulate(SIMULATIONTIMESTEP);
		}

		// uncomment this line to max out simulation rate
		// gravityHandler.simulate(SIMULATIONTIMESTEP);

		/************************************************RENDER FRAME**************************************************/
		
		mvp = (*(display.getProjection())) * (*(display.getView()));
		shader.bind();
		shader.setUniformMat4f("u_MVP",mvp);
		objectRenderer.clear();
		objectRenderer.render(&shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		/**************************************************************************************************************/

		// Simulates a frame hitch realistically this could just be done by adding some time to the simulation time;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			for (long long int i = 0; i < 9999; i++) {
				std::cout << "FRAME HITCH: " << i << std::endl;
			}
		}
		
		simulationTime += ns() - frameStartTime;
		std::cout << "Drawing..." << std::endl;	
    }
    
    glfwTerminate();
    return 0;
}

