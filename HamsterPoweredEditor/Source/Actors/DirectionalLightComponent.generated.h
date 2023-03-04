#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	m_Light = j["m_Light"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "DirectionalLightComponent"; \
	j["m_Light"] = m_Light; \
} 

