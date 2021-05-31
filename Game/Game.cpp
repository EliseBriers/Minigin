#include <Minigin.h>
#include "CubeGrid.h"

int main( int, char*[] )
{
	dae::Minigin engine{ };
	engine.RegisterComponent<CubeGrid>( );
	engine.AddSceneFromFile( "test.json" );
	engine.SetActiveScene( "Demo" );
	engine.Run( );
	return 0;
}
