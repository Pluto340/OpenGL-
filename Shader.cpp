#include"shader.h"

Shader::Shader(const string& path)
    :m_RendId(0), filepath(path)
{
    ShaderSource source = ShaderGetSource(path);
    CreateProgram(source.vertexshader, source.fragmentshader,source.geometryshader);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendId);
}

void Shader::Bind() const
{
    glUseProgram(m_RendId);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

unsigned int Shader::CompileShader(unsigned int type, const string& shader)
{
    unsigned int Id = glCreateShader(type);
    const char* src = shader.c_str();
    glShaderSource(Id, 1, &src, NULL);
    glCompileShader(Id);
    int result;
    glGetShaderiv(Id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(Id, length, &length, message);
        cout << "Shader Compile Fail £º" << filepath << "   reason£º" << ((type == GL_VERTEX_SHADER) ? "vertexShader" : ((type==GL_FRAGMENT_SHADER)?"fragmentshader":"geometryshader"))<< "  " << message << endl;
        glDeleteShader(Id);
    }
    return Id;
}
void Shader::CreateProgram(const string& vertexment, const string& fragment,const string &geometry)
{
    unsigned int gs;
    m_RendId = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexment);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment);
    
    glAttachShader(m_RendId, vs);
    glAttachShader(m_RendId, fs);
    if (geometry != "")
    {
        gs = CompileShader(GL_GEOMETRY_SHADER, geometry);
        glAttachShader(m_RendId,gs);
    }

    glLinkProgram(m_RendId);
    glValidateProgram(m_RendId);
    glDeleteShader(vs);
    glDeleteShader(fs);
    if (geometry != "")
        glDeleteShader(gs);
}

ShaderSource Shader::ShaderGetSource(const string& path)
{
    ifstream ifs(path);
    if (!ifs)
    {
        cout << "Basic.shader is not open!" << endl;
    }
    enum class ShaderType
    {
        NONE = -1, VERTEXMENT = 0, FRAGMENT = 1, GEOMETRY = 2
    };
    ShaderType type = ShaderType::NONE;
    string line;
    stringstream ss[3];
    while (getline(ifs, line))
    {
        if (line.find("#shader") != string::npos)
        {
            if (line.find("vertex") != string::npos)
                type = ShaderType::VERTEXMENT;
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("geometry") != string::npos)
                type = ShaderType::GEOMETRY;
        }
        else
        {
            if (type == ShaderType::NONE)
            {
                cout << "Basic.shader content error" << endl;
                break;
            }
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str(),ss[2].str()};

}


