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
#include "GameOverSceneBehavior.h"

int main( int, char*[] )
{
	srand( static_cast<unsigned>(time( nullptr )) );

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
	MenuSceneBehavior::scene_array_t sceneList{ "SoloLevel1", "CoopLevel1", "VsLevel1" };

	size_t globalScore{ };

	engine.AddSceneFromFile( "Global/main_menu.json" )->SetBehavior( std::make_unique<MenuSceneBehavior>( sceneList ) );

	engine.AddSceneFromFile( "Solo/level_solo_1.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "SoloLevel2", globalScore ) );
	engine.AddSceneFromFile( "Solo/level_solo_2.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "SoloLevel3", globalScore ) );
	engine.AddSceneFromFile( "Solo/level_solo_3.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "GameOver", globalScore ) );

	engine.AddSceneFromFile( "Coop/level_coop_1.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "CoopLevel2", globalScore ) );
	engine.AddSceneFromFile( "Coop/level_coop_2.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "CoopLevel3", globalScore ) );
	engine.AddSceneFromFile( "Coop/level_coop_3.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "GameOver", globalScore ) );

	engine.AddSceneFromFile( "Vs/level_vs_1.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "VsLevel2", globalScore ) );
	engine.AddSceneFromFile( "Vs/level_vs_2.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "VsLevel3", globalScore ) );
	engine.AddSceneFromFile( "Vs/level_vs_3.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "GameOver", globalScore ) );


	engine.AddSceneFromFile( "Global/game_over.json" )->SetBehavior( std::make_unique<GameOverSceneBehavior>( globalScore ) );

	engine.SetActiveScene( "MainMenu" );
	engine.Run( );

	return 0;
}
