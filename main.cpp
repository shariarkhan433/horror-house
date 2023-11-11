//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "cube.h"
#include "stb_image.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void bed(Shader& lightingShader, glm::mat4 alTogether, Cube& cube);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, -0.2f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// lights
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.50f,  0.70f,  -7.90f),
    glm::vec3(6.0f,  0.70f,  0.0f),
    glm::vec3(-1.5f,  1.5f,  0.0f),
    glm::vec3(-3.4f,  0.70f,  -4.0f),
    glm::vec3(1.5f,  -1.5f,  0.70f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);


// light settings
bool dirLightOn = true;
bool pointLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AMBATUKAM", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

   

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    string diffuseMapPath = "ghost.jpg";
    string specularMapPath = "ghost.jpg";


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //wall 1
    string diffuseMapPath1 = "wall.jpg";
    string specularMapPath1 = "wall.jpg";


    unsigned int diffMap1 = loadTexture(diffuseMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap1 = loadTexture(specularMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap1, specMap1, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 1 side wall 1
    string diffuseMapPath2 = "wall.jpg";
    string specularMapPath2 = "wall.jpg";


    unsigned int diffMap2 = loadTexture(diffuseMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //second wall
    string diffuseMapPath3 = "wall.jpg";
    string specularMapPath3 = "wall.jpg";


    unsigned int diffMap3 = loadTexture(diffuseMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //room 1 side wall 2
    string diffuseMapPath4 = "wall.jpg";
    string specularMapPath4 = "wall.jpg";


    unsigned int diffMap4 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube4 = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 1 side wall 2 door 
    string diffuseMapPath5 = "wall.jpg";
    string specularMapPath5 = "wall.jpg";


    unsigned int diffMap5 = loadTexture(diffuseMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube5 = Cube(diffMap5, specMap5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 1 side wall 2 door top
    string diffuseMapPath6 = "wall.jpg";
    string specularMapPath6 = "wall.jpg";


    unsigned int diffMap6 = loadTexture(diffuseMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap6 = loadTexture(specularMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube6 = Cube(diffMap6, specMap6, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //floor one
    string diffuseMapPath7 = "floor.jpg";
    string specularMapPath7 = "floor.jpg";


    unsigned int diffMap7 = loadTexture(diffuseMapPath7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap7 = loadTexture(specularMapPath7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube7 = Cube(diffMap7, specMap7, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //celling one
    string diffuseMapPath8 = "wall.jpg";
    string specularMapPath8 = "wall.jpg";


    unsigned int diffMap8 = loadTexture(diffuseMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap8 = loadTexture(specularMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube8 = Cube(diffMap8, specMap8, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //floor 2
    string diffuseMapPath9 = "floor.jpg";
    string specularMapPath9 = "floor.jpg";


    unsigned int diffMap9 = loadTexture(diffuseMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap9 = loadTexture(specularMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube9 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //celling 2
    string diffuseMapPath10 = "celling.jpg";
    string specularMapPath10 = "celling.jpg";
    unsigned int diffMap10 = loadTexture(diffuseMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap10 = loadTexture(specularMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube10 = Cube(diffMap10, specMap10, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 2 wall 1
    string diffuseMapPath11 = "wall.jpg";
    string specularMapPath11 = "wall.jpg";
    unsigned int diffMap11 = loadTexture(diffuseMapPath11.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap11 = loadTexture(specularMapPath11.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube11 = Cube(diffMap11, specMap11, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 2 wall 1 door top
    string diffuseMapPath12 = "wall.jpg";
    string specularMapPath12 = "wall.jpg";
    unsigned int diffMap12 = loadTexture(diffuseMapPath12.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap12 = loadTexture(specularMapPath12.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube12 = Cube(diffMap12, specMap12, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 2 wall 1 last part
    string diffuseMapPath13 = "wall.jpg";
    string specularMapPath13 = "wall.jpg";
    unsigned int diffMap13 = loadTexture(diffuseMapPath13.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap13 = loadTexture(specularMapPath13.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube13 = Cube(diffMap13, specMap13, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 2 side wall 1
    string diffuseMapPath14 = "wall.jpg";
    string specularMapPath14 = "wall.jpg";
    unsigned int diffMap14 = loadTexture(diffuseMapPath14.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap14 = loadTexture(specularMapPath14.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube14 = Cube(diffMap14, specMap14, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 2 wall 2
    string diffuseMapPath15 = "wall.jpg";
    string specularMapPath15 = "wall.jpg";
    unsigned int diffMap15 = loadTexture(diffuseMapPath15.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap15 = loadTexture(specularMapPath15.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube15 = Cube(diffMap15, specMap15, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //floor 3
    string diffuseMapPath16 = "floor.jpg";
    string specularMapPath16 = "floor.jpg";


    unsigned int diffMap16 = loadTexture(diffuseMapPath16.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap16 = loadTexture(specularMapPath16.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube16 = Cube(diffMap16, specMap16, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //celling 3
    string diffuseMapPath17 = "celling.jpg";
    string specularMapPath17 = "celling.jpg";


    unsigned int diffMap17 = loadTexture(diffuseMapPath17.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap17 = loadTexture(specularMapPath17.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube17 = Cube(diffMap17, specMap17, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 3 sidewaLL 1
    string diffuseMapPath18 = "wall.jpg";
    string specularMapPath18 = "wall.jpg";


    unsigned int diffMap18 = loadTexture(diffuseMapPath18.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap18 = loadTexture(specularMapPath18.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube18 = Cube(diffMap18, specMap18, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 3 window wall 1
    string diffuseMapPath19 = "wall.jpg";
    string specularMapPath19 = "wall.jpg";


    unsigned int diffMap19 = loadTexture(diffuseMapPath19.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap19 = loadTexture(specularMapPath19.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube19 = Cube(diffMap19, specMap19, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 3 window top
    string diffuseMapPath20 = "wall.jpg";
    string specularMapPath20 = "wall.jpg";


    unsigned int diffMap20 = loadTexture(diffuseMapPath20.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap20 = loadTexture(specularMapPath20.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube20 = Cube(diffMap20, specMap20, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 3 window bottom
    string diffuseMapPath21 = "wall.jpg";
    string specularMapPath21 = "wall.jpg";


    unsigned int diffMap21 = loadTexture(diffuseMapPath21.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap21 = loadTexture(specularMapPath21.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube21 = Cube(diffMap21, specMap21, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 3 second wall
    string diffuseMapPath22 = "wall.jpg";
    string specularMapPath22 = "wall.jpg";


    unsigned int diffMap22 = loadTexture(diffuseMapPath22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap22 = loadTexture(specularMapPath22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube22 = Cube(diffMap22, specMap22, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 3 sidewall2
    string diffuseMapPath23 = "wall.jpg";
    string specularMapPath23 = "wall.jpg";


    unsigned int diffMap23 = loadTexture(diffuseMapPath23.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap23 = loadTexture(specularMapPath23.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube23 = Cube(diffMap23, specMap23, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 3 sidewall2
    string diffuseMapPath24 = "wall.jpg";
    string specularMapPath24 = "wall.jpg";


    unsigned int diffMap24 = loadTexture(diffuseMapPath24.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap24 = loadTexture(specularMapPath24.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube24 = Cube(diffMap24, specMap24, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //room 4
    string diffuseMapPath25 = "floor.jpg";
    string specularMapPath25 = "floor.jpg";


    unsigned int diffMap25 = loadTexture(diffuseMapPath25.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap25 = loadTexture(specularMapPath25.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube25 = Cube(diffMap25, specMap25, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        //point light 5
        pointlight5.setUpPointLight(lightingShaderWithTexture);

        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(model, glm::vec3(-0.45f, -0.4f, -2.8f));
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //first wall
        

        glm::mat4 modelMatrixForContainer1 = glm::mat4(1.0f);
        modelMatrixForContainer1 = glm::translate(model, glm::vec3(-8.0f, 1.2f, -5.0f));
        modelMatrixForContainer1 = glm::scale(modelMatrixForContainer1, glm::vec3(10.0f, -4.45f, -0.3f));
        cube1.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer1);

        //room 1 side 1
        

        glm::mat4 modelMatrixForContainer2 = glm::mat4(1.0f);
        modelMatrixForContainer2 = glm::translate(model, glm::vec3(-8.5f, 1.2f, 1.4f));
        modelMatrixForContainer2 = glm::scale(modelMatrixForContainer2, glm::vec3(0.5f, -4.45f, -6.5f));
        cube2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer2);

        //second wall
       

        glm::mat4 modelMatrixForContainer3 = glm::mat4(1.0f);
        modelMatrixForContainer3 = glm::translate(model, glm::vec3(-8.0f, 1.2f, 1.7f));
        modelMatrixForContainer3 = glm::scale(modelMatrixForContainer3, glm::vec3(10.0f, -4.45f, -0.3f));
        cube3.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        // also draw the lamp object(s)
     

        //room 1 side 2 door side 1
        lightingShaderWithTexture.use();
       
        

        glm::mat4 modelMatrixForContainer4 = glm::mat4(1.0f);
        modelMatrixForContainer4 = glm::translate(model, glm::vec3(2.0f, 1.2f, -2.5f));
        modelMatrixForContainer4 = glm::scale(modelMatrixForContainer4, glm::vec3(0.5f, -4.45f, -2.5f));
        cube4.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        //room 1 side 2 door side 2
       

        glm::mat4 modelMatrixForContainer5 = glm::mat4(1.0f);
        modelMatrixForContainer5 = glm::translate(model, glm::vec3(2.0f, 1.2f, 1.65f));
        modelMatrixForContainer5 = glm::scale(modelMatrixForContainer5, glm::vec3(0.5f, -4.45f, -2.5f));
        cube5.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer5);

        //room 1 side 2 door top
       

        glm::mat4 modelMatrixForContainer6 = glm::mat4(1.0f);
        modelMatrixForContainer6 = glm::translate(model, glm::vec3(2.0f, 1.2f, -0.80f));
        modelMatrixForContainer6 = glm::scale(modelMatrixForContainer6, glm::vec3(0.5f, -1.45f, -1.7f));
        cube6.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer6);

        //first FLoor

        lightingShaderWithTexture.use();
        

        glm::mat4 modelMatrixForContainer7 = glm::mat4(1.0f);
        modelMatrixForContainer7 = glm::translate(model, glm::vec3(2.0f, -2.92f, 1.40f));
        modelMatrixForContainer7 = glm::scale(modelMatrixForContainer7, glm::vec3(-10.0f, -0.30f, -6.5f));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer7);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //first celling
       

        glm::mat4 modelMatrixForContainer8 = glm::mat4(1.0f);
        modelMatrixForContainer8 = glm::translate(model, glm::vec3(2.0f, 1.2f, 1.40f));
        modelMatrixForContainer8 = glm::scale(modelMatrixForContainer8, glm::vec3(-10.0f, -0.30f, -6.5f));
        cube8.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer8);


                        //          THE SECOND ROOM                                     THE SECOND ROOM

        //second floor

      

        glm::mat4 modelMatrixForContainer9 = glm::mat4(1.0f);
        modelMatrixForContainer9 = glm::translate(model, glm::vec3(12.0f, -2.92f, 1.40f));
        modelMatrixForContainer9 = glm::scale(modelMatrixForContainer9, glm::vec3(-10.0f, -0.30f, -6.5f));
        cube9.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer9);

        //second celling
        

        glm::mat4 modelMatrixForContainer10 = glm::mat4(1.0f);
        modelMatrixForContainer10 = glm::translate(model, glm::vec3(12.0f, 1.2f, 1.40f));
        modelMatrixForContainer10 = glm::scale(modelMatrixForContainer10, glm::vec3(-10.0f, -0.30f, -6.5f));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer10);

        //room 2 first wall
       

        glm::mat4 modelMatrixForContainer11 = glm::mat4(1.0f);
        modelMatrixForContainer11 = glm::translate(model, glm::vec3(2.0f, 1.2f, -5.0f));
        modelMatrixForContainer11 = glm::scale(modelMatrixForContainer11, glm::vec3(4.5f, -4.45f, -0.3f));
        cube11.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer11);

        //room2 1st wall door top

       

        glm::mat4 modelMatrixForContainer12 = glm::mat4(1.0f);
        modelMatrixForContainer12 = glm::translate(model, glm::vec3(6.5f, 1.2f, -5.0f));
        modelMatrixForContainer12 = glm::scale(modelMatrixForContainer12, glm::vec3(1.70f, -1.45f, -0.3f));
        cube12.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer12);

        //room 2 first wall last part
     

        glm::mat4 modelMatrixForContainer13 = glm::mat4(1.0f);
        modelMatrixForContainer13 = glm::translate(model, glm::vec3(8.2f, 1.2f, -5.0f));
        modelMatrixForContainer13 = glm::scale(modelMatrixForContainer13, glm::vec3(3.80f, -4.45f, -0.3f));
        cube13.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer13);

        //room 2 side 1
       

        glm::mat4 modelMatrixForContainer14 = glm::mat4(1.0f);
        modelMatrixForContainer14 = glm::translate(model, glm::vec3(8.9f, 1.2f, 1.4f));
        modelMatrixForContainer14 = glm::scale(modelMatrixForContainer14, glm::vec3(0.5f, -4.45f, -6.5f));
        cube14.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer14);

        //room 2 second wall
        

        glm::mat4 modelMatrixForContainer15 = glm::mat4(1.0f);
        modelMatrixForContainer15 = glm::translate(model, glm::vec3(2.0f, 1.2f, 1.7f));
        modelMatrixForContainer15 = glm::scale(modelMatrixForContainer15, glm::vec3(10.0f, -4.45f, -0.3f));
        cube15.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer15);



        //                       THIRD ROOM                                                           THIRD ROOM

        //room 3 floor
        
       

        glm::mat4 modelMatrixForContainer16 = glm::mat4(1.0f);
        modelMatrixForContainer16 = glm::translate(model, glm::vec3(12.0f, -2.92f, -5.0f));
        modelMatrixForContainer16 = glm::scale(modelMatrixForContainer16, glm::vec3(-16.0f, -0.30f, -6.5f));
        cube16.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer16);

        //room 3 celling 

        

        glm::mat4 modelMatrixForContainer17 = glm::mat4(1.0f);
        modelMatrixForContainer17 = glm::translate(model, glm::vec3(12.0f, 1.2f, -5.0f));
        modelMatrixForContainer17 = glm::scale(modelMatrixForContainer17, glm::vec3(-16.0f, -0.30f, -6.5f));
        cube17.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer17);

        //room 3 sidewall 1

       

        glm::mat4 modelMatrixForContainer18 = glm::mat4(1.0f);
        modelMatrixForContainer18 = glm::translate(model, glm::vec3(12.0f, 1.2f, -5.0f));
        modelMatrixForContainer18 = glm::scale(modelMatrixForContainer18, glm::vec3(0.5f, -4.45f, -6.5f));
        cube18.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer18);

        //room 3 window wall first, mane ghore dhukte age choke jeta pore
        

        glm::mat4 modelMatrixForContainer19 = glm::mat4(1.0f);
        modelMatrixForContainer19 = glm::translate(model, glm::vec3(12.0f, 1.2f, -11.5f));
        modelMatrixForContainer19 = glm::scale(modelMatrixForContainer19, glm::vec3(-8.6f, -4.45f, -0.3f));
        cube19.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer19);

        //room 3 window top
       

        glm::mat4 modelMatrixForContainer20 = glm::mat4(1.0f);
        modelMatrixForContainer20 = glm::translate(model, glm::vec3(3.5f, 1.2f, -11.5f));
        modelMatrixForContainer20 = glm::scale(modelMatrixForContainer20, glm::vec3(-1.6f, -1.45f, -0.3f));
        cube20.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer20);

        //room 3 window bottom
       

        glm::mat4 modelMatrixForContainer21 = glm::mat4(1.0f);
        modelMatrixForContainer21 = glm::translate(model, glm::vec3(3.5f, -1.3f, -11.5f));
        modelMatrixForContainer21 = glm::scale(modelMatrixForContainer21, glm::vec3(-1.6f, -1.65f, -0.3f));
        cube21.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer21);

        //room 3 second wall
        

        glm::mat4 modelMatrixForContainer22 = glm::mat4(1.0f);
        modelMatrixForContainer22 = glm::translate(model, glm::vec3(1.9f, 1.2f, -11.5f));
        modelMatrixForContainer22 = glm::scale(modelMatrixForContainer22, glm::vec3(-5.9f, -4.45f, -0.3f));
        cube22.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer22);

        //room 3 sidewall 2

        

        glm::mat4 modelMatrixForContainer23 = glm::mat4(1.0f);
        modelMatrixForContainer23 = glm::translate(model, glm::vec3(-4.1f, 1.2f, -5.1f));
        modelMatrixForContainer23 = glm::scale(modelMatrixForContainer23, glm::vec3(0.5f, -4.45f, -4.60f));
        cube23.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer23);

        //room 3 door top

        glm::mat4 modelMatrixForContainer24 = glm::mat4(1.0f);
        modelMatrixForContainer24 = glm::translate(model, glm::vec3(-4.1f, 1.2f, -9.6f));
        modelMatrixForContainer24 = glm::scale(modelMatrixForContainer24, glm::vec3(0.5f, -1.45f, -2.0f));
        cube24.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer24);



        //          fourth room                         FOURTH ROOM 



        //room 4 floor

        glm::mat4 modelMatrixForContainer25 = glm::mat4(1.0f);
        modelMatrixForContainer25 = glm::translate(model, glm::vec3(8.0f, -2.92f, -5.0f));
        modelMatrixForContainer25 = glm::scale(modelMatrixForContainer25, glm::vec3(-2.6f, -0.30f, -6.5f));
        cube25.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer25);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        for (unsigned int i = 0; i <= 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(ourShader, model, 0.8f, 0.8f, 0.8f);
        }

        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

   
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1f;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1f;
        else rotateAngle_Z -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001f;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1f;
        rotateAxis_X = 1.0f;
        rotateAxis_Y = 0.0f;
        rotateAxis_Z = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1f;
        rotateAxis_X = 0.0f;
        rotateAxis_Y = 1.0f;
        rotateAxis_Z = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1f;
        rotateAxis_X = 0.0f;
        rotateAxis_Y = 0.0f;
        rotateAxis_Z = 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            
            pointLightOn = !pointLightOn;
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            
            pointlight2.turnOff();
           
            pointLightOn = !pointLightOn;
        }
        else
        {
            
            pointlight2.turnOn();
           
            pointLightOn = !pointLightOn;
        }
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            
            pointlight3.turnOff();
           
            pointLightOn = !pointLightOn;
        }
        else
        {
           
            pointlight3.turnOn();
           
            pointLightOn = !pointLightOn;
        }
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
           
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
           
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {

            pointlight5.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {

            pointlight5.turnOn();
            pointLightOn = !pointLightOn;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
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
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
