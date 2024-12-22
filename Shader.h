#pragma once
#include"GL/glew.h"
#include"GLFW/glfw3.h"
#include<iostream>
using namespace std;
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include<fstream>
#include<sstream>
#include"glm/gtc/type_ptr.hpp"
struct ShaderSource
{
    string vertexshader;
    string fragmentshader;
    string geometryshader;
};

class Shader
{
private:
    unsigned int m_RendId;
    string filepath;
public:
    Shader() = default;
    Shader(const string& path);
    ~Shader();
    void Bind()const;
    void UnBind()const;
    unsigned int CompileShader(unsigned int type, const string& shader);
    void CreateProgram(const string& vertexment, const string& fragment,const string& geometry="");
    ShaderSource ShaderGetSource(const string& path);
    inline unsigned int GetId()const {
        return m_RendId;
    }
     void SetShader(const string &path)
    {
         filepath = path;
         m_RendId = 0;
        ShaderSource source = ShaderGetSource(path);
        CreateProgram(source.vertexshader, source.fragmentshader,source.geometryshader);
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_RendId, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_RendId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_RendId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_RendId, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_RendId, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_RendId, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_RendId, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_RendId, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(m_RendId, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_RendId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_RendId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_RendId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
