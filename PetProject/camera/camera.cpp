#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 _position, glm::vec3 up , float _yaw, float _pitch) 
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	movementSpeed(SPEED), 
	mouseSensitivity(SENSITIVITY), 
	zoom(ZOOM)
{
	position = _position;
	worldUp = up;
	yaw = _yaw;
	pitch = _pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw, float _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	yaw = _yaw;
	pitch = _pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD)
	{
		position += front * velocity;
	}
	if (direction == CameraMovement::BACKWARD)
	{
		position -= front * velocity;
	}
	if (direction == CameraMovement::RIGHT)
	{
		position += right * velocity;
	}
	if (direction == CameraMovement::LEFT)
	{
		position -= right * velocity;
	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;
	
	yaw += xOffset;
	pitch += yOffset;

	if (constraintPitch)
	{
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	zoom -= float(yoffset);
	if (zoom < 1.0f)
	{
		zoom = 1.0f;
	}
	if (zoom > 45.0f)
	{
		zoom = 45.0f;
	}
}

void Camera::updateCameraVectors()
{
	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);
		
	right = glm::normalize(glm::cross(front, worldUp));
	up	  = glm::normalize(glm::cross(right, front));
}
