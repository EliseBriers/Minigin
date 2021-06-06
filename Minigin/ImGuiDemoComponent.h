#pragma once
#include "BaseComponent.h"

namespace dae
{
	class ImGuiDemoComponent final : public BaseComponent
	{
	public:
		ImGuiDemoComponent(GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name);
		void Draw( Renderer& ) override;
	};
}
