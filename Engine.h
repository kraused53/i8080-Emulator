#pragma once

// SDL Settings
#include <SDL.h>
const int WINDOW_W = 224;
const int WINDOW_H = 256;

// Timing Settings
const int TARGET_FPS = 60;
const int MSEC_PER_FRAME = ( int ) 1000 / TARGET_FPS;

class Engine {
private:
	// SDL Settings
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Emulation Variables
	bool initialized;
	bool running;
	int msec_prev_frame;

public:
	// Constructors
	Engine( void );
	~Engine( void );

	// Emulation Steps
	void setup( void );
	void run( void );
	void handle_keydown( SDL_Event* e );
	void handle_keyup( SDL_Event* e );
	void input( void );
	void update( void );
	void render( void );

	bool is_initialized( void ) const { return initialized; }
};

