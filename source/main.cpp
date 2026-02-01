#include <GL/glew.h> // OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector> // used for storing vertices data
#include <string> // used for storing vertex shader code

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

	std::cout << "GL: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	// this code is used by the vertex shader, uses version 330 core,
	// says that the info being passed into it should be interpreted as contain x,y,z values
	// uses a function to apply that position data
	std::string vertextShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 color;

		out vec3 vColor;

		void main()
		{
			vColor = color;
			gl_Position = vec4(position.x, position.y, position.z, 1.0);
		}
	)";

	// compile the vertex shader
	GLuint vertextShader = glCreateShader(GL_VERTEX_SHADER);
	// pass the shader source to the vertex shader
	const char* vertextShaderCStr = vertextShaderSource.c_str();
	glShaderSource(vertextShader, 1, &vertextShaderCStr, nullptr);
	glCompileShader(vertextShader);
	
	GLint success;
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
	if (!success){
		char infoLog[512];
		glGetShaderInfoLog(vertextShader, 512, nullptr, infoLog);
		std::cerr << "ERROR: VERTEX_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;
	}

	// define the fragment shader source, which will output a simple frag colour.
	std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 fragColor;

		in vec3 vColor;
		uniform vec4 uColor;

		void main()
		{
			fragColor = vec4(vColor, 1.0) * uColor;
		}
	)";

	// create the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
	glCompileShader(fragmentShader);	

	// check the fragment shader works
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "ERROR: FRAGMENT_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;
	}

	// Combine vertex and fragment shaders into a single shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertextShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	// check the shader program creation worked
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success){
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "ERROR: SHADER_PROGRAM_LINKING_FAILED: " << infoLog << std::endl;
	}

	// once the program has completed, delete the shaders
	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader);

	std::vector<float> vertices = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
	};

	std::vector<unsigned int> indices = 
	{
		0, 1, 2,
		0, 2, 3
	};

	// we load our vertices data in GPU data via a buffer
	// create a handle ID for the GPU buffer
	GLuint vbo;
	glGenBuffers( 1, &vbo);
	// make it the current buffer for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // This defines a buffer that'll accept vertices data
	// we then bind data to the buffer, which we know the size and data type of
	// then say it'll be remaining on the screen as a static object
	// the GPU will now store vertex position
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW );
	// delete the buffer once used
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint ebo;
	glGenBuffers( 1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW );
	// delete the buffer once used
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// we now need to tell the shader how to access the vertex data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// here we say how the vertex data should be accessed by the shader,
	// start position, number of positions, normalise, memory size and offset
	// define info from index 0
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// define info from index 1
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// delete the buffer and vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");

	// run game loop
	while(!glfwWindowShouldClose(window)){
		// set the screen colour
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform4f(uColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
		glBindVertexArray(vao);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}