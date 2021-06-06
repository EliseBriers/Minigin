#pragma once
#include <BaseComponent.h>

class ScoreTextComponent final : public dae::BaseComponent
{
public:
	ScoreTextComponent( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	virtual void Init( const dae::InitInfo& ) override;

	// Rule of 5
	virtual ~ScoreTextComponent( ) override = default;
	ScoreTextComponent( const ScoreTextComponent& other ) = delete;
	ScoreTextComponent( ScoreTextComponent&& other ) noexcept = delete;
	ScoreTextComponent& operator=( const ScoreTextComponent& other ) = delete;
	ScoreTextComponent& operator=( ScoreTextComponent&& other ) noexcept = delete;
private:
	
};
