#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "parser.h"
const char* filename = "bunny_iH.ply2";
GLfloat* vertices = (GLfloat*)malloc(sizeof(GLfloat)*34834*3);
GLuint* indices = (GLuint*)malloc(sizeof(GLuint)*69451*3);

int main(){
    GLuint vertices_size, indices_size;
    parseIndex(filename, vertices, indices, vertices_size, indices_size);
    std::cout << vertices_size << std::endl << indices_size << std::endl;
    for(int i = 0; i < 10; i++){
        std::cout << indices[3*i] << " " << indices[3*i+1] << " " << indices[3*i+2] << std::endl;
    }
    return 0;
}