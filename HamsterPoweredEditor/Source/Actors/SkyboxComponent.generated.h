#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	MeshComponent::Deserialize(j); \
	cubemapPath = j["cubemapPath"]; \
	brightness = j["brightness"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	MeshComponent::Serialize(j); \
	j["Type"] = "SkyboxComponent"; \
	j["cubemapPath"] = cubemapPath; \
	j["brightness"] = brightness; \
} 

