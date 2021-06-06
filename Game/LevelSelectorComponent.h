#pragma once
#include <BaseComponent.h>
#include "StaticTextComponent.h"
#include "SpriteSheet.h"
#include "MenuSceneBehavior.h"

class LevelSelectorComponent : public dae::BaseComponent
{
public:
	LevelSelectorComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper&, std::string name );

	void Draw( dae::Renderer& ) override;
	void Init( const dae::InitInfo& initInfo ) override;
	virtual void Update( const dae::UpdateInfo& ) override;
	virtual void Activate( ) override;

	~LevelSelectorComponent( ) override = default;
	LevelSelectorComponent( const LevelSelectorComponent& other ) = delete;
	LevelSelectorComponent( LevelSelectorComponent&& other ) noexcept = delete;
	LevelSelectorComponent& operator=( const LevelSelectorComponent& other ) = delete;
	LevelSelectorComponent& operator=( LevelSelectorComponent&& other ) noexcept = delete;
private:
	size_t m_CurrentIndex;
	dae::SpriteSheet m_Sprites;

	dae::StaticTextComponent* m_pSolo;
	dae::StaticTextComponent* m_pCoop;
	dae::StaticTextComponent* m_pVs;

	bool m_Pressed;

	MenuSceneBehavior* m_pSceneBehavior;

	dae::StaticTextComponent* GetSelected( ) const;
	void GetComponentPointers( const dae::InitInfo& initInfo );
	static dae::StaticTextComponent* GetTextComponent( const dae::InitInfo& initInfo, const std::string& gameObjectName );
};
