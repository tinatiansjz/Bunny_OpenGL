#include <iostream>

// GLFW
#include <GLFW/glfw3.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include "parser.h"

const int MAXPOINT = 40000;
const int MAXINDEX = 70000;
const char* vertexFile = "bunny_iH.ply2";
const char* normalFile = "bunny_normal.ply2";
GLfloat* vertices = (GLfloat*)malloc(sizeof(GLfloat)*MAXPOINT*3);
GLuint* indices = (GLuint*)malloc(sizeof(GLuint)*MAXINDEX*3);
glm::vec3 normals[MAXPOINT];
GLfloat* vertices1 = (GLfloat*)malloc(sizeof(GLfloat)*MAXPOINT*6);
GLuint* indices1 = (GLuint*)malloc(sizeof(GLuint)*MAXINDEX*3);

void calculate_normals(const char* filename){
    GLuint vertices_size, indices_size;
    parseIndex(vertexFile, vertices, indices, vertices_size, indices_size);
    
    for(int i = 0; i < indices_size; i++){
        int ind1 = indices[3*i], ind2 = indices[3*i+1], ind3 = indices[3*i+2];
        glm::vec3 dot1 = glm::vec3(vertices[ind1*3+0], vertices[ind1*3+1], vertices[ind1*3+2]);
        glm::vec3 dot2 = glm::vec3(vertices[ind2*3+0], vertices[ind2*3+1], vertices[ind2*3+2]);
        glm::vec3 dot3 = glm::vec3(vertices[ind3*3+0], vertices[ind3*3+1], vertices[ind3*3+2]);
        glm::vec3 side1 = glm::vec3(dot1 - dot2);
        glm::vec3 side2 = glm::vec3(dot1 - dot3);
        glm::vec3 triangleNorm = glm::normalize(glm::cross(side1, side2));
        normals[ind1] += triangleNorm;
        normals[ind2] += triangleNorm;
        normals[ind3] += triangleNorm;
    }
    for(int i = 0; i < vertices_size; i++){
        normals[i] = glm::normalize(normals[i]);
    }
    std::FILE *f = std::fopen(filename, "w");
    if(f == NULL){
        std::cout << "ERROR IN CREATRING bunny_normals.ply2!\n";
        return;
    }
    std::fprintf(f, "%d\n", vertices_size);
    std::fprintf(f, "%d\n", indices_size);
    for(int i = 0; i < vertices_size; i++){
        std::fprintf(f, "%f %f %f %f %f %f\n", vertices[3*i], vertices[3*i+1], vertices[3*i+2], \
            normals[i].x, normals[i].y, normals[i].z);
    }
    for(int i = 0; i < indices_size; i++){
        std::fprintf(f, "%d %d %d\n", indices[3*i], indices[3*i+1], indices[3*i+2]);
    }
    std::fclose(f);
}
int main(){
    calculate_normals(normalFile);
    GLuint vertices_size, indices_size;
    parseNormal(normalFile, vertices1, indices1, vertices_size, indices_size);
    // std::cout << vertices_size << std::endl << indices_size << std::endl;
    // for(int i = 0; i < 3; i++){
    //     std::cout << vertices1[6*i] << " " << vertices1[6*i+1] << " " << vertices1[6*i+2] << " " << \
    //         vertices1[6*i+3] << " " << vertices1[6*i+4] << " " << vertices1[6*i+5] << std::endl;
    // }
    // std::cout << vertices1[6*(vertices_size-1)] << " " << vertices1[6*(vertices_size-1)+1] << " " << vertices1[6*(vertices_size-1)+2] << " " << \
    //     vertices1[6*(vertices_size-1)+3] << " " << vertices1[6*(vertices_size-1)+4] << " " << vertices1[6*(vertices_size-1)+5] << std::endl;
    // for(int i = 0; i < 10; i++){
    //     std::cout << indices1[3*i] << " " << indices1[3*i+1] << " " << indices1[3*i+2] << std::endl;
    // }
    // std::cout << indices1[3*(indices_size-1)] << " " << indices1[3*(indices_size-1)+1] << " " << indices1[3*(indices_size-1)+2] << std::endl;
    return 0;
}