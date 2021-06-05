#pragma once
#include <ICommand.h>

class MenuControlCommand : public dae::ICommand
{
public:
	enum class Action
	{
		Increment,
		Decrement
	};

	MenuControlCommand( size_t& ref, size_t max, Action action );;

	virtual void Execute( ) override;;

	// Rule of 5
	virtual ~MenuControlCommand( ) override = default;
	MenuControlCommand( const MenuControlCommand& other ) = delete;
	MenuControlCommand( MenuControlCommand&& other ) noexcept = delete;
	MenuControlCommand& operator=( const MenuControlCommand& other ) = delete;
	MenuControlCommand& operator=( MenuControlCommand&& other ) noexcept = delete;
private:
	size_t& m_Ref;
	size_t m_Max;
	Action m_Action;
};
