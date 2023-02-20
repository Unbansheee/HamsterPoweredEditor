#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Collider::Deserialize(j); \
	m_Size = j["m_Size"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Collider::Serialize(j); \
	j["Type"] = "BoxCollider"; \
	j["m_Size"] = m_Size; \
} 
