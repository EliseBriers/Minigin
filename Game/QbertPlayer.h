#pragma once
#include <IComponent.h>

namespace dae
{
	class JsonObjectWrapper;
}

class GridHopper;
class QbertSpriteComponent;

class QbertPlayer : public dae::IComponent
{
public:
	QbertPlayer( dae::GameObject& gameObject, const dae::JsonObjectWrapper& jsonObject );

	void Update( const dae::UpdateInfo& ) override;
	void Init( const dae::InitInfo& initInfo ) override;

	~QbertPlayer( ) override = default;
	QbertPlayer( const QbertPlayer& other ) = delete;
	QbertPlayer( QbertPlayer&& other ) noexcept = delete;
	QbertPlayer& operator=( const QbertPlayer& other ) = delete;
	QbertPlayer& operator=( QbertPlayer&& other ) noexcept = delete;
private:
	GridHopper* m_pGridHopper;
	QbertSpriteComponent* m_pSprite;
	bool m_InputLeft;
	bool m_InputRight;
	bool m_InputUp;
	bool m_InputDown;

	int GetInputCount( ) const;
};
