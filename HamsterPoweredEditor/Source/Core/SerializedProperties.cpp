#include "SerializedProperties.h"

#include <fstream>
#include <iostream>
#include <json.hpp>

std::vector<SerializedProperty> SerializedProperties::GetProperties(const std::string& className)
{
    std::vector<SerializedProperty> properties;

    std::string metaPath = "Meta/" + className + ".json";
    std::ifstream metaFile(metaPath);

    nlohmann::json metaJson;
    metaFile >> metaJson;

    for (auto& property : metaJson)
    {
        properties.emplace_back(SerializedProperty(property["Name"], property["Type"], property["Value"]));
    }

    return properties;
}
