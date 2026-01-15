#include "Engine.h"

int main( int argc, char** argv ) {

	Engine engine;
	if ( !engine.is_initialized() ) {
		return 1;
	}

	engine.run();

	return 0;
}