#include "Engine.h"

#include <format>
#include "Logger.h"

// Constructors
Engine::Engine( void ) {
	// Initialize emulation variables
	running = false;
	initialized = false;
	msec_prev_frame = 0;
	window = nullptr;
	renderer = nullptr;

	for ( int h = 0; h < WINDOW_H; h++ ) {
		for ( int w = 0; w < WINDOW_W; w++ ) {
			screen_buffer[ h ][ w ][ 0 ] = 0;
			screen_buffer[ h ][ w ][ 1 ] = 0;
			screen_buffer[ h ][ w ][ 2 ] = 0;
			screen_buffer[ h ][ w ][ 3 ] = 0xFF;
		}
	}

	// Initialize SDL Subsystems
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
		Logger::Err( std::format( "Could not initialize SDL Subsystems: {}", SDL_GetError() ) );
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

	// Initialize Texture
	texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_W, WINDOW_H );
	if ( texture == NULL ) {
		SDL_Log( "unable to create texture: %s", SDL_GetError() );
		return;
	}

	// No errors, set initialized
	Logger::Log( "Engine initialized without error!" );
	initialized = true;
}

Engine::~Engine( void ) {
	SDL_DestroyTexture( texture );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
	Logger::Log( "Engine memory freed!" );
}


// Emulation Steps
void Engine::setup( void ) {
	// Reset Cpu
	cpu.reset();
	cpu.clear_memory();

	// Load the 4 ROMs into memory
	if ( !cpu.load_rom( ".\\ROMs\\invaders.H", 0x0000 ) ) {
		return;
	}

	if ( !cpu.load_rom( ".\\ROMs\\invaders.G", 0x0800 ) ) {
		return;
	}

	if ( !cpu.load_rom( ".\\ROMs\\invaders.F", 0x1000 ) ) {
		return;
	}

	if ( !cpu.load_rom( ".\\ROMs\\invaders.E", 0x1800 ) ) {
		return;
	}

	// CPU ready with no errors. Start emulator
	running = true;
}

void Engine::run( void ) {
	setup();
	int cnt = 0;
	while ( running ) {
		input();
		/*
		printf( "PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, CYC: %lu",
						cpu.PC, cpu.get_register( i8080::RP_AF ), cpu.get_register( i8080::RP_BC ), cpu.get_register( i8080::RP_DE ), cpu.get_register( i8080::RP_HL ),
						cpu.SP, cpu.cycles );

		printf( "\t(%02X %02X %02X %02X)", cpu.read_byte( cpu.PC ), cpu.read_byte( cpu.PC + 1 ),
						cpu.read_byte( cpu.PC + 2 ), cpu.read_byte( cpu.PC + 3 ) );

		printf( "\n" );
		*/
		update();

		if ( cnt == 10000 ) {
			cnt = 0;

			render();
		}
		cnt++;
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
	if ( cpu.error ) {
		running = false;
	}else {
		cpu.step();
	}
}

void Engine::render( void ) {

	for ( int i = 0; i < 256 * 224 / 8; i++ ) {
		const int y = i * 8 / 256;
		const int base_x = ( i * 8 ) % 256;
		const uint8_t cur_byte = cpu.memory[ 0x2400 + i ];

		for ( uint8_t bit = 0; bit < 8; bit++ ) {
			int px = base_x + bit;
			int py = y;
			const bool is_pixel_lit = ( cur_byte >> bit ) & 1;
			uint8_t r = 0, g = 0, b = 0;
		
			if ( is_pixel_lit ) {
				r = 0;
				g = 255;
				b = 0;
			} else {
				r = 0;
				g = 0;
				b = 0;

			}

			// space invaders' screen is rotated 90 degrees anti-clockwise
			// so we invert the coordinates:
			const int temp_x = px;
			px = py;
			py = -temp_x + 256 - 1;

			screen_buffer[ py ][ px ][ 0 ] = r;
			screen_buffer[ py ][ px ][ 1 ] = g;
			screen_buffer[ py ][ px ][ 2 ] = b;
		}
	}

	int pitch = 0;
	void* pixels = NULL;
	if ( SDL_LockTexture( texture, NULL, &pixels, &pitch ) != 0 ) {
		SDL_Log( "Unable to lock texture: %s", SDL_GetError() );
	} else {
		memcpy( pixels, screen_buffer, pitch * WINDOW_H );
	}
	SDL_UnlockTexture( texture );

	SDL_RenderClear( renderer );
	SDL_RenderCopy( renderer, texture, NULL, NULL );
	SDL_RenderPresent( renderer );
}