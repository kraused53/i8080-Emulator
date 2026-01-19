#pragma once

#include "i8080.h"

// SDL Settings
#include <SDL.h>

const int WINDOW_W = 224;
const int WINDOW_H = 256;
const int WINDOW_SCALE = 4;

// Timing Settings
const int TARGET_FPS = 50;
const int MSEC_PER_FRAME = ( int ) 1000 / TARGET_FPS;

class Engine {
private:
	// SDL
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	uint8_t screen_buffer[ 256 ][ 224 ][ 4 ];

	// Emulation Variables
	bool initialized;
	bool running;
	bool half_screen_interrupt;
	bool frame_finished;
	int msec_prev_frame;
	unsigned int cycles_elapsed;

public:
	// Constructors
	Engine( void );
	~Engine( void );

	// CPU
	i8080 cpu;

	// Emulation Steps
	void setup( void );
	void run( void );
	void handle_keydown( SDL_Event* e );
	void handle_keyup( SDL_Event* e );
	void input( void );
	void update( void );
	void render( void );

	bool is_initialized( void ) const { return initialized; }

	void debug( void );
};

