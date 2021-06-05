#pragma once
#include "IDirectionController.h"

class PlayerDirectionController final : public IDirectionController
{
public:
	PlayerDirectionController( );

	void InitControls( const dae::InitInfo& initInfo, bool isFirstPlayer );
	MoveDirection GetMoveDirection( ) override;

	// Rule of 5
	~PlayerDirectionController( ) override = default;
	PlayerDirectionController( const PlayerDirectionController& other ) = delete;
	PlayerDirectionController( PlayerDirectionController&& other ) noexcept = delete;
	PlayerDirectionController& operator=( const PlayerDirectionController& other ) = delete;
	PlayerDirectionController& operator=( PlayerDirectionController&& other ) noexcept = delete;
private:
	bool m_InputLeft;
	bool m_InputRight;
	bool m_InputUp;
	bool m_InputDown;
	int GetInputCount( ) const;
};
