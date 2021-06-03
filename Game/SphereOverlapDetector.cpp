#include "GamePCH.h"
#include "SphereOverlapDetector.h"
#include <SDL.h>
#include <InitInfo.h>
#include "QbertSceneBehavior.h"

SphereOverlapDetector::SphereOverlapDetector( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: IComponent{ gameObject, std::move( name ) }
	, m_Callback{ DefaultCallback }
	, m_Circle{ }
	, m_Radius{ jsonObject.GetFloat( "radius" ) }
	, m_pTransform{ nullptr }
{
}

void SphereOverlapDetector::Update( const dae::UpdateInfo& )
{
	const auto it{ std::remove_if( m_Overlaps.begin( ), m_Overlaps.end( ), HasNoOverlap ) };

	for( auto idx{ it }; idx < m_Overlaps.end( ); ++idx )
	{
		m_Callback( idx->pOther, TriggerAction::Exit );
	}

	m_Overlaps.erase( it, m_Overlaps.end( ) );

	for( OverlapData& overlapData : m_Overlaps )
	{
		overlapData.HasNoOverlap = false;
	}


	UpdateCircle( );
}

void SphereOverlapDetector::Draw( dae::Renderer& renderer )
{
	const SDL_Color red{ 255, 0, 0, 255 };
	const SDL_Color green{ 0, 255, 0, 255 };
	renderer.RenderCircle( m_Circle.Pos, m_Circle.R, m_Overlaps.empty( ) ? red : green );
}

void SphereOverlapDetector::Init( const dae::InitInfo& initInfo )
{
	m_pTransform = m_GameObject.get( ).GetComponent<dae::TransformComponent>( );
	if( !m_pTransform )
	{
		dae::Logger::LogError( "SphereOverlapDetector::Init > GameObject has no transform" );
		return;
	}

	UpdateCircle( );

	QbertSceneBehavior* pSceneBehavior{ initInfo.Scene_GetSceneBehaviorAs<QbertSceneBehavior>( ) };

	if( !pSceneBehavior )
	{
		dae::Logger::LogWarning( "SphereOverlapDetector::Init > Scene doesn't have QbertSceneBehavior" );
		return;
	}

	pSceneBehavior->RegisterOverlapDetector( *this );
}

void SphereOverlapDetector::SetCallback( const callback_t& callback )
{
	m_Callback = callback;
}

void SphereOverlapDetector::UpdateCircle( )
{
	m_Circle.Pos = m_pTransform->GetPosition( );
	m_Circle.R = m_pTransform->GetScale( ) * m_Radius;
}

void SphereOverlapDetector::DefaultCallback( dae::GameObject*, TriggerAction )
{
	// if( ta == TriggerAction::Enter )
	// {
	// 	dae::Logger::LogInfo( "GameObject got entered by \"" + pGo->GetName( ) + "\" with id " + std::to_string( pGo->Id.GetValue( ) ) );
	// }
	// else
	// {
	// 	dae::Logger::LogInfo( "GameObject got exited by \"" + pGo->GetName( ) + "\" with id " + std::to_string( pGo->Id.GetValue( ) ) );
	// }
}

void SphereOverlapDetector::RegisterOverlap( dae::GameObject* pOther )
{
	const auto it{
		std::find_if( m_Overlaps.begin( ), m_Overlaps.end( ), [pOther]( const OverlapData& overlap )
		{
			return overlap.pOther->Id == pOther->Id;
		} )
	};

	if( it != m_Overlaps.end( ) )
	{
		it->HasNoOverlap = true;
		return;
	}

	m_Overlaps.push_back( { pOther, true } );
	m_Callback( pOther, TriggerAction::Enter );
}

void SphereOverlapDetector::DetectOverlap( SphereOverlapDetector& other )
{
	if( !AreCirclesOverlapping( m_Circle, other.m_Circle ) )
		return;

	RegisterOverlap( &other.GetGameObject( ) );
	other.RegisterOverlap( &GetGameObject( ) );
}

bool SphereOverlapDetector::HasNoOverlap( const OverlapData& overlap )
{
	return !overlap.HasNoOverlap;
}
