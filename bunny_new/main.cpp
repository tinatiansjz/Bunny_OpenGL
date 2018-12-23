#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include "parser.h"
#define INF 1.0e10
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 618;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 30.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; 
float lastFrame = 0.0f;

// lighting
glm::vec3 PointlightPos(0.0f, 15.0f, 0.0f);
// glm::vec3 PointlightPos(1.2f, 0.0f, 0.25f);

const int MAXPOINT = 40000;
const int MAXINDEX = 70000;

const char* normalFile = "bunny_normal.ply2";
GLfloat vertices[MAXPOINT*6];
GLuint indices[MAXINDEX*3];

// Attenuation or not (for point lights)
bool isAttenuation = false;
bool isFlashlight = false;
int main(){
    std::cout << "-------------------------------------------------\n";
    std::cout << "Starting GLFW context, OpenGL 3.3\n";
    std::cout << "L: Attenuation of point light\n";
    std::cout << "P: Flash light\n";
    std::cout << "-------------------------------------------------\n";
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tina's bunny", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    Shader lightingShader("bunny.vs", "bunny.fs");
    Shader lampShader("lamp.vs", "lamp.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    GLuint vertices_size, indices_size;
    parseNormal(normalFile, vertices, indices, vertices_size, indices_size);
    
    float lightVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
    };

    // first, configure the bunny's VAO (and VBO)
    unsigned int bunnyVAO, bunnyVBO, bunnyEBO;
    glGenVertexArrays(1, &bunnyVAO);
    glGenBuffers(1, &bunnyVBO);
    glGenBuffers(1, &bunnyEBO);

    glBindVertexArray(bunnyVAO);
        glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO);
        glBufferData(GL_ARRAY_BUFFER, 6*vertices_size*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*indices_size*sizeof(GLuint), indices, GL_STATIC_DRAW);
            
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbing EBO (after unbinding the VAO)

    // second, configure the light's VAO
    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO


    // render loop
    while (!glfwWindowShouldClose(window)){
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        glfwSetKeyCallback(window, key_callback);
        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        glm::vec3 lightColor;
        // lightColor.x = sin(glfwGetTime() * 2.0f);
        // lightColor.y = sin(glfwGetTime() * 0.7f);
        // lightColor.z = sin(glfwGetTime() * 1.3f);
        lightColor.x = 1.0f;
        lightColor.y = 1.0f;
        lightColor.z = 1.0f;
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

        // point light
        lightingShader.setVec3("pointLights.position", PointlightPos);
        lightingShader.setVec3("pointLights.ambient", ambientColor);
        lightingShader.setVec3("pointLights.diffuse", diffuseColor);
        lightingShader.setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights.constant", 1.0f);
        lightingShader.setFloat("pointLights.linear", (isAttenuation ? 0.014 : 0.0f));
        lightingShader.setFloat("pointLights.quadratic", (isAttenuation ? 0.0007 : 0.0f));
        
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", (isFlashlight? 1.0f : INF));
        lightingShader.setFloat("spotLight.linear", 0.045f);
        lightingShader.setFloat("spotLight.quadratic", 0.0075f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(5.0f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(9.0f)));     

        // material properties
        //lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        lightingShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);

        // render the bunny
        glBindVertexArray(bunnyVAO);
        glDrawElements(GL_TRIANGLES, 3*indices_size, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, PointlightPos);
        model = glm::scale(model, glm::vec3(0.4f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &bunnyVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &bunnyVBO);
    glDeleteBuffers(1, &lightVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_L && action == GLFW_PRESS){
        isAttenuation = !isAttenuation;
        std::cout << "Point light's attenuation is ";
        if(isAttenuation) std::cout << "on.\n";
        else std::cout << "off.\n";
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS){
        isFlashlight = !isFlashlight;
        std::cout << "Flash light is ";
        if(isFlashlight) std::cout << "on.\n";
        else std::cout << "off.\n";
    }
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}