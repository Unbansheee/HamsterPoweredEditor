#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	m_Active = j["m_Active"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "Collider"; \
	j["m_Active"] = m_Active; \
} 

