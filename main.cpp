#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

GLfloat vertices[] = {
	// X     Y     Z
	-0.5f, -0.5f, -0.5f, // 0 - Lewy dolny ty³
	 0.5f, -0.5f, -0.5f, // 1 - Prawy dolny ty³
	 0.5f,  0.5f, -0.5f, // 2 - Prawy górny ty³
	-0.5f,  0.5f, -0.5f, // 3 - Lewy górny ty³
	-0.5f, -0.5f,  0.5f, // 4 - Lewy dolny przód
	 0.5f, -0.5f,  0.5f, // 5 - Prawy dolny przód
	 0.5f,  0.5f,  0.5f, // 6 - Prawy górny przód
	-0.5f,  0.5f,  0.5f  // 7 - Lewy górny przód
};

GLuint indices[] = {
	// Tylna œciana chuj w dupe ped³¹om
	0, 1, 2,  2, 3, 0,
	// Przednia œciana
	4, 5, 6,  6, 7, 4,
	// Lewa œciana
	0, 3, 7,  7, 4, 0,
	// Prawa œciana
	1, 5, 6,  6, 2, 1,
	// Dolna œciana
	0, 1, 5,  5, 4, 0,
	// Górna œciana
	3, 2, 6,  6, 7, 3
};



int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "maze3D", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 800);


	Shader shaderProgram("default.vert", "default.frag");


	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	EBO1.Bind();

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	Camera camera(800, 800, glm::vec3(0.0f, 0.0f, 3.0f));

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}