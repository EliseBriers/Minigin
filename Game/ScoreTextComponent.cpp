#include "GamePCH.h"
#include "ScoreTextComponent.h"
#include "DynamicTextComponent.h"
#include "QbertSceneBehavior.h"
#include "GameOverSceneBehavior.h"

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
	GameOverSceneBehavior* pGameOverSceneBehavior{ initInfo.Scene_GetSceneBehaviorAs<GameOverSceneBehavior>( ) };
	if( !pSceneBehavior && !pGameOverSceneBehavior )
	{
		dae::Logger::LogWarning( "ScoreTextComponent::Init > pSceneBehavior and pGameOverSceneBehavior are nullptr" );
		return;
	}

	if( pSceneBehavior )
	{
		pTextComp->SetText( std::to_string( pSceneBehavior->GetScore( ) ) );
		pSceneBehavior->AddScoreObserver( [=]( const size_t& score )
		{
			if( pSceneBehavior )
				pTextComp->SetText( std::to_string( score ) );
			else
				pTextComp->SetText( std::to_string( score ) );
		} );
	}
	else
		pTextComp->SetText( "Final Score " + std::to_string( pGameOverSceneBehavior->GetScore( ) ) );
}
