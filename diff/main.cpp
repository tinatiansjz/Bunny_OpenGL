#include <iostream>
#include <cstdio>
// GLFW
#include <GLFW/glfw3.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "parser.h"
const int MAXPOINT = 40000;
const int MAXINDEX = 70000;
const char* vertexFile = "bunny_iH.ply2";
const char* normalFile = "bunny_normal.ply2";
GLfloat* vertices1 = (GLfloat*)malloc(sizeof(GLfloat)*MAXPOINT*3);
GLuint* indices1 = (GLuint*)malloc(sizeof(GLuint)*MAXINDEX*3);

GLfloat* vertices2 = (GLfloat*)malloc(sizeof(GLfloat)*MAXPOINT*6);
GLuint* indices2 = (GLuint*)malloc(sizeof(GLuint)*MAXINDEX*3);

int main(){
    GLuint vertices_size1, vertices_size2;
    GLuint indices_size1, indices_size2;
    parseIndex(vertexFile, vertices1, indices1, vertices_size1, indices_size1);
    parseNormal(normalFile, vertices2, indices2, vertices_size2, indices_size2);
    if(vertices_size1 != vertices_size2){std::cout << "Wrong!\n"; return 0;}
    if(indices_size1 != indices_size2) {std::cout << "Wrong!\n"; return 0;}
    for(int i = 0; i < vertices_size1; i++){
        if(vertices1[3*i] == vertices2[6*i] && vertices1[3*i+1] == vertices2[6*i+1] \
            && vertices1[3*i+2] == vertices2[6*i+2])
            continue;
        else
            std::cout << "Wrong! " << i << std::endl; 
    }
    for(int i = 0; i < indices_size1; i++){
        if(indices1[3*i] == indices2[3*i] && indices1[3*i+1] == indices2[3*i+1] &&\
            indices1[3*i+2] == indices2[3*i+2])
            continue;
        else
            std::cout << "Wrong! " << i << std::endl;
    }
    return 0;
}