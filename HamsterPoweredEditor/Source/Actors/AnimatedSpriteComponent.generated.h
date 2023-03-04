#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	SpriteComponent::Deserialize(j); \
	m_fps = j["m_fps"]; \
	m_rows = j["m_rows"]; \
	m_columns = j["m_columns"]; \
	m_spriteWidth = j["m_spriteWidth"]; \
	m_spriteHeight = j["m_spriteHeight"]; \
	m_currentFrame = j["m_currentFrame"]; \
	m_animationTime = j["m_animationTime"]; \
	m_frameCount = j["m_frameCount"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	SpriteComponent::Serialize(j); \
	j["Type"] = "AnimatedSpriteComponent"; \
	j["m_fps"] = m_fps; \
	j["m_rows"] = m_rows; \
	j["m_columns"] = m_columns; \
	j["m_spriteWidth"] = m_spriteWidth; \
	j["m_spriteHeight"] = m_spriteHeight; \
	j["m_currentFrame"] = m_currentFrame; \
	j["m_animationTime"] = m_animationTime; \
	j["m_frameCount"] = m_frameCount; \
} 

