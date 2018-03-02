#include "game.h"


int main(int argc, char *argv[])
{
	Game game(true, false);

	game.init_allegro();
	game.init_timer();
	game.start_timer();
	game.init_display();
	game.create_event_queue();
	game.register_event_sources();
	game.init_addons();
	game.display_screen();
	game.go();

	// Game loop
	while (game.game_is_running) {
		game.go();
		game.update();
		game.draw();
	}

	game.cleanup();

	return game.return_value;
}
