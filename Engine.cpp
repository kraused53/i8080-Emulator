#include "Engine.h"

#include <format>
#include "Logger.h"

// Constructors
Engine::Engine( void ) {
	// Initialize emulation variables
	running = false;
	initialized = false;
	msec_prev_frame = 0;

	// Initialize SDL Subsystems
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
		Logger::Err( std::format( "Could not initialize SDL Subsystems: {}", SDL_GetError() ));
		return;
	}

	// Initialize SDL Window
	window = SDL_CreateWindow( "Space Invaders!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN );
	if ( !window ) {
		Logger::Err( std::format( "Could not initialize SDL window: {}", SDL_GetError() ) );
		return;
	}
	
	// Initialize SDL Renderer
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		Logger::Err( std::format( "Could not initialize SDL renderer: {}", SDL_GetError() ) );
		return;
	}

	// No errors, set initialized
	Logger::Log( "Engine initialized without error!" );
	initialized = true;
}

Engine::~Engine( void ) {
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
	Logger::Log( "Engine memory freed!" );
}


// Emulation Steps
void Engine::setup( void ) {
	running = true;
}

void Engine::run( void ) {
	// Initialize memory and start emulator
	setup();

	while ( running ) {
		input();
		update();
		render();
	}
}

void Engine::handle_keydown( SDL_Event* e ) {
	switch ( e->key.keysym.sym ) {
		case SDLK_ESCAPE: running = false; break;
		default: break;
	}
}

void Engine::handle_keyup( SDL_Event* e ) {
	switch ( e->key.keysym.sym ) {
		default: break;
	}
}

void Engine::input( void ) {
	SDL_Event e;

	while ( SDL_PollEvent( &e ) ) {
		switch ( e.type ) {
			case SDL_QUIT: running = false; break;
			case SDL_KEYDOWN: handle_keydown( &e ); break;
			case SDL_KEYUP: handle_keyup( &e ); break;
			default: break;
		}
	}
}

void Engine::update( void ) {

}

void Engine::render( void ) {
	// Clear buffer
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderer );

	// Push buffer to screen
	SDL_RenderPresent( renderer );
}
