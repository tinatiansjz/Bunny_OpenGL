#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "parser.h"
const int MAXPOINT = 40000;
const int MAXINDEX = 70000;
const char* filename = "bunny_iH.ply2";
GLfloat* vertices = (GLfloat*)malloc(sizeof(GLfloat)*MAXPOINT*3);
GLuint* indices = (GLuint*)malloc(sizeof(GLuint)*MAXINDEX*3);

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main(){
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Set up vertex data and attribute pointers
    GLuint vertices_size, indices_size;
    parseIndex(filename, vertices, indices, vertices_size, indices_size);
    // std::cout << vertices[0] << std::endl;
    // std::cout << indices[0] << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bunny", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader ourShader("./shader.vs", "./shader.frag");

    // Set up vertex data (and buffer(s)) and attribute pointers
    // GLfloat vertices[] = {
    //      0.5f,  0.5f, 0.0f,  // Top Right
    //      0.5f, -0.5f, 0.0f,  // Bottom Right
    //     -0.5f, -0.5f, 0.0f,  // Bottom Left
    //     -0.5f,  0.5f, 0.0f   // Top Left 
    // };
    // GLuint indices[] = {  // Note that we start from 0!
    //     0, 1, 3,  // First Triangle
    //     1, 2, 3   // Second Triangle
    // };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 3*vertices_size*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*indices_size*sizeof(GLuint), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbing EBO (after unbinding the VAO)


    // Uncommenting this call will result in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // Game loop
    while (!glfwWindowShouldClose(window)){
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);

        // Create transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the triangle
        ourShader.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3*indices_size, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}