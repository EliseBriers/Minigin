#pragma once

enum class MoveDirection
{
	Left,
	Right,
	Up,
	Down
};

enum class LevelColor : int
{
	Teal,
	Orange,
	Gray,
	Blue,
	Black,
	LimeOrange,
	LimeBlue,
	Green,
	Void,
	CubeColor_Size
};

enum class TriggerAction
{
	Enter,
	Exit
};

enum class PlayerState
{
	Idle,
	Jumping,
	Dead,
	AwaitingMove,
	Moving
};

enum class DiskState
{
	Idle,
	Moving,
	Arrived,
	Inactive
};
