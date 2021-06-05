#pragma once
#include "Enums.h"

class IDirectionController
{
public:
	IDirectionController( ) = default;

	virtual MoveDirection GetMoveDirection( ) = 0;
	
	// Rule of 5
	virtual ~IDirectionController( ) = default;
	IDirectionController( const IDirectionController& other ) = delete;
	IDirectionController( IDirectionController&& other ) noexcept = delete;
	IDirectionController& operator=( const IDirectionController& other ) = delete;
	IDirectionController& operator=( IDirectionController&& other ) noexcept = delete;
};
