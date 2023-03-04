#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	m_MeshPath = j["m_MeshPath"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "MeshComponent"; \
	j["m_MeshPath"] = m_MeshPath; \
} 

