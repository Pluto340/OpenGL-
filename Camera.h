#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"iostream"
#include"GL/glew.h"
#include"GLFW/glfw3.h"
using namespace std;

class Camera
{
public:
	float m_lastX, m_lastY, m_pitch, m_yaw;
	float m_fov, m_deltatime, m_lastframe;
	glm::vec3 CameraPos, CameraFront, CameraUp;
	bool firstmouse;

	Camera(float lastX = 400, float lastY = 300, float pitch = -90.0f, float yaw = 0.0f, float fov = 45.0f);
	void mouse_callback(GLFWwindow *window,double xpos,double ypos);
	void scroll_callback(GLFWwindow *window,double xoffset,double yoffset);
	void key_callback(GLFWwindow *window);
};