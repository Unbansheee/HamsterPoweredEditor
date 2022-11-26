#pragma once
#include "NameComponent.h"
#include "Core/JSONConversions.h"


void NameComponent::Deserialize(nlohmann::json& j){
	__super::Deserialize(j);
	m_name = j["m_name"];
}


void NameComponent::Serialize(nlohmann::json& j){
	__super::Serialize(j);
	j["Type"] = "NameComponent";
	j["m_name"] = m_name;
}
