#pragma once
#include <SceneBehavior.h>
#include <array>

class MenuSceneBehavior final : public dae::SceneBehavior
{
public:
	using scene_array_t = std::array<std::string, 3>;
	MenuSceneBehavior( const scene_array_t& sceneArray );

	void OnSceneSelected( size_t index );

	~MenuSceneBehavior( ) override = default;
	MenuSceneBehavior( const MenuSceneBehavior& other ) = delete;
	MenuSceneBehavior( MenuSceneBehavior&& other ) noexcept = delete;
	MenuSceneBehavior& operator=( const MenuSceneBehavior& other ) = delete;
	MenuSceneBehavior& operator=( MenuSceneBehavior&& other ) noexcept = delete;
private:
	scene_array_t m_SceneArray;
};
