#define GENERATED() \
public: \
void Deserialize(nlohmann::json& j) override{ \
} \
void Serialize(nlohmann::json& j) override{ \
	j["Type"] = "Component"; \
} 
