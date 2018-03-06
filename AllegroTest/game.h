#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Game
{
public:
	struct Player{
		//Player(int x_pos, int y_pos);
		float x, y;
	}player;
	
	~Game();
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
	void handle_events();
	void handle_key_press(int key_code, Player *player_pos);
	void update();
	void draw();
	float undulate_color(float *color);
	void cleanup();
	bool player_is_out_of_bounds(Player *player_pos);
	void clamp_player_to_screen(Player *player);

	void draw_player();

	bool game_is_running;
	bool can_redraw;
	int return_value;
	const float FPS = 60;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMEOUT timeout;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
};
