#include "camera.h"


Camera::Camera(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::cross(cameraViewDirection, cameraUp);
}

Camera::Camera()
{
	this->cameraPosition = glm::vec3(0.0f, 0.0f, 100.0f);
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::cross(cameraViewDirection, cameraUp);
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = cameraViewDirection;
	this->cameraUp = cameraUp;
	this->right = glm::cross(cameraViewDirection, cameraUp);
}

Camera::~Camera()
{
}

void Camera::rotateOx(float angle)
{
	//cameraViewDirection = glm::rotate(cameraViewDirection, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Camera::rotateOy (float angle)
{
	//TODO
}

void Camera::translateUp(float speed)
{
	float cameraSpeed = 100.5f * speed;
	cameraPosition = cameraPosition + cameraUp * cameraSpeed;
}

void Camera::translateDown(float speed)
{
	float cameraSpeed = 100.5f * speed;
	cameraPosition = cameraPosition - cameraUp * cameraSpeed;
}

void Camera::translateRight(float speed)
{
	float cameraSpeed = 100.5f * speed;
	cameraPosition = cameraPosition + right * cameraSpeed;
}

void Camera::translateLeft(float speed)
{
	float cameraSpeed = 100.5f * speed;
	cameraPosition = cameraPosition - right * cameraSpeed;
}

void Camera::translateFront(float speed)
{
	float cameraSpeed = 10.0f * speed;
	cameraPosition = cameraPosition + cameraViewDirection * cameraSpeed;
}

void Camera::translateBack(float speed)
{
	float cameraSpeed = 10.0f * speed;
	cameraPosition = cameraPosition - cameraViewDirection * cameraSpeed;
}


glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection()
{
	return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp()
{
	return cameraUp;
}

void Camera::setCameraPosition(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
}

void Camera::setCameraUp(glm::vec3 cameraUp)
{
	this->cameraUp = cameraUp;
}

void Camera::setCameraViewDirection(glm::vec3 cameraViewDirection)
{
	this->cameraViewDirection = cameraViewDirection;
}

