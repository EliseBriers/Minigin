#include "GamePCH.h"
#include "QbertSpriteComponent.h"
#include "JsonObjectWrapper.h"

QbertSpriteComponent::QbertSpriteComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_pTransform{ nullptr }
	, m_SpriteSheet{ jsonObject.GetObjectWrapper( "sprite_sheet" ) }
	, m_Direction{ Direction::Down }
	, m_State{ PlayerState::Idle }
{
}

void QbertSpriteComponent::Draw( dae::Renderer& r )
{
	if( m_State == PlayerState::Dead )
		return;

	const glm::vec2 pos{ m_pTransform->GetPosition( ) };
	const float scale{ m_pTransform->GetScale( ) };

	m_SpriteSheet.Draw( r, pos, { 0.5f, 1.f }, scale, GetIndex( m_Direction, m_State ) );
}

void QbertSpriteComponent::Init( const dae::InitInfo& initInfo )
{
	m_SpriteSheet.Init( initInfo );
	m_pTransform = m_GameObject.get( ).GetComponent<dae::TransformComponent>( );
}

void QbertSpriteComponent::SetDirection( Direction direction )
{
	m_Direction = direction;
}

void QbertSpriteComponent::SetState( PlayerState state )
{
	m_State = state;
}

void QbertSpriteComponent::NextRotation( )
{
	switch( m_Direction )
	{
	case Direction::Up:
		m_Direction = Direction::Left;
		break;
	case Direction::Left:
		m_Direction = Direction::Down;
		break;
	case Direction::Right:
		m_Direction = Direction::Up;
		break;
	case Direction::Down:
		m_Direction = Direction::Right;
		break;
	default: ;
	}
}

size_t QbertSpriteComponent::GetIndex( Direction direction, PlayerState state )
{
	return static_cast<size_t>(direction) * 2u + static_cast<size_t>(state);
}
