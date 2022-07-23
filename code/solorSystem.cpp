#include <glew.h>
#include <GL/glut.h>
#include <time.h>
#include "shader.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

int w = 520;
int h = 520;

const int NumPoints = 3;
/*
GLfloat points[] = {  
	0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};




	this code in init() methods if i want to do ebo
	
	
*/

GLfloat points[] =
{
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };


void reshape(int W, int H)
{
	glViewport(0, 0, W, H);
	w = W;
	h = H;
}

unsigned char* textureData;

GLuint ebo;

void init(void)
{

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	//genrate ebo
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//---------------------------------------------------------------------------------------- loading TEXTURE
	//Generating a texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);// all upcoming GL_TEXTURE_2D operations now have effect on this texture object

	 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	textureData = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);

	if (textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Faild to load texture" << "\n";
	}
	stbi_image_free(textureData);
	//----------------------------------------------------------------------------------------
	
	// Load shaders and use the resulting shader program
	GLuint program = LoadShaders("./shaders/vshader.glsl", "./shaders/fshader.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "aPos");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,8 * sizeof(float),(void*)0);

	GLuint aColor = glGetAttribLocation(program, "aColor");
	glEnableVertexAttribArray(aColor);
	glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)) );

	GLuint aTexCoord = glGetAttribLocation(program, "aTexCoord");
	glEnableVertexAttribArray(aTexCoord);
	glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << "\n";

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES, 0, NumPoints);    // draw the points
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("test");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	glutMainLoop();
	
	return 0;
}
