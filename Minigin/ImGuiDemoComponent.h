#pragma once
#include "IComponent.h"

namespace dae
{
	class ImGuiDemoComponent final : public IComponent
	{
	public:
		ImGuiDemoComponent(GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name);
		void Draw( Renderer& ) override;
	};
}
