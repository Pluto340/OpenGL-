#pragma once
#include"iostream"
using namespace std;
#include"GL/glew.h"
#include"GLFW/glfw3.h"
#include"Camera.h"

extern int WIDTH ;
extern int HEIGHT;
extern unsigned int frameibo;
extern unsigned int textureAttachment;
extern unsigned int rendibo;
extern Camera camera;

void ProcessInput(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void PressKey(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void SetFramebuffer();
