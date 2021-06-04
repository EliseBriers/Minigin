#pragma once
#include <IComponent.h>
#include "SpriteSheet.h"
#include "Enums.h"

namespace dae
{
	class TransformComponent;
}

class HopperSpriteComponent final : public dae::IComponent
{
public:
	HopperSpriteComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	void Draw( dae::Renderer& ) override;
	void Init( const dae::InitInfo& ) override;

	void SetDirection( SpriteDirection direction );
	void SetState( SpriteState state );
	void NextRotation( );
	
	// Rule of 5
	~HopperSpriteComponent( ) override = default;
	HopperSpriteComponent( const HopperSpriteComponent& other ) = delete;
	HopperSpriteComponent( HopperSpriteComponent&& other ) noexcept = delete;
	HopperSpriteComponent& operator=( const HopperSpriteComponent& other ) = delete;
	HopperSpriteComponent& operator=( HopperSpriteComponent&& other ) noexcept = delete;
private:
	dae::TransformComponent* m_pTransform;

	dae::SpriteSheet m_SpriteSheet;
	SpriteDirection m_Direction;
	SpriteState m_State;
	HopperType m_Type;
};
