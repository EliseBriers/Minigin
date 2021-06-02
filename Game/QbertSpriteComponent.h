#pragma once
#include <IComponent.h>
#include "SpriteSheet.h"

namespace dae
{
	class TransformComponent;
}

class QbertSpriteComponent : public dae::IComponent
{
public:
	QbertSpriteComponent(dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name);

	enum class Direction : size_t
	{
		Up,
		Left,
		Right,
		Down
	};

	enum class State : size_t
	{
		Idle,
		Jumping
	};

	void Draw( dae::Renderer& ) override;
	void Init( const dae::InitInfo& ) override;

	void SetDirection( Direction direction );
	void SetState( State state );
	
	// Rule of 5
	~QbertSpriteComponent( ) override = default;
	QbertSpriteComponent( const QbertSpriteComponent& other ) = delete;
	QbertSpriteComponent( QbertSpriteComponent&& other ) noexcept = delete;
	QbertSpriteComponent& operator=( const QbertSpriteComponent& other ) = delete;
	QbertSpriteComponent& operator=( QbertSpriteComponent&& other ) noexcept = delete;
private:
	dae::TransformComponent* m_pTransform;

	dae::SpriteSheet m_SpriteSheet;
	Direction m_Direction;
	State m_State;

	static size_t GetIndex( Direction direction, State state );
};
