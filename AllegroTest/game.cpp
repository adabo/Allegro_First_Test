#include "game.h"

Game::Game()
	:	game_is_running(true),
		can_redraw(false),
		can_update(false),
		can_draw_angle(false),
		event_occurred(false)
{
	aimer.coords.x = player.coords.x = 200; 
	aimer.coords.y = player.coords.y = 200;
	player.x_speed = 10;
	player.y_speed = 4;
	projectile.x_speed = 8;
	projectile.y_speed = 8;
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
	else std::cout << stderr << " al_init success.\n";
}

void Game::init_addons()
{
	if (!al_init_primitives_addon()){
		std::cout << stderr << " Failed to initialize primitives addon.\n";
		return_value =  1;
	}
	else std::cout << stderr << " al_init_primitives_addon Success.";

	if (!al_install_keyboard()) {
		std::cout << stderr << " Failed to install keyboard.\n";
		return_value =  1;
	}
	else std::cout << stderr << " al_install_keyboard() Success.";

	if (!al_install_mouse()) {
		std::cout << stderr << " Failed to install mouse.\n";
		return_value =  1;
	}
	else std::cout << stderr << " al_install_mouse() Success.";
}

void Game::init_timer()
{
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		std::cout << stderr << " Failed to create timer.\n";
		return_value =  1;
	}
	else std::cout << stderr << " al_create_timer success.\n";
}

void Game::init_display()
{
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display) {
		std::cout << stderr << " Failed to create display.\n";
		return_value = 1;
	}
	else std::cout << stderr << " al_create_display success.\n";
}

void Game::create_event_queue()
{
	event_queue = al_create_event_queue();
	if (!event_queue) {
		std::cout << stderr << " Failed to create event queue.";
		return_value = 1;
	}
	else std::cout << stderr << "al_create_event_queue success.\n";
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
		case ALLEGRO_EVENT_TIMER:
			// Check if the queue is empty before any updates or draw calls
			if (al_is_event_queue_empty(event_queue)) {
				can_update = true;
				can_redraw = true;
				event_occurred = false;
			}
			break;

			/*
			if (al_is_event_queue_empty(event_queue)) {
				update_entities();
				can_redraw = true; 
			}
			break;
			*/
		case ALLEGRO_EVENT_DISPLAY_CLOSE: game_is_running = false; break;
		case ALLEGRO_EVENT_KEY_DOWN:
			  save_key_state();
			  event_occurred = true; break;
		case ALLEGRO_EVENT_KEY_UP:
			  save_key_state();
			  event_occurred = true; break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			  save_mouse_action();
			  event_occurred = true; break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			  save_mouse_action();
			  event_occurred = true; break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			  save_mouse_action();
			  event_occurred = true; break;
		default: break;
	}
}

void Game::update_entities()
{
	update_player();
	update_projectile();
	update_aimer();
	update_target();
}

void Game::save_key_state()
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

void Game::save_mouse_action()
{
	switch(event.type) {
		case ALLEGRO_EVENT_MOUSE_AXES:
			mouse_x = event.mouse.x;
			mouse_y = event.mouse.y;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			mouse_button_is_down = true;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			mouse_button_is_down = false;
			break;
		default: break;
	}
}

void Game::update_player()
{
	if (w_key_is_down) player.coords.y -= player.y_speed;
	if (s_key_is_down) player.coords.y += player.y_speed;
	if (a_key_is_down) player.coords.x -= player.x_speed;
	if (d_key_is_down) player.coords.x += player.x_speed;
	if (esc_key_is_down) game_is_running = false;
	//if (t_was_pressed) t_toggle = !t_toggle;

	if (entity_is_out_of_bounds(&player.coords, 0))
		clamp_entity_to_screen(&player.coords, 0);

}

void Game::update_aimer()
{
	//aimer.coords.x = player.coords.x;
	//aimer.coords.y = player.coords.y;
	aimer.velocity = get_velocity(aimer.coords, Coords{(float)mouse_x, (float)mouse_y});

	int offset = 40;
	if (entity_is_out_of_bounds(&aimer.coords, offset))
		clamp_entity_to_screen(&aimer.coords, offset);
}

