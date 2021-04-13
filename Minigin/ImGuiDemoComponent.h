#pragma once
#include "IComponent.h"

namespace dae
{
	class ImGuiDemoComponent final : public IComponent
	{
	public:
		ImGuiDemoComponent( GameObject& gameObject );
		void Draw( Renderer& ) override;
	};
}
