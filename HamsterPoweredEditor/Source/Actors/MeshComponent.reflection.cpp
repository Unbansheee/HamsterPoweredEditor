#pragma once
#include "MeshComponent.h"
#include "Core/JSONConversions.h"


void MeshComponent::Deserialize(nlohmann::json& j){
	__super::Deserialize(j);
	m_Mesh = j["m_Mesh"];
	m_MeshPath = j["m_MeshPath"];
}


void MeshComponent::Serialize(nlohmann::json& j){
	__super::Serialize(j);
	j["Type"] = "MeshComponent";
	j["m_Mesh"] = m_Mesh;
	j["m_MeshPath"] = m_MeshPath;
}
