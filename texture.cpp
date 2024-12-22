#include "texture.h"

Texture::Texture(const string& path):
	m_width(0),m_height(0),m_localbuffer(NULL),m_BPP(0),m_filepath(path),
	m_RendID(0)
{
	stbi_set_flip_vertically_on_load(0);
	m_localbuffer = stbi_load(path.c_str(),&m_width,&m_height,&m_BPP,4);
	glGenTextures(1,&m_RendID);
	glBindTexture(GL_TEXTURE_2D,m_RendID);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	if (m_localbuffer)
	{
		cout << "纹理加载成功！" << endl;
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_width,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_localbuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(m_localbuffer);
	}
	else
	{
		cout << "纹理加载失败" << endl;
		cout << "path：" << path << " reason：" << stbi_failure_reason() << endl;
	}
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(slot+GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_RendID);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D,0);
}

Texture::~Texture()
{
	glDeleteTextures(1,&m_RendID);
}
