#include "GamePCH.h"
#define SDL_MAIN_HANDLED
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

	engine.AddSceneFromFile( "test.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "Demo2", false ) );
	engine.AddSceneFromFile( "test2.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "Demo3", false) );
	engine.AddSceneFromFile( "test3.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "", true ) );

	engine.SetActiveScene( "Demo" );
	engine.Run( );
	return 0;
}
