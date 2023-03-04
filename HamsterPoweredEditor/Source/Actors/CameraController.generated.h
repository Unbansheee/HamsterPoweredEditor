#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	yaw = j["yaw"]; \
	pitch = j["pitch"]; \
	m_CameraType = j["m_CameraType"]; \
	dt = j["dt"]; \
	m_Zoom = j["m_Zoom"]; \
	m_OrthoZoom = j["m_OrthoZoom"]; \
	m_PerspMoveSpeed = j["m_PerspMoveSpeed"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "CameraController"; \
	j["yaw"] = yaw; \
	j["pitch"] = pitch; \
	j["m_CameraType"] = m_CameraType; \
	j["dt"] = dt; \
	j["m_Zoom"] = m_Zoom; \
	j["m_OrthoZoom"] = m_OrthoZoom; \
	j["m_PerspMoveSpeed"] = m_PerspMoveSpeed; \
} 

