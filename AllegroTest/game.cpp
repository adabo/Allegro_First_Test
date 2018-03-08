#include "game.h"
#include "Vector2d.h"

Game::Game(bool Is_running, bool Can_draw)
	:	game_is_running(Is_running),
		can_redraw(Can_draw)
		//player()
{
	player.x = 200; player.y = 200;
	player.x_speed = 10; player.y_speed = 10;

	projectile.x_speed = 10; projectile.y_speed = 10;
}

Game::~Game()
{
}

void Game::init_allegro()
{
	if (!al_init()) {
		std::cout << stderr << " Failed to initialize allegro.\n";
		return_value =  1;
	}
	else {
		std::cout << stderr << " al_init success.\n";
	}
}

void Game::init_addons()
{
	if (!al_init_primitives_addon()){
		std::cout << stderr << " Failed to initialize primitives addon.\n";
		return_value =  1;
	}
	else {
		std::cout << stderr << " al_init_primitives_addon Success.";
	}

	if (!al_install_keyboard()) {
		std::cout << stderr << " Failed to install keyboard.\n";
		return_value =  1;
	}
	else {
		std::cout << stderr << " al_install_keyboard() Success.";
	}
	if (!al_install_mouse()) {
		std::cout << stderr << " Failed to install mouse.\n";
		return_value =  1;
	}
	else {
		std::cout << stderr << " al_install_mouse() Success.";
	}
}

void Game::init_timer()
{
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		std::cout << stderr << " Failed to create timer.\n";
		return_value =  1;
	}
	else {
		std::cout << stderr << " al_create_timer success.\n";
	}
}

void Game::init_display()
{
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display) {
		std::cout << stderr << " Failed to create display.\n";
		return_value = 1;
	}
	else {
		std::cout << stderr << " al_create_display success.\n";
	}
}

void Game::create_event_queue()
{
	event_queue = al_create_event_queue();
	if (!event_queue) {
		std::cout << stderr << " Failed to create event queue.";
		return_value = 1;
	}
	else {
		std::cout << stderr << "al_create_event_queue success.\n";
	}
}

void Game::register_event_sources()
{
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
}

void Game::start_timer()
{
	al_start_timer(timer);
}

void Game::init_timeout()
{
	al_init_timeout(&timeout, 0.06);
}

void Game::handle_events()
{
	// Fetch the event (if one exists)
	al_wait_for_event(event_queue, &event);

	switch (event.type) {
		case ALLEGRO_EVENT_TIMER: can_redraw = true; break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE: game_is_running = false; break;
		case ALLEGRO_EVENT_KEY_DOWN:
			std::cout << "A key was Pressed: "<< event.keyboard.keycode << std::endl;
			store_key_state();
			break;
		case ALLEGRO_EVENT_KEY_UP: store_key_state(); break;
		case ALLEGRO_EVENT_MOUSE_AXES: handle_mouse_action(); return;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: handle_mouse_action(); break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP: handle_mouse_action(); break;
		default:
			std::cout << stderr << " Unsupported event received: " << event.type << std::endl;
			break;
	}
	
	handle_key_press();
}

void Game::handle_key_press()
{
	if (w_key_is_down) player.y -= player.y_speed;
	if (s_key_is_down) player.y += player.y_speed;
	if (a_key_is_down) player.x -= player.x_speed;
	if (d_key_is_down) player.x += player.x_speed;
	if (esc_key_is_down) game_is_running = false;
	//if (t_was_pressed) t_toggle = !t_toggle;

}
void Game::store_key_state()
{
	if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
		switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_W: w_key_is_down = true; break;
			case ALLEGRO_KEY_S: s_key_is_down = true; break;
			case ALLEGRO_KEY_A: a_key_is_down = true; break;
			case ALLEGRO_KEY_D: d_key_is_down = true; break;
			case ALLEGRO_KEY_ESCAPE: esc_key_is_down = true; break;
			default: break;
		}
	}
	if (event.type == ALLEGRO_EVENT_KEY_UP) {
		switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_W: w_key_is_down = false; break;
			case ALLEGRO_KEY_S: s_key_is_down = false; break;
			case ALLEGRO_KEY_A: a_key_is_down = false; break;
			case ALLEGRO_KEY_D: d_key_is_down = false; break;
			case ALLEGRO_KEY_ESCAPE: esc_key_is_down = false; break;
			default: break;
		}
	}
}

