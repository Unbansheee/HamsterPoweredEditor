#include "json.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Rendering/Lights.h"
#include "Rendering/StaticMesh.h"
#include "Rendering/DynamicMesh.h"


namespace nlohmann
{
    template <>
    struct adl_serializer<glm::vec3>
    {
        static void to_json(json& j, const glm::vec3& vec)
        {
            j = json{ vec.x, vec.y, vec.z };
        }

        static void from_json(const json& j, glm::vec3& vec)
        {
            vec.x = j[0];
            vec.y = j[1];
            vec.z = j[2];
        }
    };

    template <>
    struct adl_serializer<glm::vec4>
    {
        static void to_json(json& j, const glm::vec4& vec)
        {
            j = json{ vec.x, vec.y, vec.z, vec.w };
        }

        static void from_json(const json& j, glm::vec4& vec)
        {
            vec.x = j[0];
            vec.y = j[1];
            vec.z = j[2];
            vec.w = j[3];
        }
    };

    template <>
    struct adl_serializer<glm::vec2>
    {
        static void to_json(json& j, const glm::vec2& vec)
        {
            j = json{ vec.x, vec.y };
        }

        static void from_json(const json& j, glm::vec2& vec)
        {
            vec.x = j[0];
            vec.y = j[1];
        }
    };

    template <>
    struct adl_serializer<PointLightData>
    {
        static void to_json(json& j, const PointLightData& light)
        {
            j["Color"] = light.color;
            j["Position"] = light.position;
            j["Intensity"] = light.intensity;
            j["Radius"] = light.radius;
        }

        static void from_json(const json& j, PointLightData& light)
        {
            light.color = j["Color"];
            light.position = j["Position"];
            light.intensity = j["Intensity"];
            light.radius = j["Radius"];
        }
    };

    template <>
    struct adl_serializer<DirectionalLightData>
    {
        static void to_json(json& j, const DirectionalLightData& light)
        {
            j = json{ light.direction, light.color, light.intensity };
        }

        static void from_json(const json& j, DirectionalLightData& light)
        {
            light.direction = j[0];
            light.color = j[1];
            light.intensity = j[2];
        }
    };

    template <>
    struct adl_serializer<StaticMesh>
    {
        static void to_json(json& j, const StaticMesh& mesh)
        {
            j["MeshPath"] = mesh.GetPath();
        }

        static void from_json(const json& j, StaticMesh& mesh)
        {
            mesh.Load(j["MeshPath"]);
        }
    };


    template <>
    struct adl_serializer<DynamicMesh>
    {
        static void to_json(json& j, const DynamicMesh& mesh)
        {
            j["MeshPath"] = mesh.GetPath();
        }

        static void from_json(const json& j, DynamicMesh& mesh)
        {
            mesh.Load(j["MeshPath"]);
        }
    };
    
    
}
