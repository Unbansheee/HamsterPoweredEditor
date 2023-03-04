#include "Actors/AnimatedSpriteComponent.h"
#include "Actors/SpriteComponent.h"
#include "Actors/TextLabelComponent.h"
#include "Actors/ClothComponent.h"
#include "Actors/MeshComponent.h"
#include "Actors/SkyboxComponent.h"
#include "Actors/NameComponent.h"
#include "Actors/TransformComponent.h"
#include "Actors/DirectionalLightComponent.h"
#include "Actors/PointLightComponent.h"
#include "Core/BoxCollider.h"
#include "Core/Collider.h"
#include "Actors/CameraController.h"
#define GENERATED_COMPONENTADDS() \
else if (type == "AnimatedSpriteComponent")\
{\
	component = GetComponent<AnimatedSpriteComponent>();\
	if (!component)\
	{\
		component = AddComponent<AnimatedSpriteComponent>();\
	}\
}\
else if (type == "SpriteComponent")\
{\
	component = GetComponent<SpriteComponent>();\
	if (!component)\
	{\
		component = AddComponent<SpriteComponent>();\
	}\
}\
else if (type == "TextLabelComponent")\
{\
	component = GetComponent<TextLabelComponent>();\
	if (!component)\
	{\
		component = AddComponent<TextLabelComponent>();\
	}\
}\
else if (type == "ClothComponent")\
{\
	component = GetComponent<ClothComponent>();\
	if (!component)\
	{\
		component = AddComponent<ClothComponent>();\
	}\
}\
else if (type == "MeshComponent")\
{\
	component = GetComponent<MeshComponent>();\
	if (!component)\
	{\
		component = AddComponent<MeshComponent>();\
	}\
}\
else if (type == "SkyboxComponent")\
{\
	component = GetComponent<SkyboxComponent>();\
	if (!component)\
	{\
		component = AddComponent<SkyboxComponent>();\
	}\
}\
else if (type == "NameComponent")\
{\
	component = GetComponent<NameComponent>();\
	if (!component)\
	{\
		component = AddComponent<NameComponent>();\
	}\
}\
else if (type == "TransformComponent")\
{\
	component = GetComponent<TransformComponent>();\
	if (!component)\
	{\
		component = AddComponent<TransformComponent>();\
	}\
}\
else if (type == "DirectionalLightComponent")\
{\
	component = GetComponent<DirectionalLightComponent>();\
	if (!component)\
	{\
		component = AddComponent<DirectionalLightComponent>();\
	}\
}\
else if (type == "PointLightComponent")\
{\
	component = GetComponent<PointLightComponent>();\
	if (!component)\
	{\
		component = AddComponent<PointLightComponent>();\
	}\
}\
else if (type == "BoxCollider")\
{\
	component = GetComponent<BoxCollider>();\
	if (!component)\
	{\
		component = AddComponent<BoxCollider>();\
	}\
}\
else if (type == "Collider")\
{\
	component = GetComponent<Collider>();\
	if (!component)\
	{\
		component = AddComponent<Collider>();\
	}\
}\
else if (type == "CameraController")\
{\
	component = GetComponent<CameraController>();\
	if (!component)\
	{\
		component = AddComponent<CameraController>();\
	}\
}\



#define GENERATED_COMPONENTADDSUI() \
	if (ImGui::BeginMenu("2D")){\
	if (ImGui::MenuItem("AnimatedSpriteComponent"))\
	{\
		selected->AddComponent<AnimatedSpriteComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	if (ImGui::MenuItem("SpriteComponent"))\
	{\
		selected->AddComponent<SpriteComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	if (ImGui::MenuItem("TextLabelComponent"))\
	{\
		selected->AddComponent<TextLabelComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	ImGui::EndMenu(); }\
	if (ImGui::BeginMenu("3D")){\
	if (ImGui::MenuItem("ClothComponent"))\
	{\
		selected->AddComponent<ClothComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	if (ImGui::MenuItem("MeshComponent"))\
	{\
		selected->AddComponent<MeshComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	if (ImGui::MenuItem("SkyboxComponent"))\
	{\
		selected->AddComponent<SkyboxComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	ImGui::EndMenu(); }\
	if (ImGui::BeginMenu("Core")){\
	if (ImGui::MenuItem("NameComponent"))\
	{\
		selected->AddComponent<NameComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	if (ImGui::MenuItem("TransformComponent"))\
	{\
		selected->AddComponent<TransformComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	ImGui::EndMenu(); }\
	if (ImGui::BeginMenu("Lights")){\
	if (ImGui::MenuItem("DirectionalLightComponent"))\
	{\
		selected->AddComponent<DirectionalLightComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	if (ImGui::MenuItem("PointLightComponent"))\
	{\
		selected->AddComponent<PointLightComponent>();\
		ImGui::CloseCurrentPopup();\
	}\
	ImGui::EndMenu(); }\
	if (ImGui::BeginMenu("Physics")){\
	if (ImGui::MenuItem("BoxCollider"))\
	{\
		selected->AddComponent<BoxCollider>();\
		ImGui::CloseCurrentPopup();\
	}\
	if (ImGui::MenuItem("Collider"))\
	{\
		selected->AddComponent<Collider>();\
		ImGui::CloseCurrentPopup();\
	}\
	ImGui::EndMenu(); }\
	if (ImGui::BeginMenu("Rendering")){\
	if (ImGui::MenuItem("CameraController"))\
	{\
		selected->AddComponent<CameraController>();\
		ImGui::CloseCurrentPopup();\
	}\
	ImGui::EndMenu(); }\
