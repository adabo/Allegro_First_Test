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

struct Dimension {
	int width, height;
	int left, right, top, bot;
}; 

struct Entity {
	Entity();
	//void set_sides(Entity &_ent, int _x, int _y, int _w, int _h);
	//void set_sides(int _x, int _y, int _w, int _h);

	void set_sides(Entity &_ent, Coords _origin, Dimension _dimension);
	void set_sides(Coords _origin, Dimension _dimension);
	bool operator>(const Entity &entity) const;

	Vector2d velocity;
	Coords coord;
	Dimension dimension;
	float x_speed, y_speed;
	int count;
};

class Game
{
public:
	Entity aimer, player, target, projectile;
	~Game();
	Game(bool Is_running, bool Can_draw);
	void init_allegro();
	void init_timer();
	void start_timer();
	void init_display();
	void init_addons();
	void create_event_queue();
	void register_event_sources();
	void init_timeout();
	void handle_events();
	void save_key_state();
	void save_mouse_action();
	void update();
	void update_entities();
	void update_player();
	void update_projectile();
	void update_aimer();
	void update_target();
	void check_collision();
	bool entity_is_out_of_bounds(Coords *player_coord, int _offset);
	void clamp_entity_to_screen(Coords *entity_coord, int _offset);
	bool entities_collided(Entity &_entity0, Entity &_entity1);
	void destroy_entity(Entity *entity);
	void remove_dead_entities();
	void draw();
	void draw_entity();
	void draw_target();
	void draw_aimer();
	void draw_projectile();
	void draw_undulation();
	Vector2d get_vector(Coords &_start,  Coords _end, int _offset);
	float undulate_color(float *color);
	void cleanup();


	int last_key_pressed;
	bool w_key_is_down;
	bool s_key_is_down;
	bool a_key_is_down;
	bool d_key_is_down;
	bool esc_key_is_down;
	bool mouse_button_is_down;
	bool t_toggle;
	bool t_is_pressed;
	bool projectile_is_dead;
	bool game_is_running;
	bool can_update;
	bool can_redraw;
	int return_value;
	int mouse_x, mouse_y;
	const float FPS = 60;
	ALLEGRO_KEYBOARD_STATE current_state;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMEOUT timeout;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
};
