#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Player.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  -45.0f;
const GLfloat SPEED      =  6.0f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;

class Camera{
public:
    // Constructor with vectors
    Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ), GLfloat yaw = YAW, GLfloat pitch = PITCH ) : front( glm::vec3( 0.0f, 0.0f, -1.0f ) ), movementSpeed( SPEED ), mouseSensitivity( SENSITIVTY ), zoom( ZOOM )
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors( );
    }
    
    // Constructor with scalar values
    Camera( GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch ) : front( glm::vec3( 0.0f, 0.0f, -1.0f ) ), movementSpeed( SPEED ), mouseSensitivity( SENSITIVTY ), zoom( ZOOM ) {
        this->position = glm::vec3( posX, posY, posZ );
        this->worldUp = glm::vec3( upX, upY, upZ );
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors( );
    }
    
    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt( this->position, this->position + this->front, this->up );
    }
    
    void Move(GLfloat deltaTime){
//        GLfloat velocity = this->movementSpeed * deltaTime;
        this->position -= glm::vec3(0.0f, 0.0f, 0.005);//this->front * velocity;
    }
    
    void FollowObject(Player player){
        glm::vec3 p_position = player.GetPosition();
        
        this->position.x = p_position.x;
        this->position.y = p_position.y + 10.0f;
        this->position.z = p_position.z + 7.0f;
    }

    void ProcessKeyboard( Camera_Movement direction, GLfloat deltaTime ){
        GLfloat velocity = this->movementSpeed * deltaTime;
        
        if ( direction == FORWARD )
        {
            this->position -= glm::vec3(0.0f, 0.0f, 0.005f);//this->front * velocity;
        }
        
        if ( direction == BACKWARD )
        {
            this->position += glm::vec3(0.0f, 0.0f, 0.005f);//this->front * velocity;
        }
        
        if ( direction == LEFT )
        {
            this->position -= this->right * velocity;
        }
        
        if ( direction == RIGHT )
        {
            this->position += this->right * velocity;
        }
    }
    
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement( GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true )
    {
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;
        
        this->yaw   += xOffset;
        this->pitch += yOffset;
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if ( constrainPitch )
        {
            if ( this->pitch > 89.0f ){
                this->pitch = 89.0f;
            }
            
            if ( this->pitch < -89.0f ){
                this->pitch = -89.0f;
            }
        }
        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors( );
    }
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll( GLfloat yOffset ){
        
    }
    
    GLfloat GetZoom(){
        return this->zoom;
    }
    
    glm::vec3 GetPosition(){
        return this->position;
    }
    
    glm::vec3 GetFront(){
        return this->front;
    }
    
private:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;
    
    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
    
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors( )
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos( glm::radians( this->yaw ) ) * cos( glm::radians( this->pitch ) );
        front.y = sin( glm::radians( this->pitch ) );
        front.z = sin( glm::radians( this->yaw ) ) * cos( glm::radians( this->pitch ) );
        this->front = glm::normalize( front );
        // Also re-calculate the Right and Up vector
        this->right = glm::normalize( glm::cross( this->front, this->worldUp ) );  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->up = glm::normalize( glm::cross( this->right, this->front ) );
    }
};
