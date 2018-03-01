#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
class Game
{
public:
	Game();
	Game(bool Is_running, bool Can_draw);
	void init_game_engine();
	void init_timer();
	void init_timeout();
	void init_display();
	void start_timer();
	void create_event_queue();
	void register_event_sources();
	void init_allegro();
	void init_addons();
	void display_screen();
	void go();
	void handle_events();
	void update();
	void draw();
	void cleanup();

	bool is_running;
	bool can_redraw;
	int return_value;
	const float FPS = 60;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMEOUT timeout;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
};
