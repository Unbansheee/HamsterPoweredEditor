#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	ProceduralMeshComponent::Deserialize(j); \
	width = j["width"]; \
	height = j["height"]; \
	restLength = j["restLength"]; \
	stiffness = j["stiffness"]; \
	damping = j["damping"]; \
	iterations = j["iterations"]; \
	collisionEnabled = j["collisionEnabled"]; \
	windTurbulenceStrength = j["windTurbulenceStrength"]; \
	windFrequency = j["windFrequency"]; \
	m_gravity = j["m_gravity"]; \
	m_wind = j["m_wind"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	ProceduralMeshComponent::Serialize(j); \
	j["Type"] = "ClothComponent"; \
	j["width"] = width; \
	j["height"] = height; \
	j["restLength"] = restLength; \
	j["stiffness"] = stiffness; \
	j["damping"] = damping; \
	j["iterations"] = iterations; \
	j["collisionEnabled"] = collisionEnabled; \
	j["windTurbulenceStrength"] = windTurbulenceStrength; \
	j["windFrequency"] = windFrequency; \
	j["m_gravity"] = m_gravity; \
	j["m_wind"] = m_wind; \
} 

