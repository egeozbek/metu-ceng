#include "helper.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"

static GLFWwindow * window = NULL;

/*
 * Before reading through main and camera class, 
 * read the shader.vert , it explains the assignment in depth
 */

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idJpegHeightMap;
GLuint idMVPMatrix;

int triangle_Count;
int vertex_Count;
glm::vec3* vertices;

//externed globals
int texture_Width, texture_Height;
int heightMap_Width, heightMap_Height;

Camera *camera = new Camera();

void prepareVertices(){

    //as an advice , don't bother with the index array , it only causes problems

    triangle_Count = 2 * texture_Width * texture_Height;
    vertex_Count = triangle_Count * 3 ;
    vertices = new glm::vec3[vertex_Count];

    glm::vec3 vertex0, vertex1, vertex2, vertex3;
    unsigned long int location = 0;

    for(int i = 0; i < texture_Width; i++){
        for(int j = 0; j < texture_Height; j++){

            //  0 --- 1
            //  |     |
            //  2 --- 3
            vertex0 = glm::vec3(i, 0, j);
            vertex1 = glm::vec3(i+1, 0, j);
            vertex2 = glm::vec3(i, 0, j+1);
            vertex3 = glm::vec3(i+1, 0, j+1);

            vertices[location++] = vertex0;
            vertices[location++] = vertex2;
            vertices[location++] = vertex1;

            vertices[location++] = vertex2;
            vertices[location++] = vertex3,
            vertices[location++] = vertex1;

        }
    }
}

void set_GPU_Variables(){

    glUniform1i(glGetUniformLocation(idProgramShader, "grayScaleTexture"), 0);

    glUniform1i(glGetUniformLocation(idProgramShader, "rgbTexture"), 1);

    glUniform1i(glGetUniformLocation(idProgramShader, "texture_Width"), texture_Width);

    glUniform1i(glGetUniformLocation(idProgramShader, "texture_Height"), texture_Height);

    glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), camera->heightFactor);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){

    bool isFullScreen = false;

    if(action == GLFW_PRESS){
        switch(key) {
            case GLFW_KEY_I: {
                camera->setToInitialPosition(texture_Width, texture_Height);
                break;
            }
            case GLFW_KEY_P: {
                isFullScreen = true;
                break;
            }
            case GLFW_KEY_ESCAPE:
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            }
        }
    }

    if(action == GLFW_PRESS || action == GLFW_REPEAT){
        switch(key) {
            case GLFW_KEY_A: {
                camera->decreaseYaw();
                break;
            }
            case GLFW_KEY_S: {
                camera->decreasePitch();
                break;
            }
            case GLFW_KEY_D: {
                camera->increaseYaw();
                break;
            }
            case GLFW_KEY_W: {
                camera->increasePitch();
                break;
            }
            case GLFW_KEY_Y: {
                camera->increaseSpeed();
                break;
            }
            case GLFW_KEY_H: {
                camera->decreaseSpeed();
                break;

            }
            case GLFW_KEY_X: {
                camera->setSpeedZero();
                break;

            }
            case GLFW_KEY_R:
            {
                camera->increaseHeightFactor();
                break;
            }
            case GLFW_KEY_F:
            {
                camera->decreaseHeightFactor();
                break;
            }
            case GLFW_KEY_Q:
            {
                camera->decreaseOffset();
                break;
            }
            case GLFW_KEY_E:
            {
                camera->increaseOffset();
                break;
            }
            case GLFW_KEY_T:
            {
                camera->increaseLight_Y();
                break;
            }
            case GLFW_KEY_G:
            {
                camera->decreaseLight_Y();
                break;
            }
            case GLFW_KEY_DOWN:
            {
                camera->decreaseLight_Z();
                break;
            }
            case GLFW_KEY_UP:
            {
                camera->increaseLight_Z();
                break;
            }
            case GLFW_KEY_RIGHT:
            {
                camera->decreaseLight_X();
                break;
            }
            case GLFW_KEY_LEFT:
            {
                camera->increaseLight_X();
                break;
            }
        }
    }

    if(isFullScreen){

        isFullScreen = false;

        if(camera->fullScreen){
            camera->fullScreen = !camera->fullScreen;

            camera->currHeight = camera->displayHeight;
            camera->currWidth = camera->displayWidth;

            glfwSetWindowMonitor(window, nullptr, 0, 0, camera->currWidth, camera->currHeight, 0);
        }
        else{
            camera->fullScreen = !camera->fullScreen;

            camera->currHeight = camera->videoMode->height;
            camera->currWidth = camera->videoMode->width;

            glfwSetWindowMonitor(window, camera->primaryMonitor,0,0,camera->currWidth, camera->currHeight, camera->videoMode->refreshRate);
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char * argv[]) {

    if (argc != 3) {
        std::cout << "There should be 2 images." << std::endl;
        return -1;
    }

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(1000, 1000, "Homework3 - Flat", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetErrorCallback(error_callback);

    std::cout <<  glGetString(GL_VERSION) << std::endl;

    glewInit();

    createShaders("hw3_flat_source/shader.vert","hw3_flat_source/shader.frag");
    glUseProgram(idProgramShader);
    initTexture(argv[1], &heightMap_Width, &heightMap_Height, 0);
    initTexture(argv[2], &texture_Width, &texture_Height, 1);

    glfwSetKeyCallback(window, key_callback); // HANDLE KEY PRESS
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // HANDLE RESIZE

    camera->setTextureUniforms();
    camera->setToInitialPosition(texture_Width, texture_Height);

    camera->primaryMonitor = glfwGetPrimaryMonitor();
    camera->videoMode = glfwGetVideoMode(camera->primaryMonitor);

    glViewport(0,0, camera->displayWidth, camera->displayHeight);

    prepareVertices();
    glEnable(GL_DEPTH_TEST);
    set_GPU_Variables();

    while (!glfwWindowShouldClose(window)) {
        /* Window resize callback */
        glfwGetWindowSize(window, &camera->currWidth, &camera->currHeight);

        /* CLEAR */
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        /* UPDATE CAMERA */
        camera->updatePosition();
        camera->setTextureUniforms();

        /* DRAW */
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLES, 0, vertex_Count);
        glDisableClientState(GL_VERTEX_ARRAY);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(idProgramShader);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
