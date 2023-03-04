#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
	Component::Deserialize(j); \
	m_Text = j["m_Text"]; \
	m_FontSize = j["m_FontSize"]; \
	m_Color = j["m_Color"]; \
	m_HoverColor = j["m_HoverColor"]; \
	m_PressedColor = j["m_PressedColor"]; \
	m_BaseColor = j["m_BaseColor"]; \
	m_FontPath = j["m_FontPath"]; \
	m_ScreenSpace = j["m_ScreenSpace"]; \
	m_scaleBounce = j["m_scaleBounce"]; \
	m_scaleBounceDelta = j["m_scaleBounceDelta"]; \
	m_sideScroll = j["m_sideScroll"]; \
	m_scrollSpeed = j["m_scrollSpeed"]; \
} \
void Serialize(nlohmann::json& j) override{ \
	Component::Serialize(j); \
	j["Type"] = "TextLabelComponent"; \
	j["m_Text"] = m_Text; \
	j["m_FontSize"] = m_FontSize; \
	j["m_Color"] = m_Color; \
	j["m_HoverColor"] = m_HoverColor; \
	j["m_PressedColor"] = m_PressedColor; \
	j["m_BaseColor"] = m_BaseColor; \
	j["m_FontPath"] = m_FontPath; \
	j["m_ScreenSpace"] = m_ScreenSpace; \
	j["m_scaleBounce"] = m_scaleBounce; \
	j["m_scaleBounceDelta"] = m_scaleBounceDelta; \
	j["m_sideScroll"] = m_sideScroll; \
	j["m_scrollSpeed"] = m_scrollSpeed; \
} 

