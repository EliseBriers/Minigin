#include "GamePCH.h"
#include "MenuControlCommand.h"

MenuControlCommand::MenuControlCommand( size_t& ref, size_t max, Action action )
	: m_Ref{ ref }
	, m_Max{ max }
	, m_Action{ action }
{
}

void MenuControlCommand::Execute( )
{
	switch( m_Action )
	{
	case Action::Increment:
		m_Ref++;
		m_Ref %= m_Max;
		break;
	case Action::Decrement:
		if( m_Ref == 0 )
			m_Ref = m_Max;
		--m_Ref;
		break;
	}
}
