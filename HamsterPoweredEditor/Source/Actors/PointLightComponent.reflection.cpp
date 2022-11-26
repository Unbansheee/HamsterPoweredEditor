#pragma once
#include "PointLightComponent.h"
#include "Core/JSONConversions.h"


void PointLightComponent::Deserialize(nlohmann::json& j){
	__super::Deserialize(j);
	m_PointLight = j["m_PointLight"];
}


void PointLightComponent::Serialize(nlohmann::json& j){
	__super::Serialize(j);
	j["Type"] = "PointLightComponent";
	j["m_PointLight"] = m_PointLight;
}
