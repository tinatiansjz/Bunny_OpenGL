#include <iostream>
#include <cstdio>
// GLFW
#include <GLFW/glfw3.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void parseIndex(const char* fileName, GLfloat* vertices, GLuint* indices, GLuint& vertices_size, GLuint& indices_size){
    std::FILE* f = std::fopen(fileName, "r");
    std::fscanf(f, "%d%d", &vertices_size, &indices_size);
    // cout << vertices_size << endl << indices_size << endl;
    
    for(int i = 0; i < vertices_size; i++){
        std::fscanf(f, "%f%f%f", &vertices[3*i], &vertices[3*i+1], &vertices[3*i+2]);
    }
    for(int i = 0; i < indices_size; i++){
        int _;
        std::fscanf(f, "%d%d%d%d", &_, &indices[3*i], &indices[3*i+1], &indices[3*i+2]);
    }
    std::fclose(f);
}
void parseNormal(const char* fileName, GLfloat* vertices, GLuint* indices, GLuint& vertices_size, GLuint& indices_size){
    std::FILE* f = std::fopen(fileName, "r");
    std::fscanf(f, "%d%d", &vertices_size, &indices_size);
    // cout << vertices_size << endl << indices_size << endl;
    
    for(int i = 0; i < vertices_size; i++){
        std::fscanf(f, "%f%f%f%f%f%f", &vertices[6*i], &vertices[6*i+1], &vertices[6*i+2], \
            &vertices[6*i+3], &vertices[6*i+4], &vertices[6*i+5]);
    }
    for(int i = 0; i < indices_size; i++){
        std::fscanf(f, "%d%d%d", &indices[3*i], &indices[3*i+1], &indices[3*i+2]);
    }
    std::fclose(f);
}