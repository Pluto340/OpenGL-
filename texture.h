#pragma once
#include"stb_image.h"
#include"GL/glew.h"
#include"iostream"
using namespace std;
class Texture
{
public:
	Texture(const string& path);
	void Bind(unsigned int slot=0)const;
	void UnBind()const;
	inline int GetWidth()const {
		return m_width;
	}
	inline int GetHeight()const {
		return m_height;
	}
	unsigned int GetId()const {
		return m_RendID;
	}
	~Texture();
private:
	unsigned int m_RendID;
	int m_width, m_height, m_BPP;
	string m_filepath;
	unsigned char* m_localbuffer;
};