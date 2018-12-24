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
void mouse_click_callback(GLFWwindow *window, int button, int action, int mods);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void do_movement();

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
// bunny
glm::mat4 modelBunny = glm::mat4(1.0f);
GLuint vertices_size, indices_size;
// lighting
glm::vec3 PointlightPos(0.0f, 15.0f, 0.0f);

//Mouse
GLfloat mouseX = SCR_WIDTH / 2.0;
GLfloat mouseY = SCR_HEIGHT / 2.0;
GLuint selectedPointIndice = 0 - 1; // max

const int MAXPOINT = 40000;
const int MAXINDEX = 70000;

const char* normalFile = "bunny_normal.ply2";
const char *SPLIT = "--------------------------------------------------------------";
GLfloat vertices[MAXPOINT*6];
GLuint indices[MAXINDEX*3];

bool keys[1024];
bool isAttenuation = false;
bool isFlashlight = false;
bool cursorDisabled = true;

void description(){
    std::cout << SPLIT << std::endl;
    std::cout << "Starting GLFW context, OpenGL 3.3\n";
    std::cout << "=: Attenuation of point light\n";
    std::cout << "P: Flash light\n";
    std::cout << "A: Camera moves to the left.\n";
    std::cout << "D: Camera moves to the right.\n";
    std::cout << "W: Camera moves forward.\n";
    std::cout << "S: Camera moves backward.\n";
    std::cout << "<: Bunny moves left.\n";
    std::cout << ">: Bunny moves right.\n";
    std::cout << "J: Bunny rotates left.\n";
    std::cout << "L: Bunny rotates right.\n";
    std::cout << "I: Bunny rotates forward.\n";
    std::cout << "K: Bunny rotates backward.\n";
    std::cout << "Z: Bunny zooms in.\n";
    std::cout << "X: Bunny zooms out.\n";
    std::cout << "Tab: Wireframe mode(enable cursor) | default mode(disable cursor)\n";
    std::cout << SPLIT << std::endl;
}
GLFWwindow* init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tina's bunny", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_SUCCESS);
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    return window;
}
int main(){
    // glfw: initialize and configure
    description();
    GLFWwindow* window = init();

    // build and compile our shader zprogram
    Shader lightingShader("bunny.vs", "bunny.fs");
    Shader lampShader("lamp.vs", "lamp.fs");
    Shader selectShader("select.vs", "select.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    
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
    //bunny's initial state
    modelBunny = glm::rotate(modelBunny, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // render loop
    while (!glfwWindowShouldClose(window)){
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        do_movement();
        if(!cursorDisabled) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.2f);
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.2f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        lightingShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", modelBunny);

        // render the bunny
        glBindVertexArray(bunnyVAO);
        glDrawElements(GL_TRIANGLES, 3*indices_size, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, PointlightPos);
        model = glm::scale(model, glm::vec3(0.4f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Select Point
        if (selectedPointIndice <= vertices_size){
            selectShader.use();
            glm::vec4 now(modelBunny * glm::vec4(vertices[selectedPointIndice * 6], vertices[selectedPointIndice * 6 + 1], vertices[selectedPointIndice * 6 + 2], 1.f));
            model = glm::translate(glm::mat4(1.f), glm::vec3(now.x, now.y, now.z));
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
            view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            selectShader.setMat4("projection", projection);
            selectShader.setMat4("view", view);
            selectShader.setMat4("model", model);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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
void do_movement(){
    GLfloat bunnySpeed = 30.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    // <- bunny move left
    if (keys[GLFW_KEY_LEFT])
        modelBunny = glm::translate(modelBunny, glm::vec3(-bunnySpeed, 0, 0));
    // -> bunny move right
    if (keys[GLFW_KEY_RIGHT])
        modelBunny = glm::translate(modelBunny, glm::vec3(bunnySpeed, 0, 0));
    // J bunny rotate left
    if (keys[GLFW_KEY_J])
        modelBunny = glm::rotate(modelBunny, glm::radians(bunnySpeed), glm::vec3(0.f, 0.f, 1.f));
    // L bunny rotate right
    if (keys[GLFW_KEY_L])
        modelBunny = glm::rotate(modelBunny, glm::radians(-bunnySpeed), glm::vec3(0.f, 0.f, 1.f));
    // I bunny rotate forward
    if (keys[GLFW_KEY_I])
        modelBunny = glm::rotate(modelBunny, glm::radians(-bunnySpeed), glm::vec3(1.f, 0.f, 0.f));
    // K bunny rotate backward
    if (keys[GLFW_KEY_K])
        modelBunny = glm::rotate(modelBunny, glm::radians(bunnySpeed), glm::vec3(1.f, 0.f, 0.f));
    // Z bunny zooms in
    if (keys[GLFW_KEY_Z])
        modelBunny = glm::scale(modelBunny, glm::vec3(1.0f - 0.001f * bunnySpeed, 1.0f - 0.001f * bunnySpeed, 1.0f - 0.001f * bunnySpeed));
    // X bunny zooms out
    if (keys[GLFW_KEY_X])
        modelBunny = glm::scale(modelBunny, glm::vec3(1.0f + 0.001f * bunnySpeed, 1.0f + 0.001f * bunnySpeed, 1.0f + 0.001f * bunnySpeed));
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS){
        isAttenuation = !isAttenuation;
        std::cout << "Point light's attenuation is ";
        if(isAttenuation) std::cout << "on.\n";
        else std::cout << "off.\n";
        std::cout << SPLIT << std::endl;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS){
        isFlashlight = !isFlashlight;
        std::cout << "Flash light is ";
        if(isFlashlight) std::cout << "on.\n";
        else std::cout << "off.\n";
        std::cout << SPLIT << std::endl;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS){
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorDisabled = false;
        }else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorDisabled = true;
            firstMouse = true;
        }
    }
    if (key >= 0 && key < 1024){
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
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
    mouseX = xpos;
    mouseY = ypos;
    if(cursorDisabled){
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
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}
// take points
void mouse_click_callback(GLFWwindow *window, int button, int action, int mods){
    if (!cursorDisabled && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_MOUSE_BUTTON_LEFT){
        GLfloat xpos = mouseX;
        GLfloat ypos = mouseY;
        std::cout << "Screen Position : " << xpos << ' ' << ypos << std::endl;
        GLfloat minDistance = glm::pow(10, 20);
        GLuint minIndice = 0 - 1;
        GLfloat minX, minY;
        for (int i = 0; i < vertices_size; i++){
            if (glm::dot(glm::mat3(glm::transpose(glm::inverse(modelBunny))) * \
            glm::vec3(vertices[6*i+3], vertices[6*i+4], vertices[6*i+5]), camera.Front) < 0){
                glm::vec4 iPos;
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
                iPos = modelBunny * glm::vec4(vertices[i*6 + 0], vertices[i*6 + 1], vertices[i*6 + 2], 1.0f);
                iPos = projection * view * iPos;
                GLfloat pointPosX = SCR_WIDTH / 2 * (iPos.x / iPos.w) + SCR_WIDTH / 2;
                // Attention to the negative sign here
                GLfloat pointPosY = SCR_HEIGHT / 2 * (-iPos.y / iPos.w) + SCR_HEIGHT / 2;
                if ((pointPosX - xpos) * (pointPosX - xpos) + (pointPosY - ypos) * (pointPosY - ypos) < minDistance){
                    minDistance = (pointPosX - xpos) * (pointPosX - xpos) + (pointPosY - ypos) * (pointPosY - ypos);
                    minIndice = i;
                    minX = pointPosX;
                    minY = pointPosY;
                }
            }
        }
        // distance <20
        if (minDistance < 400){
            selectedPointIndice = minIndice;
            std::cout << "The point indice is : " << minIndice << std::endl;
            std::cout << "The point position is : " << vertices[minIndice * 6 + 0] << ' ' << vertices[minIndice * 6 + 1] << ' ' << vertices[minIndice * 6 + 2] << std::endl;
            std::cout << "The point screen position is : " << minX << ' ' << minY << std::endl;
        }else{
            std::cout << "No point nearby (The distance between the cursor and the nearest point is less than 20 pixels)" << std::endl;
        }
        std::cout << SPLIT << std::endl;
    }
}