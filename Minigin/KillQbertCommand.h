#pragma once
#include "ICommand.h"
#include "QbertComponent.h"

namespace dae
{
	class KillQbertCommand final : public ICommand
	{
	public:
		KillQbertCommand( QbertComponent& qbertComponent )
			: m_Qbert{ qbertComponent }
		{
		}

		void Execute( ) override
		{
			m_Qbert.Die( );
		};

		~KillQbertCommand( ) override = default;
		KillQbertCommand( const KillQbertCommand& other ) = delete;
		KillQbertCommand( KillQbertCommand&& other ) noexcept = delete;
		KillQbertCommand& operator=( const KillQbertCommand& other ) = delete;
		KillQbertCommand& operator=( KillQbertCommand&& other ) noexcept = delete;
	private:
		QbertComponent& m_Qbert;
	};
}
