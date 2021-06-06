#include "GamePCH.h"
#include "LevelSelectorComponent.h"
#include "MenuControlCommand.h"
#include "SetVarCommand.h"

LevelSelectorComponent::LevelSelectorComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name )
	: BaseComponent{ gameObject, std::move( name ) }
	, m_CurrentIndex{ 0u }
	, m_Sprites{ jsonObject.GetObjectWrapper( "sprite_sheet" ) }
	, m_pSolo{ nullptr }
	, m_pCoop{ nullptr }
	, m_pVs{ nullptr }
	, m_Pressed{ false }
	, m_pSceneBehavior{ nullptr }
{
}

void LevelSelectorComponent::Draw( dae::Renderer& renderer )
{
	dae::StaticTextComponent* pSelected{ GetSelected( ) };
	const glm::vec2 origin{ pSelected->GetPosition( ) };
	const float offset{ pSelected->GetSize( ).x / 2.f + 50.f };
	const glm::vec2 offsetVec{ offset, 0.f };

	m_Sprites.Draw( renderer, origin - offsetVec, { 1.f, 0.5f }, 5.f, 0 );
	m_Sprites.Draw( renderer, origin + offsetVec, { 0.25f, 0.5f }, 5.f, 1 );
}

void LevelSelectorComponent::Init( const dae::InitInfo& initInfo )
{
	m_pSceneBehavior = initInfo.Scene_GetSceneBehaviorAs<MenuSceneBehavior>( );
	if( !m_pSceneBehavior )
	{
		dae::Logger::LogWarning( "LevelSelectorComponent::Init > m_pSceneBehavior is nullptr" );
		return;
	}
	const dae::ButtonState pressed{ dae::ButtonState::Pressed };
	const MenuControlCommand::Action decrement{ MenuControlCommand::Action::Decrement };
	const MenuControlCommand::Action increment{ MenuControlCommand::Action::Increment };
	const dae::ControllerButton up{ dae::ControllerButton::ButtonUp };
	const dae::ControllerButton down{ dae::ControllerButton::ButtonDown };
	const dae::ControllerButton confirm{ dae::ControllerButton::ButtonA };

	m_Sprites.Init( initInfo );
	GetComponentPointers( initInfo );
	
	// Up
	initInfo.Input_AddKeyboardCommand( pressed, SDLK_UP, std::make_unique<MenuControlCommand>( m_CurrentIndex, 3u, decrement ) );
	initInfo.Input_AddControllerCommand( 0u, pressed, up, std::make_unique<MenuControlCommand>( m_CurrentIndex, 3u, decrement ) );
	// Down
	initInfo.Input_AddKeyboardCommand( pressed, SDLK_DOWN, std::make_unique<MenuControlCommand>( m_CurrentIndex, 3u, increment ) );
	initInfo.Input_AddControllerCommand( 0u, pressed, down, std::make_unique<MenuControlCommand>( m_CurrentIndex, 3u, increment ) );

	// Activate
	initInfo.Input_AddKeyboardCommand( pressed, SDLK_SPACE, std::make_unique<dae::SetVarCommand<bool>>( m_Pressed, true ) );
	initInfo.Input_AddControllerCommand( 0u, pressed, confirm, std::make_unique<dae::SetVarCommand<bool>>( m_Pressed, true ) );
}

void LevelSelectorComponent::Update( const dae::UpdateInfo& )
{
	if( m_Pressed )
	{
		GetGameObject( ).Deactivate( );
		m_pSceneBehavior->OnSceneSelected( m_CurrentIndex );
	}
}

void LevelSelectorComponent::Activate( )
{
	m_Pressed = false;
	m_CurrentIndex = 0u;
}

dae::StaticTextComponent* LevelSelectorComponent::GetSelected( ) const
{
	switch( m_CurrentIndex )
	{
	case 0:
		return m_pSolo;
	case 1:
		return m_pCoop;
	case 2:
		return m_pVs;
	default:
		dae::Logger::LogWarning( "LevelSelectorComponent::GetSelected > Invalid index: " + std::to_string( m_CurrentIndex ) );
		return nullptr;
	}
}

void LevelSelectorComponent::GetComponentPointers( const dae::InitInfo& initInfo )
{
	m_pSolo = GetTextComponent( initInfo, "SoloText" );
	m_pCoop = GetTextComponent( initInfo, "CoopText" );
	m_pVs = GetTextComponent( initInfo, "VsText" );
}

dae::StaticTextComponent* LevelSelectorComponent::GetTextComponent( const dae::InitInfo& initInfo, const std::string& gameObjectName )
{
	dae::GameObject* pGameObject{ initInfo.Scene_GetGameObject( gameObjectName ) };
	if( !pGameObject )
	{
		dae::Logger::LogWarning( "LevelSelectorComponent::GetTextComponent > No GameObject \"" + gameObjectName + '"' );
		return nullptr;
	}

	dae::StaticTextComponent* pComp{ pGameObject->GetComponent<dae::StaticTextComponent>( ) };

	if( !pComp )
	{
		dae::Logger::LogWarning( "LevelSelectorComponent::GetTextComponent > GameObject \"" + gameObjectName + "\" doesn't contain a dae::StaticTextComponent" );
	}

	return pComp;
}
