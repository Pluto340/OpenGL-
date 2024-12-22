#include "Camera.h"

Camera::Camera(float lastX, float lastY, float pitch, float yaw, float fov):
	CameraPos(0.0f,0.0f,3.0f),CameraFront(0.0f,0.0f,-2.0f),CameraUp(0.0f,1.0f,0.0f),
	m_deltatime(0),m_lastframe(0),firstmouse(true)
{
	m_lastX = lastX;
	m_lastY = lastY;
	m_pitch = pitch;
	m_yaw = yaw;
	m_fov = fov;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstmouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		firstmouse = false;
	}
	float xoffset = xpos - m_lastX;
	float yoffset = m_lastY-ypos;
	m_lastX = xpos;
	m_lastY = ypos;

	float sensitivity = 0.3f;
	m_pitch += yoffset * sensitivity;
	m_yaw += xoffset * sensitivity;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch))*cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	CameraFront = front;
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_pitch += yoffset;
	if (m_fov > 45.0f)
		m_fov = 45.0f;
	if (m_fov < 1.0f)
		m_fov = 1.0f;
}

void Camera::key_callback(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
		glfwSetWindowShouldClose(window,GL_TRUE);

	float currentframe = glfwGetTime();
	m_deltatime = currentframe - m_lastframe;
	m_lastframe = currentframe;
	float CameraSpeed = m_deltatime * 2.5;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		CameraPos -= CameraSpeed * glm::normalize(glm::cross(CameraFront,CameraUp));
	if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
		CameraPos+= CameraSpeed * glm::normalize(glm::cross(CameraFront, CameraUp));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		CameraPos += CameraSpeed * CameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		CameraPos -= CameraFront * CameraSpeed;

}
