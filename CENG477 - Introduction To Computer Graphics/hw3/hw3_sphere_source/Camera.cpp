#include "Camera.h"

Camera :: Camera(){
}

void Camera :: setToInitialPosition(int textureWidth, int textureHeight){
    this->position = glm::vec3(
            (float) (0.0f),
            (float) (600.0f),
            (float) (0.0f)
        );
    this->light_position = glm::vec3(
            (float) (0.0f),
            (float) (1600.0f),
            (float) (0.0f)
        );
    upVector = glm::vec3(0.0f, 0.0f, 1.0f);
    gazeVector = glm::vec3(0.0f, -1.0f, 0.0f);
    crossVector = cross(upVector, gazeVector);
    speed = 0.0f;
    camera_offset = 0.0f;
    heightFactor = 0.004f;
    glUniform1f(glGetUniformLocation(idProgramShader, "cameraOffset"), this->camera_offset);
    glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), this->heightFactor);
}

void Camera :: updatePosition(){
    this->position += this->gazeVector * this->speed;
}

void Camera :: increaseSpeed(){
    this->speed += SPEED_CHANGE;
}

void Camera :: decreaseSpeed(){
    this->speed -= SPEED_CHANGE;
}

void Camera :: setSpeedZero(){
    this->speed = 0.0f;
}

void Camera :: increaseHeightFactor(){
    this->heightFactor += HEIGHT_FACTOR_CHANGE;
    glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), this->heightFactor);
}

void Camera :: decreaseHeightFactor(){
    this->heightFactor -= HEIGHT_FACTOR_CHANGE;
    glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), this->heightFactor);
}

void Camera :: decreaseYaw(){ // Left -- A
    this->crossVector = glm::rotate(this->crossVector, YAW_CHANGE, this->upVector);
    this->gazeVector = glm::rotate(this->gazeVector, YAW_CHANGE, this->upVector);
}

void Camera :: increaseYaw(){ // Right -- D
    this->crossVector = glm::rotate(this->crossVector, -YAW_CHANGE, this->upVector);
    this->gazeVector = glm::rotate(this->gazeVector, -YAW_CHANGE, this->upVector);
}

void Camera :: decreasePitch(){
    this->upVector = glm::rotate(this->upVector, PITCH_CHANGE, this->crossVector);
    this->gazeVector = glm::rotate(this->gazeVector, PITCH_CHANGE, this->crossVector);
}

void Camera :: increasePitch(){
    this->upVector = glm::rotate(this->upVector, -PITCH_CHANGE, this->crossVector);
    this->gazeVector = glm::rotate(this->gazeVector, -PITCH_CHANGE, this->crossVector);
}

void Camera :: increaseLight_X(){
    this->light_position.x += 5;
}

void Camera :: increaseLight_Y(){
    this->light_position.y += 5;
}

void Camera :: increaseLight_Z(){
    this->light_position.z += 5;
}

void Camera :: decreaseLight_X(){
    this->light_position.x -= 5;
}

void Camera :: decreaseLight_Y(){
    this->light_position.y -= 5;
}

void Camera :: decreaseLight_Z(){
    this->light_position.z -= 5;
}

void Camera :: decreaseOffset(){
    this->camera_offset -= 1.f/this->horizontalStepCount;
    glUniform1f(glGetUniformLocation(idProgramShader, "cameraOffset"), this->camera_offset);
}
void Camera :: increaseOffset(){
    this->camera_offset += 1.f/this->horizontalStepCount;
    glUniform1f(glGetUniformLocation(idProgramShader, "cameraOffset"), this->camera_offset);
}


void Camera::setTextureUniforms(){

    glm::mat4 M_projection = glm::perspective(45.0f, this->aspect_ratio, this->nearPlane, this->farPlane);

    glm::vec3 center = this->position + this->gazeVector * this->nearPlane;

    glm::mat4 viewMatrix = glm::lookAt(this->position, center, this->upVector);

    glm::mat4 modelViewProjection = M_projection * viewMatrix ;

    glm::mat4 normalMatrix = glm::inverseTranspose(viewMatrix);

    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MV"), 1, GL_FALSE, &viewMatrix[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MVP"), 1, GL_FALSE, &modelViewProjection[0][0]);

    glUniform3fv(glGetUniformLocation(idProgramShader, "cameraPosition"), 1, &this->position[0]);

    glUniform3fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &this->light_position[0]);
}


