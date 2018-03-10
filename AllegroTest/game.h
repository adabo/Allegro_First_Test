#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include "Vector2d.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct Coords {
	float x, y;
};

struct Shape {
	int width, height;
	int left, right, top, bot;
}; 

struct Entity {
	Entity();
	void set_sides(Entity &_ent, int _x, int _y, int _w, int _h);
	void set_sides(int _x, int _y, int _w, int _h);
	bool operator>(const Entity &entity) const;

	Vector2d velocity;
	Coords coords;
	Shape shape;
	float x_speed, y_speed;
	int count;
};

class Game
{
public:
	Entity aimer, player, target, projectile;
	~Game();
	Game(bool Is_running, bool Can_draw);
	void init_timer();
	void init_timeout();
	void init_display();
	void start_timer();
	void create_event_queue();
	void register_event_sources();
	void init_allegro();
	void init_addons();
	void handle_events();
	void update_entities();
	void update_player();
	void update_projectile();
	void update_aimer();
	void update_target();
	void check_collision();
	void remove_dead_entities();
	void save_mouse_action();
	void save_key_state();
	void update();
	void destroy_entity(Entity *entity);
	void draw();
	void draw_entity();
	void draw_target();
	void draw_undulation();
	bool entities_collided(Entity &_entity0, Entity &_entity1);
	Vector2d get_velocity(Coords &_coords0,  Coords _coords1);
	float undulate_color(float *color);
	void cleanup();
	bool entity_is_out_of_bounds(Coords *player_coords, int _offset);
	void clamp_entity_to_screen(Coords *entity_coords, int _offset);


	int last_key_pressed;
	bool w_key_is_down;
	bool s_key_is_down;
	bool a_key_is_down;
	bool d_key_is_down;
	bool esc_key_is_down;
	int mouse_x, mouse_y;
	bool mouse_button_is_down;
	bool t_toggle;
	bool t_is_pressed;
	bool projectile_is_dead;
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
