#include "GamePCH.h"
#include "HopperSpriteComponent.h"
#include "JsonObjectWrapper.h"
#include "EnumHelpers.h"

HopperSpriteComponent::HopperSpriteComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_pTransform{ nullptr }
	, m_SpriteSheet{ jsonObject.GetObjectWrapper( "sprite_sheet" ) }
	, m_Direction{ SpriteDirection::Down }
	, m_State{ SpriteState::Idle }
	, m_Type{ EnumHelpers::StringToHopperType( jsonObject.GetString( "type" ) ) }
{
}

void HopperSpriteComponent::Draw( dae::Renderer& r )
{
	if( m_State == SpriteState::Dead )
		return;

	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const float scale{ m_pTransform->GetScale( ) };

	m_SpriteSheet.Draw( r, pos, { 0.5f, 1.f }, scale, EnumHelpers::GetHopperSpriteIndex( m_Type, m_Direction, m_State ) );
}

void HopperSpriteComponent::Init( const dae::InitInfo& initInfo )
{
	m_SpriteSheet.Init( initInfo );
	m_pTransform = m_GameObject.get( ).GetComponent<dae::TransformComponent>( );
}

void HopperSpriteComponent::SetDirection( SpriteDirection direction )
{
	m_Direction = direction;
}

void HopperSpriteComponent::SetState( SpriteState state )
{
	m_State = state;
}

void HopperSpriteComponent::NextRotation( )
{
	switch( m_Direction )
	{
	case SpriteDirection::Up:
		m_Direction = SpriteDirection::Left;
		break;
	case SpriteDirection::Left:
		m_Direction = SpriteDirection::Down;
		break;
	case SpriteDirection::Right:
		m_Direction = SpriteDirection::Up;
		break;
	case SpriteDirection::Down:
		m_Direction = SpriteDirection::Right;
		break;
	default: ;
	}
}
