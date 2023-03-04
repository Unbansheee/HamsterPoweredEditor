#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	m_TexturePath = j["m_TexturePath"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "SpriteComponent"; \
	j["m_TexturePath"] = m_TexturePath; \
} 

