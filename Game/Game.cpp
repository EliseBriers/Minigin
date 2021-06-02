#include "GamePCH.h"
#include <Minigin.h>
#include "CubeGrid.h"
#include "GridHopper.h"
#include "QbertPlayer.h"
#include "QbertSpriteComponent.h"

int main( int, char*[] )
{
	dae::Minigin engine{ };
	engine.RegisterComponent<CubeGrid>( );
	engine.RegisterComponent<GridHopper>( );
	engine.RegisterComponent<QbertPlayer>( );
	engine.RegisterComponent<QbertSpriteComponent>( );
	engine.AddSceneFromFile( "test.json" );
	engine.SetActiveScene( "Demo" );
	engine.Run( );
	return 0;
}