void Game::handle_mouse_action()
{
	switch(event.type) {
		case ALLEGRO_EVENT_MOUSE_AXES:
			target.x = event.mouse.x;
			target.y = event.mouse.y;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			projectile.count++;
			projectile.x = player.x;
			projectile.y = player.y;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			break;
		default: break;
	}
}

void Game::clamp_entity_to_screen(Entity *entity)
{
	if (entity->x < 1) 			   entity->x = 1;
	if (entity->x > SCREEN_WIDTH)  entity->x = SCREEN_WIDTH - 1;
	if (entity->y < 1) 			   entity->y = 1;
	if (entity->y > SCREEN_HEIGHT) entity->y = SCREEN_HEIGHT -1;
}

void Game::update()
{
	if (entity_is_out_of_bounds(&player)) {
		clamp_entity_to_screen(&player);
	}
	if (entity_is_out_of_bounds(&projectile)) {
		destroy_entity(&projectile);
	}
}

void Game::destroy_entity(Entity *entity)
{
	if (entity->count > 0) entity->count--;
}

void Game::draw()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	draw_entity();
	al_flip_display();
}


void Game::draw_entity() // TODO: Split draw functions for each entity member
{
	Vector2d player_vector(player.x, player.y);
	Vector2d target_vector(target.x, target.y);

	// If you imagine cartesian, x -> and y ^ of a triangle
	// This Vector2d overload takes two vectors and subtracts their components
	// (target.x - player.x, target.y - player.y)
	Vector2d side_AB(player_vector, target_vector);
	Vector2d line(side_AB.x, side_AB.y); // Magnitude, or hypotenuse
	float distance = line.get_distance();
	line.get_normal(distance);

	// Set line's origin to the player's position since that's where we want the
	// line to start from, like a projectile
	line.x = player.x;
	line.y = player.y;

	// Draw the line extending from player to target
	for (int length = 0; length < 80; length++) {
		line.x += line.normal_x;
		line.y += line.normal_y;
		al_draw_pixel(line.x, line.y , al_map_rgb(0, 255, 0));
	}

	if (projectile.count > 0) {
		projectile.x += line.normal_x;
		projectile.y += line.normal_y;
		al_draw_pixel(projectile.x, projectile.y, al_map_rgb(255,0,255));
	}

	/*
	static float red;
	for (int shape_width = 0; shape_width < 200; shape_width++) {
		undulate_color(&red);
		for (int i = 0; i < 40; i++) {
			al_draw_pixel((player.x + shape_width) + i, player.y + i, al_map_rgb(red, red, 255));
		}
	}
	*/
}

float Game::undulate_color(float *color)
{
	//static float r = 1;
	static bool red_reached_max = false;
	static bool red_reached_min = true;

	if (red_reached_max) *color -= 0.02;
	if (red_reached_min) *color += 0.02;
	if (*color > 254) {
		*color = 254;
		red_reached_max = !red_reached_max;
		red_reached_min = !red_reached_min;
	}
	else if (*color < 1) {
		*color = 1;
		red_reached_max = !red_reached_max;
		red_reached_min = !red_reached_min;
	}

	return *color;
}

bool Game::entity_is_out_of_bounds(Entity *entity_pos)
{
	if (entity_pos->x < 1) return true;
	else if (entity_pos->x > SCREEN_WIDTH) return true;
	else if (entity_pos->y < 1) return true;
	else if (entity_pos->y > SCREEN_HEIGHT) return true;
	else return false;
}

void Game::cleanup()
{
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
}
