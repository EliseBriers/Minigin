#include "GamePCH.h"
#include <Minigin.h>
#include "CubeGrid.h"
#include "GridHopper.h"
#include "QbertPlayer.h"
#include "QbertSpriteComponent.h"
#include <Scene.h>
#include "QbertSceneBehavior.h"
#include "SphereOverlapDetector.h"

int main( int, char*[] )
{
	dae::Minigin engine{ };
	engine.RegisterComponent<CubeGrid>( );
	engine.RegisterComponent<GridHopper>( );
	engine.RegisterComponent<QbertPlayer>( );
	engine.RegisterComponent<QbertSpriteComponent>( );
	engine.RegisterComponent<SphereOverlapDetector>( );
	engine.AddSceneFromFile( "test.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "Demo2", false ) );
	engine.AddSceneFromFile( "test2.json" )->SetBehavior( std::make_unique<QbertSceneBehavior>( "", true ) );
	engine.SetActiveScene( "Demo" );
	engine.Run( );
	return 0;
}
