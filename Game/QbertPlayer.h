#pragma once
#include <IComponent.h>
#include "GridHopper.h"

namespace dae
{
	class JsonObjectWrapper;
	class TimerComponent;
}

class QbertSpriteComponent;

class QbertPlayer : public dae::IComponent
{
	enum class State
	{
		Idle,
		Jumping,
		Dead,
		Moving
	};

public:
	QbertPlayer( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject, std::string name );

	void Update( const dae::UpdateInfo& ) override;
	void Init( const dae::InitInfo& initInfo ) override;

	void OnTouchDown( GridHopper::TouchdownType touchdownType );
	void Respawn( );
	void OnDeath( );

	~QbertPlayer( ) override = default;
	QbertPlayer( const QbertPlayer& other ) = delete;
	QbertPlayer( QbertPlayer&& other ) noexcept = delete;
	QbertPlayer& operator=( const QbertPlayer& other ) = delete;
	QbertPlayer& operator=( QbertPlayer&& other ) noexcept = delete;
private:
	State m_State;
	GridHopper* m_pGridHopper;
	QbertSpriteComponent* m_pSprite;
	dae::TimerComponent* m_pRespawnTimer;
	bool m_InputLeft;
	bool m_InputRight;
	bool m_InputUp;
	bool m_InputDown;

	int GetInputCount( ) const;
};
