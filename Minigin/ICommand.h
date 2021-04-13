#pragma once
namespace dae
{
	class ICommand
	{
	public:
		virtual void Execute( ) = 0;

		ICommand( ) = default;

		// Rule of 5
		virtual ~ICommand( ) = default;
		ICommand( const ICommand& other ) = delete;
		ICommand( ICommand&& other ) noexcept = delete;
		ICommand& operator=( const ICommand& other ) = delete;
		ICommand& operator=( ICommand&& other ) noexcept = delete;
	};
}
