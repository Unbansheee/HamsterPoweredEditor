﻿#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int rendererID;
    std::string vertexPath;
    std::string fragmentPath;
    std::unordered_map<std::string, int> uniformLocationCache;
    
public:
    Shader(const std::string& vertex, const std::string& fragment);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& value);
    
    int GetUniformLocation(const std::string& name);
    
private:

    unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
    ShaderProgramSource CreateShaderSource(const std::string& vertexPath, const std::string& fragmentPath);
};
