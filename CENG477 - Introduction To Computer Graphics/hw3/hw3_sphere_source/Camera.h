#ifndef HOMEWORK3_CAMERA_H
#define HOMEWORK3_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"


//amount of change constants

#define HEIGHT_FACTOR_CHANGE 0.5f
#define PITCH_CHANGE 0.05f
#define YAW_CHANGE 0.05f
#define SPEED_CHANGE 0.01f

//initial camera configuration

#define INITIAL_SPEED 0.0f
#define INITIAL_PITCH_ANGLE 90.0f
#define INITIAL_YAW_ANGLE 0.00f
#define INITIAL_HEIGHT_FACTOR 10.0f

#define PROJECTION_NEAR_LIMIT 0.01f
#define PROJECTION_FAR_LIMIT 1000.01f
#define PROJECTION_FOV 45.0f
#define PROJECTION_ASPECT_RATIO 1.0f

extern GLuint idProgramShader;

class Camera{

public :

    glm::vec3 position;
    glm::vec3 light_position;
    glm::vec3 upVector = glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 gazeVector = glm::vec3(0.0, -1.0, 0.0);
    glm::vec3 crossVector = cross(upVector, gazeVector);
    GLfloat speed = 0.0f;
    GLfloat camera_offset = 0.0f;

    int currWidth, currHeight;

    bool fullScreen;

    int displayWidth = 1000;
    int displayHeight = 1000;

    GLFWmonitor* primaryMonitor;
    const GLFWvidmode* videoMode;

    GLfloat aspect_ratio = 1;
    GLfloat nearPlane = 0.1;
    GLfloat farPlane = 1000;

    GLfloat heightFactor = 0.008f;
    float horizontalStepCount = 250.0f;


    Camera();

    void setToInitialPosition(int textureWidth, int textureHeight);

    void updatePosition();

    void increaseSpeed();

    void decreaseSpeed();

    void setSpeedZero();

    void increaseHeightFactor();

    void decreaseHeightFactor();

    void decreaseYaw();

    void increaseYaw();

    void decreasePitch();

    void increasePitch();

    void setTextureUniforms();

    void increaseLight_X();

    void increaseLight_Y();

    void increaseLight_Z();

    void decreaseLight_X();

    void decreaseLight_Y();

    void decreaseLight_Z();

    void decreaseOffset();

    void increaseOffset();

};


#endif //HOMEWORK3_CAMERA_H
