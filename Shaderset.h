#pragma once
#include"iostream"
using namespace std;
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"Shader.h"
#include"string"

class Shaderset
{
public:
	Shaderset()
	{
		 objectshader.SetShader("objector.editorconfig");
		 Stencilshader.SetShader("Stencil.editorconfig");
		 transparentshader.SetShader("transparent.editorconfig");
		 oppsitetextureshader.SetShader("Opposite_Color_Texture.editorconfig");
		 Rendershader.SetShader("Render.editorconfig");
	}
private:
	Shader objectshader;
	Shader Stencilshader;
	Shader transparentshader;
	Shader oppsitetextureshader;
	Shader Rendershader;
};