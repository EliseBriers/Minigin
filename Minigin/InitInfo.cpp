#include "MiniginPCH.h"
#include "InitInfo.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Scene.h"

dae::InitInfo::InitInfo( ResourceManager& resourceManager, Renderer& renderer, InputManager& inputManager, Scene& scene )
	: m_ResourceManager{ resourceManager }
	, m_Renderer{ renderer }
	, m_InputManager{ inputManager }
	, m_Scene{ scene }
{
}

const dae::Texture2D& dae::InitInfo::Resource_GetTexture( const std::string& file ) const
{
	return m_ResourceManager.GetTexture( file, m_Renderer );
}

const dae::Texture2D& dae::InitInfo::Resource_GetTextTexture( const std::string& text, const std::string& fileName, uint32_t size ) const
{
	return m_ResourceManager.LoadText( text, fileName, size, m_Renderer );
}

void dae::InitInfo::Input_AddControllerCommand( uint8_t controllerId, ButtonState buttonState, ControllerButton button, std::unique_ptr<ICommand> command ) const
{
	m_InputManager.AddControllerCommand( controllerId, buttonState, button, std::move( command ) );
}

void dae::InitInfo::Input_AddKeyboardCommand( ButtonState buttonState, SDL_Keycode button, std::unique_ptr<ICommand> command ) const
{
	m_InputManager.AddKeyboardCommand( buttonState, button, std::move( command ) );
}

void dae::InitInfo::Input_AddSDLEventCommand( Uint32 event, std::unique_ptr<ICommand> command ) const
{
	m_InputManager.AddSDLEventCommand( event, std::move( command ) );
}

dae::GameObject* dae::InitInfo::Scene_GetGameObject( const std::string& name ) const
{
	return m_Scene.GetGameObject( name );
}
