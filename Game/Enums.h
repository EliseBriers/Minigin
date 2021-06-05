#pragma once

enum class MoveDirection
{
	UpLeft,
	DownRight,
	UpRight,
	DownLeft,
	Left,
	Right,
	None
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

enum class HopperType
{
	Qbert1,
	Qbert2,
	Coily,
	Ugg,
	Wrongway,
	Slick,
	Sam,
	Egg,
	HopperType_Size
};

// The difference between direction and spriteDirection is that spriteDirection is aligned with the .png with the sprites
enum class SpriteDirection : size_t
{
	Up,
	Left,
	Right,
	Down
};

enum class SpriteState : size_t
{
	Idle,
	Jumping,
	Dead
};

enum class RandomBounceDirection
{
	Down,
	UpLeft,
	UpRight
};

enum class RandomBouncerState
{
	Spawning,
	Active,
	Falling
};

enum class CoilyState
{
	SpawningEgg,
	Egg,
	Hatching,
	FollowingPlayer,
	Dead
};

enum class LevelType
{
	Solo,
	Coop,
	Vs
};