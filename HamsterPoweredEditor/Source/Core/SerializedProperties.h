#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


struct SerializedProperty
{
    SerializedProperty(std::string name, std::string type, std::string value) : Name(name), Type(type), Value(value) {}
    
    std::string Name;
    std::string Type;
    std::string Value;

    
};

class SerializedProperties
{
public:
    static std::vector<SerializedProperty> GetProperties(const std::string& className);
    
};
