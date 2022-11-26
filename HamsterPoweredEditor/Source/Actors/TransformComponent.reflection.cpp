#pragma once
#include "TransformComponent.h"
#include "Core/JSONConversions.h"


void TransformComponent::Deserialize(nlohmann::json& j){
	__super::Deserialize(j);
	LocalPosition = j["LocalPosition"];
	LocalRotation = j["LocalRotation"];
	LocalScale = j["LocalScale"];
	m_lockedScale = j["m_lockedScale"];
}


void TransformComponent::Serialize(nlohmann::json& j){
	__super::Serialize(j);
	j["Type"] = "TransformComponent";
	j["LocalPosition"] = LocalPosition;
	j["LocalRotation"] = LocalRotation;
	j["LocalScale"] = LocalScale;
	j["m_lockedScale"] = m_lockedScale;
}
