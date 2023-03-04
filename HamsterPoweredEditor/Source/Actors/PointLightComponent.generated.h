#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	m_PointLight = j["m_PointLight"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "PointLightComponent"; \
	j["m_PointLight"] = m_PointLight; \
} 

