#include "Camera.h"

Camera :: Camera(){
}
//used for reseting camera to initial position
void Camera :: setToInitialPosition(int textureWidth, int textureHeight){
    this->position = glm::vec3(
            (float) (textureWidth/2.0),
            (float) (textureWidth/10.0),
            (float) (-textureWidth/4.0)
        );
    this->light_position = glm::vec3(
            (float) (textureWidth/2.0),
            (float) (100.0f),
            (float) (textureHeight/2.0)
        );
    //mainly used for initialization
    up_vector = glm::vec3(0.0, 1.0, 0.0);
    gaze_vector = glm::vec3(0.0, 0.0, 1.0);
    crossProduct = cross(up_vector, gaze_vector);
    //resets to initial height and offset
    heightFactor = 10.0f;
    speed = 0.0f;
    camera_offset = 0.0f;
    glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), this->heightFactor);
    glUniform1f(glGetUniformLocation(idProgramShader, "cameraOffset"), this->camera_offset);
}

//helper updates position
void Camera :: updatePosition(){
    this->position += this->gaze_vector * this->speed;
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
    this->crossProduct = glm::rotate(this->crossProduct, YAW_CHANGE, this->up_vector);
    this->gaze_vector = glm::rotate(this->gaze_vector, YAW_CHANGE, this->up_vector);
}

void Camera :: increaseYaw(){ // Right -- D
    this->crossProduct = glm::rotate(this->crossProduct, -YAW_CHANGE, this->up_vector);
    this->gaze_vector = glm::rotate(this->gaze_vector, -YAW_CHANGE, this->up_vector);
}

void Camera :: decreasePitch(){
    this->up_vector = glm::rotate(this->up_vector, PITCH_CHANGE, this->crossProduct);
    this->gaze_vector = glm::rotate(this->gaze_vector, PITCH_CHANGE, this->crossProduct);
}

void Camera :: increasePitch(){
    this->up_vector = glm::rotate(this->up_vector, -PITCH_CHANGE, this->crossProduct);
    this->gaze_vector = glm::rotate(this->gaze_vector, -PITCH_CHANGE, this->crossProduct);
}

void Camera :: increaseLight_X(){
    this->light_position.x += 5;
}

void Camera :: increaseLight_Y(){
    this->light_position.y += LIGHT_FACTOR;
}

void Camera :: increaseLight_Z(){
    this->light_position.z += LIGHT_FACTOR;
}

void Camera :: decreaseLight_X(){
    this->light_position.x -= LIGHT_FACTOR;
}

void Camera :: decreaseLight_Y(){
    this->light_position.y -= LIGHT_FACTOR;
}

void Camera :: decreaseLight_Z(){
    this->light_position.z -= LIGHT_FACTOR;
}

void Camera :: decreaseOffset(){
    this->camera_offset -= 1.0f/texture_Width;
    glUniform1f(glGetUniformLocation(idProgramShader, "cameraOffset"), this->camera_offset);
}
void Camera :: increaseOffset(){
    this->camera_offset += 1.0f/texture_Width;
    glUniform1f(glGetUniformLocation(idProgramShader, "cameraOffset"), this->camera_offset);
}


void Camera::setTextureUniforms(){

    //self explanatory

    glm::mat4 M_projection = glm::perspective(45.0f, this->aspect_ratio, this->nearPlane, this->farPlane);

    glm::vec3 center = this->position + this->gaze_vector * this->nearPlane;

    glm::mat4 viewMatrix = glm::lookAt(this->position, center, this->up_vector);

    glm::mat4 modelViewProjection = M_projection * viewMatrix ;

    glm::mat4 normalMatrix = glm::inverseTranspose(viewMatrix);

    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MV"), 1, GL_FALSE, &viewMatrix[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MVP"), 1, GL_FALSE, &modelViewProjection[0][0]);

    glUniform3fv(glGetUniformLocation(idProgramShader, "cameraPosition"), 1, &this->position[0]);

    glUniform3fv(glGetUniformLocation(idProgramShader, "lightPosition"), 1, &this->light_position[0]);
}


