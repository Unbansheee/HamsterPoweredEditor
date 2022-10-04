#include "Shader.h"
#include <fstream>
#include <iostream>
#include <glad/glad.h>

#include "glm/glm.hpp"


Shader::Shader(const std::string& vertex, const std::string& fragment) : vertexPath(vertex), fragmentPath(fragment), rendererID(0)
{
    ShaderProgramSource source = CreateShaderSource(vertexPath, fragmentPath);
    rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(rendererID);
}

void Shader::Bind() const
{
    glUseProgram(rendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}


void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    glUniform2f(GetUniformLocation(name), v0, v1);    
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
        return uniformLocationCache[name];
    }
    
    int location = glGetUniformLocation(rendererID, name.c_str());
    if(location == -1)
    {
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }
    uniformLocationCache[name] = location;
    return location;
    
}


unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
    const char* vsrc = vertexSource.c_str();
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vshader, 1, &vsrc, NULL);
    glCompileShader(vshader);

    {
        int  success;
        char infoLog[512];
        glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vshader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    const char* fsrc = fragmentSource.c_str();
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fshader, 1, &fsrc, NULL);
    glCompileShader(fshader);

    {
        int  success;
        char infoLog[512];
        glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fshader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    rendererID = glCreateProgram();
    glAttachShader(rendererID, vshader);
    glAttachShader(rendererID, fshader);
    glLinkProgram(rendererID);

    {
        int  success;
        char infoLog[512];
        glGetShaderiv(rendererID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vshader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINK::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
    
    glDetachShader(rendererID, vshader);
    glDetachShader(rendererID, fshader);
    
    return rendererID;
}

ShaderProgramSource Shader::CreateShaderSource(const std::string& vertexPath, const std::string& fragmentPath)
{
    ShaderProgramSource source;
    std::ifstream vin(vertexPath);
    if (!vin)
    {
        std::cout << "Vertex shader file not found: " << vertexPath << std::endl;
    }
    source.VertexSource = std::string((std::istreambuf_iterator<char>(vin)), std::istreambuf_iterator<char>());

    std::ifstream fin(fragmentPath);
    if (!fin)
    {
        std::cout << "Fragment shader file not found: " << fragmentPath << std::endl;
    }
    source.FragmentSource = std::string((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    return source;
}