void Game::update_projectile()
{
	// Spawn a projectile if the mouse button was pressed
	if (mouse_button_is_down) {
		projectile.count++;
		projectile.coords.x = player.coords.x;
		projectile.coords.y = player.coords.y;
		projectile.velocity = get_velocity(projectile.coords,
			                               Coords{(float)mouse_x,
			                                      (float)mouse_y});
	}
	else if (projectile.count > 0 ) {
		projectile.coords.x += projectile.velocity.normal_x * projectile.x_speed;
		projectile.coords.y += projectile.velocity.normal_y * projectile.y_speed;
		//projectile.coords.x += projectile.velocity.normal_x;
		//projectile.coords.y += projectile.velocity.normal_y;
	}

	if (entity_is_out_of_bounds(&projectile.coords,0)) destroy_entity(&projectile);
}

void Game::update_target()
{
	/* For now the target is just the mouse position
	 * but eventually we want this to be some sprite or
	 * enemy */
}

void Game::clamp_entity_to_screen(Coords *entity_coords, int offset)
{
	if (entity_coords->x < 1) 			   entity_coords->x = 1;
	if (entity_coords->x > SCREEN_WIDTH)  entity_coords->x = SCREEN_WIDTH - 1;
	if (entity_coords->y < 1) 			   entity_coords->y = 1;
	if (entity_coords->y > SCREEN_HEIGHT) entity_coords->y = SCREEN_HEIGHT -1;

}

void Game::update()
{
	if (can_update) {
		update_entities();
		can_update = false;
	}
}

void Game::destroy_entity(Entity *entity)
{
	if (entity->count > 0) entity->count--;
}

void Game::draw()
{

	if (can_redraw) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		draw_entity();
		al_flip_display();
		can_redraw = false;
	}
}

void Game::draw_angle()
{
	float old_coords_x = aimer.coords.x;
	float old_coords_y = aimer.coords.y;
	for (int length = 0; length < 80; length++) {
		old_coords_x += aimer.velocity.normal_x;
		old_coords_y += aimer.velocity.normal_y;
		al_draw_pixel(old_coords_x, old_coords_y , al_map_rgb(0, 180, 180));
	}
}

void Game::draw_entity() // TODO: Split draw functions for each entity member
{

	Coords angle_coords{aimer.coords.x, aimer.coords.y};
	// Draw aimer
	for (int length = 0; length < 80; length++) {
		aimer.coords.x += aimer.velocity.normal_x;
		aimer.coords.y += aimer.velocity.normal_y;
		get_velocity(angle_coords, Coords{(float)mouse_x, (float)mouse_y});
		if (can_draw_angle) draw_angle();
		al_draw_pixel(aimer.coords.x, aimer.coords.y , al_map_rgb(0, 255, 0));
	}
	aimer.coords.x = player.coords.x; // I don't know why, but 
	aimer.coords.y = player.coords.y; // assigning these here
									  // works. If you put them in
									  // update_aimer(), it breaks

	// Draw projectile
	if (projectile.count > 0) {
		al_draw_pixel(projectile.coords.x,
			          projectile.coords.y,
			          al_map_rgb(255,0,255));
	}

	//draw_undulation();
}

void Game::draw_undulation()
{
	static float red;
	for (int shape_width = 0; shape_width < 200; shape_width++) {
		undulate_color(&red);
		for (int i = 0; i < 40; i++) {
			al_draw_pixel((player.coords.x + shape_width) + i, player.coords.y + i, al_map_rgb(red, red, 255));
		}
	}
}

Vector2d Game::get_velocity(Coords _coords0,  Coords _coords1)
{

	Vector2d entity0_vector(_coords0.x, _coords0.y);
	Vector2d entity1_vector(_coords1.x, _coords1.y);

	// If you imagine cartesian, x -> and y ^ of a triangle
	// This Vector2d overload takes two vectors and subtracts their components
	// (target.x - player.x, target.y - player.y)
	Vector2d side_AB(entity0_vector, entity1_vector);

	if (side_AB.x - side_AB.y > -15 && side_AB.x - side_AB.y < 15) {
		can_draw_angle = true;
	}
	else can_draw_angle = false;
	//else
	//	std::cout << "not within view" << std::endl;

	Vector2d velocity(side_AB.x, side_AB.y); // Magnitude, or hypotenuse
	float distance = velocity.get_distance();
	velocity.get_normal(distance);

	return velocity;
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

bool Game::entity_is_out_of_bounds(Coords *entity_coords,int _offset)
{
	if (entity_coords->x < 1) return true;
	else if (entity_coords->x > SCREEN_WIDTH) return true;
	else if (entity_coords->y < 1) return true;
	else if (entity_coords->y > SCREEN_HEIGHT) return true;
	else return false;
}

void Game::cleanup()
{
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
}
