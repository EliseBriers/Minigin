#include "MiniginPCH.h"
#include "ImGuiDemoComponent.h"
#include "imgui.h"

dae::ImGuiDemoComponent::ImGuiDemoComponent( GameObject& gameObject )
	: IComponent{ gameObject }
{
}

void dae::ImGuiDemoComponent::Draw( Renderer& )
{
	bool showDemo{ true };
	ImGui::ShowDemoWindow( &showDemo );
}
