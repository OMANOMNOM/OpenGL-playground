#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Renderer.h"


Shader::Shader(const std::string& filepath)
	: m_filePath(filepath), m_rendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);

}
	
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}


// Common to just read in shaders as a string for complation sake. 
int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;

}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Do error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
	
void Shader::Bind() const 
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::Unbind() const 
{
    GLCall(glUseProgram(0));

}

	// Set uniform
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) 
{
    GLCall(glUniform4f(GetUniformLocaiton(name), v0, v1, v2, v3));

}

void Shader::SetUniform1f(const std::string& name, float v0)
{
    GLCall(glUniform1f(GetUniformLocaiton(name), v0));
}

unsigned int Shader::GetUniformLocaiton(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    else
        m_uniformLocationCache[name] = location;
    return location;
}