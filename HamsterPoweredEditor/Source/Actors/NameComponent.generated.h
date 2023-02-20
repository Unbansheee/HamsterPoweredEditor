#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	m_name = j["m_name"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "NameComponent"; \
	j["m_name"] = m_name; \
} 
