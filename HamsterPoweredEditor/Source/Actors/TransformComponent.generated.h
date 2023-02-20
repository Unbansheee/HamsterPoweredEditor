#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	LocalPosition = j["LocalPosition"]; \
	LocalRotation = j["LocalRotation"]; \
	LocalScale = j["LocalScale"]; \
	m_lockedScale = j["m_lockedScale"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "TransformComponent"; \
	j["LocalPosition"] = LocalPosition; \
	j["LocalRotation"] = LocalRotation; \
	j["LocalScale"] = LocalScale; \
	j["m_lockedScale"] = m_lockedScale; \
} 
