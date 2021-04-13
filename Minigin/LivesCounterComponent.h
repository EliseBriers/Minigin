#pragma once
#include "IComponent.h"
#include "DynamicTextComponent.h"

namespace dae
{
	class LivesCounterComponent : public IComponent
	{
	public:
		LivesCounterComponent(GameObject& gameObject);
		virtual void Init( const InitInfo& initInfo ) override;
		void OnPlayerDeath();
		
		virtual ~LivesCounterComponent( ) override = default;
		LivesCounterComponent( const LivesCounterComponent& other ) = delete;
		LivesCounterComponent( LivesCounterComponent&& other ) noexcept = delete;
		LivesCounterComponent& operator=( const LivesCounterComponent& other ) = delete;
		LivesCounterComponent& operator=( LivesCounterComponent&& other ) noexcept = delete;
	private:
		uint32_t m_Lives;
		DynamicTextComponent* m_pTextComponent;
	};
}
