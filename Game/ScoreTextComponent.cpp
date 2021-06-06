#include "GamePCH.h"
#include "ScoreTextComponent.h"
#include "DynamicTextComponent.h"
#include "QbertSceneBehavior.h"

ScoreTextComponent::ScoreTextComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper&, std::string name )
	: IComponent{ gameObject, std::move( name ) }
{
}

void ScoreTextComponent::Init( const dae::InitInfo& initInfo )
{
	dae::DynamicTextComponent* pTextComp{ GetGameObject( ).GetComponent<dae::DynamicTextComponent>( ) };
	if( !pTextComp )
	{
		dae::Logger::LogWarning( "ScoreTextComponent::Init > pTextComp is nullptr" );
		return;
	}
	QbertSceneBehavior* pSceneBehavior{ initInfo.Scene_GetSceneBehaviorAs<QbertSceneBehavior>( ) };
	if( !pSceneBehavior )
	{
		dae::Logger::LogWarning( "ScoreTextComponent::Init > pSceneBehavior is nullptr" );
		return;
	}

	pTextComp->SetText( std::to_string( pSceneBehavior->GetScore( ) ) );
	pSceneBehavior->AddScoreObserver( [=]( const size_t& score )
	{
		pTextComp->SetText( std::to_string( score ) );
	} );
}
