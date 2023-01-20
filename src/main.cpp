#include <iostream>
#include <chrono>
#include <algorithm>
#include <nano/nano.hpp>
#include <ProtoGL/display.hpp>
#include "objectrenderer.hpp"
#include "physicsobject.hpp"
#include "gravityhandler.hpp"

Display display("ProtoPhysics", glm::dvec2(1920,1080));
const double SIMULATIONTIMESTEP(10);

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
	GravityHandler gravityHandlerEuler;
	GravityHandler gravityHandlerRK4;

	unsigned long long int nextUID = 0;
	
	PhysicsObject obj1(&nextUID), obj2(&nextUID), obj3(&nextUID);

	// glm::mat4 *view = display.getView();
	// for(int i = 0; i < 131; i++) {
	// 	*view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3( 0.9, 0.9, 0)) * glm::translate(glm::mat4(1.0f), -1.0f * glm::vec3(0, 0, 0)) * (*view);
	// }

	obj1.setPosition(glm::dvec3(-200));
	obj2.setPosition(glm::dvec3(200));
	obj3.setPosition(glm::dvec3(40, -30, 60));

	obj1.setVelocity(glm::dvec3(0));
	obj2.setVelocity(glm::dvec3(0));
	obj3.setVelocity(glm::dvec3(0));

	obj1.setRadius(100);
	obj2.setRadius(50);
	obj3.setRadius(75);

	obj1.setMass(300000);
	obj2.setMass(1000000);
	obj3.setMass(2000);

	obj1.setColor(glm::dvec3(1, 0, 0));
	obj2.setColor(glm::dvec3(1, 0, 0));
	obj3.setColor(glm::dvec3(1, 0, 0));

	PhysicsObject obj1RK4(obj1), obj2RK4(obj2), obj3RK4(obj3);


	obj1RK4.setColor(glm::dvec3(0, 1, 1));
	obj2RK4.setColor(glm::dvec3(0, 1, 1));
	obj3RK4.setColor(glm::dvec3(0, 1, 1));


	// gravityHandlerEuler.addObject(&obj1);
	gravityHandlerEuler.addObject(&obj2);
	gravityHandlerEuler.addObject(&obj3);
	
	// gravityHandlerRK4.addObject(&obj1RK4);
	gravityHandlerRK4.addObject(&obj2RK4);
	gravityHandlerRK4.addObject(&obj3RK4);
	

	gravityHandlerEuler.setElasticity(1);
	gravityHandlerRK4.setElasticity(1);

	// objectRenderer.addObject(&obj1);
	objectRenderer.addObject(&obj2);
	objectRenderer.addObject(&obj3);
	
	// objectRenderer.addObject(&obj1RK4);
	objectRenderer.addObject(&obj2RK4);
	objectRenderer.addObject(&obj3RK4);

	objectRenderer.setObjectResolution(20);


    glm::mat4 mvp;

	Shader shader("../res/shaders/basic/vertex.vsh","../res/shaders/basic/fragment.fsh");
	shader.bind();
	shader.setUniformMat4f("u_MVP",mvp);

	glfwSetScrollCallback(window, scale);

	// Event Loop
	long double totalTime = 0;
	double simulationTime = 0;
    while (!glfwWindowShouldClose(window)) {
		double frameStartTime = ns() / 1000000000;

		/***********************************************Simulate Physics***********************************************/
		// In the event 1 loop takes longer than the simulation's timestep the simulation must catch up before rendering
		// further frames... This will in turn lock the simulation time to that of real life
		// i.e. 1 simulated second = 1 real second
		// for (; simulationTime >= SIMULATIONTIMESTEP; simulationTime -= SIMULATIONTIMESTEP){
		// 	// std::cout << "CATCHING UP..." << std::endl;
		// 	gravityHandler.simulate(SIMULATIONTIMESTEP);
		// }

		gravityHandlerEuler.eulerUpdate(SIMULATIONTIMESTEP);
		gravityHandlerRK4.rk4Update(SIMULATIONTIMESTEP);

		gravityHandlerEuler.applyCollisions();
		gravityHandlerRK4.applyCollisions();
		
		/**************************************************************************************************************/


		/************************************************RENDER FRAME**************************************************/
		// Only draws the frame approximately 160 times every second. Allows for more physics calculations per second at
		// the expense of the simulations visuals at runtime. This tradeoff can be completely ignored by offloading the
		// rendering process to another program and simply export the data created by the simulated physics...
		// if (simulationTime >= 1.0/160.0) {
			mvp = (*(display.getProjection())) * (*(display.getView()));
			shader.bind();
			shader.setUniformMat4f("u_MVP",mvp);
			objectRenderer.clear();
			objectRenderer.render(&shader);
			glfwSwapBuffers(window);
			simulationTime -= 1.0/160.0;
		// }
		
		/**************************************************************************************************************/
		
		glfwPollEvents();

		// if (2.4e+6 <= gravityHandler.getElapsedSimTime()) {
		// 	std::cout << "1 orbit should have been achieved..." << std::endl;
		// 	// while (!glfwGetKey(window, GLFW_KEY_ESCAPE));
		// 	goto END;
		// }
		
		auto timeDelta = ns() / 1000000000 - frameStartTime;

		// simulationTime += timeDelta;
		totalTime += timeDelta;
		// std::cout << "Drawing..." << std::endl;	
    }
	END:
    std::cout << "Elapsed Real Time: " << totalTime << " seconds" << std::endl;
	std::cout << "Elapsed Euler Time: " << gravityHandlerEuler.getElapsedSimTime() << " seconds" << std::endl;
	std::cout << "Elapsed RK4 Time: " << gravityHandlerRK4.getElapsedSimTime() << " seconds" << std::endl;

    glfwTerminate();
    return 0;
}

