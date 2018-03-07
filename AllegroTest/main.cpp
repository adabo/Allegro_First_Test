#include "game.h"

int main(int argc, char *argv[])
{
	Game game(true, false);

	game.init_allegro();
	game.init_timer();
	game.start_timer();
	game.init_display();
	game.init_addons();
	game.create_event_queue();
	game.register_event_sources();

	// Game loop
	while (game.game_is_running) {
		game.init_timeout();
		game.handle_events();
		game.update();
		game.draw();
	}

	game.cleanup();

	return game.return_value;
}
