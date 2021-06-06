#include "MiniginPCH.h"
#include "ImGuiDemoComponent.h"
#include "imgui.h"

dae::ImGuiDemoComponent::ImGuiDemoComponent( GameObject& gameObject, const JsonObjectWrapper&, std::string name )
	: BaseComponent{ gameObject, std::move( name ) }
{
}

void dae::ImGuiDemoComponent::Draw( Renderer& )
{
	bool showDemo{ true };
	ImGui::ShowDemoWindow( &showDemo );
}
