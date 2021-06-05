#pragma once
#include "IComponent.h"
#include <functional>

namespace dae
{
	class TimerComponent : public IComponent
	{
		using timeout_callback_t = std::function<void( )>;
	public:
		TimerComponent( GameObject& gameObject, const JsonObjectWrapper& jsonObject, std::string name );
		void Update( const UpdateInfo& updateInfo ) override;

		void Start( );
		void Stop( );

		void SetLoop( bool loop );
		void SetMaxTime( float maxTime );

		void SetCallback( const timeout_callback_t& callback );
		bool IsRunning( ) const;

		~TimerComponent( ) override = default;
		TimerComponent( const TimerComponent& other ) = delete;
		TimerComponent( TimerComponent&& other ) noexcept = delete;
		TimerComponent& operator=( const TimerComponent& other ) = delete;
		TimerComponent& operator=( TimerComponent&& other ) noexcept = delete;
	private:
		timeout_callback_t m_Callback;
		float m_ElapsedSec;
		float m_MaxTime;
		bool m_IsActive;
		bool m_Loop;

		static void VoidCallback( );
	};
}
