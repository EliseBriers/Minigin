#include "GamePCH.h"
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

	// ToDo: Complete SceneList
	MenuSceneBehavior::scene_array_t sceneList{ "Demo", "CoopLevel1", "VsLevel1" };


	engine.AddSceneFromFile( "main_menu.json" )->SetBehavior( std::make_unique<MenuSceneBehavior>( sceneList ) );
	engine.AddSceneFromFile( "test.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "Demo2", false ) );
	engine.AddSceneFromFile( "level_coop_1.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "", true ) );
	engine.AddSceneFromFile( "level_vs_1.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "", true ) );
	engine.AddSceneFromFile( "test2.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "Demo3", false ) );
	engine.AddSceneFromFile( "test3.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "", true ) );

	engine.SetActiveScene( "MainMenu" );
	engine.Run( );
	return 0;
}
