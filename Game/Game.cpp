#include "GamePCH.h"
#include <vld.h>
#include <Minigin.h>
#include "CubeGrid.h"
#include "GridHopper.h"
#include "QbertPlayer.h"
#include "HopperSpriteComponent.h"
#include <Scene.h>
#include "QbertSceneBehavior.h"
#include "SphereOverlapDetector.h"
#include "DiskComponent.h"
#include "RandomBouncerComponent.h"
#include "CoilyComponent.h"
#include "LevelSelectorComponent.h"
#include "MenuSceneBehavior.h"
#include "ScoreTextComponent.h"

int main( int, char*[] )
{
	dae::Minigin engine{ };

	engine.RegisterComponent<CubeGrid>( );
	engine.RegisterComponent<GridHopper>( );
	engine.RegisterComponent<QbertPlayer>( );
	engine.RegisterComponent<HopperSpriteComponent>( );
	engine.RegisterComponent<SphereOverlapDetector>( );
	engine.RegisterComponent<DiskComponent>( );
	engine.RegisterComponent<RandomBouncerComponent>( );
	engine.RegisterComponent<CoilyComponent>( );
	engine.RegisterComponent<LevelSelectorComponent>( );
	engine.RegisterComponent<ScoreTextComponent>( );

	// ToDo: Complete SceneList
	MenuSceneBehavior::scene_array_t sceneList{ "Demo", "CoopLevel1", "VsLevel1" };

	size_t globalScore{ };

	engine.AddSceneFromFile( "main_menu.json" )->SetBehavior( std::make_unique<MenuSceneBehavior>( sceneList ) );
	engine.AddSceneFromFile( "test.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "Demo2", globalScore ) );
	engine.AddSceneFromFile( "game_over.json" );
	engine.AddSceneFromFile( "level_coop_1.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "GameOver", globalScore ) );
	engine.AddSceneFromFile( "level_vs_1.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "GameOver", globalScore ) );
	engine.AddSceneFromFile( "test2.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "Demo3", globalScore ) );
	engine.AddSceneFromFile( "test3.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "GameOver", globalScore ) );

	engine.SetActiveScene( "MainMenu" );
	engine.Run( );

	return 0;
}
