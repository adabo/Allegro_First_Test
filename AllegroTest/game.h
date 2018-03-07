#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Game
{
public:
	struct Entity {
		//Entity(int x_pos, int y_pos);
		float x, y;
		float x_speed, y_speed;
	}player, target;
	
	~Game();
	Game(bool Is_running, bool Can_draw);
	//void init_game_engine();
	void init_timer();
	void init_timeout();
	void init_display();
	void start_timer();
	void create_event_queue();
	void register_event_sources();
	void init_allegro();
	void init_addons();
	//void display_screen();
	void handle_events();
	void handle_key_press();
	void handle_mouse_action(ALLEGRO_EVENT mouse_event, Entity *player_pos);
	void store_key_state();
	void update();
	void draw();
	float undulate_color(float *color);
	void cleanup();
	bool player_is_out_of_bounds(Entity *player_pos);
	void clamp_player_to_screen(Entity *player);

	void draw_player();

	int last_key_pressed;
	bool w_key_is_down;
	bool s_key_is_down;
	bool a_key_is_down;
	bool d_key_is_down;
	bool key_was_pressed;
	bool t_toggle;
	bool t_is_pressed;
	bool game_is_running;
	bool can_redraw;
	int return_value;
	const float FPS = 60;
	ALLEGRO_KEYBOARD_STATE current_state;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMEOUT timeout;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
};
