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

	//------Select Box-------
	unsigned int square[6] = {0,1,2,0,2,3};
	VertexBuffer Svb(nullptr,sizeof(float) * 6 * 4);
	IndexBuffer Sib(nullptr,6);
	VertexArray Sva;
	VertexBufferLayout Slayout;
	//Position Vec 2
	Slayout.push(GL_FLOAT,2);
	//Color Vec 4
	Slayout.push(GL_FLOAT,4);
	Sva.addBuffer(Svb,Slayout);
	Sva.unbind();
	Svb.unbind();
	Sib.unbind();
	//--------------------------

	Shader shader("../res/shaders/basic/vertex.vsh","../res/shaders/basic/fragment.fsh");
	shader.bind();
	shader.setUniformMat4f("u_MVP",mvp);

	float* points = (float *)malloc(sizeof(float) * 24);
	//each vertex
	for(int i = 0; i < 4; i++) {
		points[6 * i + 0] = (i < 2 ? 100 : -100);
		points[6 * i + 1] = (i == 1 || i == 2 ? -100 : 100);
		points[6 * i + 2] = 1.0f;
		points[6 * i + 3] = 0.5f;
		points[6 * i + 4] = 0.5f;
		points[6 * i + 5] = 0.3f;
	}

    while (!glfwWindowShouldClose(window)) {
        frameStart = ns() / 1000000000.0;

		mvp = (*(display.getProjection())) * (*(display.getView()));
		shader.bind();
		shader.setUniformMat4f("u_MVP",mvp);
        renderer.clear();

		// Drawing

		Sib.setIndices(square,6);
		Svb.setPoints((void*)points,sizeof(float) * 6 * 4);
		renderer.draw(Sva,Sib,shader);

		//

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