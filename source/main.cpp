#include <GL/glew.h> // OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
	if (!glfwInit()){
		return -1;
	}
	// setup opengl context we wish to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create game window pointer
	GLFWwindow* window = glfwCreateWindow(1200, 720, "GameDevelopmentProject", nullptr, nullptr);
    if (window == nullptr){
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1; 
	}

	// position window
	glfwSetWindowPos(window, 150, 150);
	// assign window to opengl
	glfwMakeContextCurrent(window);
	if(glewInit() != GLEW_OK){
		std::cout << "Error assigning window context to glew" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// run game loop
	while(!glfwWindowShouldClose(window)){
		// set the screen colour
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}