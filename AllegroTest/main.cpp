#include "game.h"


int main(int argc, char *argv[])
{
	Game game(true, true);
	game.init_allegro();
	game.init_game_engine();
	game.init_timer();
	game.init_timeout();
	game.start_timer();
	game.init_display();
	game.create_event_queue();
	game.register_event_sources();
	game.init_addons();
	game.handle_events();
	game.display_screen();
	game.go();
	game.update();
	game.draw();
	game.cleanup();

	// Game loop
	while (game.is_running) {
		game.go();
	}


	return game.return_value;
}
