#include "Engine.h"

int main( int argc, char** argv ) {

	Engine system;
	if ( !system.is_initialized() ) {
		return 1;
	}
	system.run();

	return 0;
}