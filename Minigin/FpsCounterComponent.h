#pragma once
#include "IComponent.h"

namespace dae
{
	class DynamicTextComponent;

	class FpsCounterComponent final : public IComponent
	{
	public:
		FpsCounterComponent( GameObject& gameObject );

		virtual void Update(const UpdateInfo& updateInfo) override;
		virtual void Init( const InitInfo& ) override;

		virtual ~FpsCounterComponent( ) override = default;
		FpsCounterComponent( const FpsCounterComponent& other ) = delete;
		FpsCounterComponent( FpsCounterComponent&& other ) noexcept = delete;
		FpsCounterComponent& operator=( const FpsCounterComponent& other ) = delete;
		FpsCounterComponent& operator=( FpsCounterComponent&& other ) noexcept = delete;
	private:
		DynamicTextComponent* m_pTextComponent;
	};
}
