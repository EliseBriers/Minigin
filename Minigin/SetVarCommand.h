#pragma once
#include "ICommand.h"
#include <type_traits>

namespace dae
{
	template<typename T>
	class SetVarCommand final : public ICommand
	{
	public:
		using value_type = T;

		SetVarCommand( value_type& ref, value_type value )
			: m_Ref{ ref }
			, m_Value{ value }
		{
		};

		void Execute( ) override
		{
			m_Ref = m_Value;
		};

		// Rule of 5
		~SetVarCommand( ) override = default;
		SetVarCommand( const SetVarCommand& other ) = delete;
		SetVarCommand( SetVarCommand&& other ) noexcept = delete;
		SetVarCommand& operator=( const SetVarCommand& other ) = delete;
		SetVarCommand& operator=( SetVarCommand&& other ) noexcept = delete;
	private:
		value_type& m_Ref;
		value_type m_Value;
	};
}
