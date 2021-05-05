#pragma once
#include "IComponent.h"
#include "Observable.h"

namespace dae
{
	class QbertComponent : public IComponent
	{
	public:
		QbertComponent( GameObject& gameObject );
		void Init( const InitInfo& initInfo ) override;
		void Update( const UpdateInfo& ) override;
		void Die( ) const;
		void AddObserver( const std::function<void( )>& observer );

		~QbertComponent( ) override = default;
		QbertComponent( const QbertComponent& other ) = delete;
		QbertComponent( QbertComponent&& other ) noexcept = delete;
		QbertComponent& operator=( const QbertComponent& other ) = delete;
		QbertComponent& operator=( QbertComponent&& other ) noexcept = delete;
	private:
		Observable m_PlayerDied;
		uint16_t m_TestSound;
		bool m_HasPlayed;
	};
}
