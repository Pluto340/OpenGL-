#include"framebuffer.h"
void ProcessInput(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
	glBindTexture(GL_TEXTURE_2D, textureAttachment);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,WIDTH,HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glBindRenderbuffer(GL_RENDERBUFFER,rendibo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendibo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "帧缓冲附件不完整！" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//恢复到默认的帧缓冲
}

void PressKey(GLFWwindow* window)
{
	camera.key_callback(window);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.scroll_callback(window, xoffset, yoffset);
}
void SetFramebuffer()
{
	//创建帧缓冲
	
	glGenFramebuffers(1, &frameibo);
	glBindFramebuffer(GL_FRAMEBUFFER, frameibo);

	
	glGenTextures(1, &textureAttachment);
	glBindTexture(GL_TEXTURE_2D, textureAttachment);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureAttachment, 0);

	
	glGenRenderbuffers(1, &rendibo);
	glBindRenderbuffer(GL_RENDERBUFFER, rendibo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendibo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "帧缓冲附件不完整！" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);//恢复到默认的帧缓冲
}