#include "game.h"

Game::Game(bool Is_running, bool Can_draw)
	:	projectile_is_dead(true),
	  	game_is_running(Is_running),
		can_update(false),
		can_redraw(Can_draw)
{
	aimer.coord = {200, 200};
	aimer.dimension = {2, 2};
	aimer.set_sides(aimer, aimer.coord, aimer.dimension);

	player.coord = {200, 200};
	player.dimension = {2, 2};
	player.set_sides(player, player.coord, player.dimension);

	target.coord = {200, 500};
	target.dimension = {50, 50};
	target.set_sides(target, target.coord, target.dimension);

	player.x_speed = 10;
	player.y_speed = 4;
	projectile.x_speed = 5;
	projectile.y_speed = 5;
}

Game::~Game()
{
}

Entity::Entity()
{
}

void Entity::set_sides(Entity &_ent, Coords _origin, Dimension _dimension)
{
	_ent.coord.x = _origin.x, _ent.coord.y = _origin.y;
	_ent.dimension.width = _dimension.width;
	_ent.dimension.height = _dimension.height;
	
	// Add the x/y components of each side
	// eg. (x1, y1) + (x2, y2) = (x1 + x2) + (y1 + y2)
	dimension.top = (_origin.x + (_origin.x + _dimension.width)) +
					(_origin.y + _origin.y);
	dimension.bot = (_origin.x + (_origin.x + _dimension.width)) +
					((_origin.y + _dimension.height) +
					 (_origin.y + _dimension.height));
	dimension.left = (_origin.x + _origin.x) +
					 (_origin.y + (_origin.y + _dimension.height));
	dimension.right = ((_origin.x + _dimension.width) + 
					  (_origin.x + _dimension.width)) +
					  (_origin.y + (_origin.y + dimension.height));
}

void Entity::set_sides(Coords _origin, Dimension _dimension)
{
	dimension.top = (_origin.x + (_origin.x + _dimension.width)) +
					(_origin.y + _origin.y);
	dimension.bot = (_origin.x + (_origin.x + _dimension.width)) +
					((_origin.y + _dimension.height) +
					 (_origin.y + _dimension.height));
	dimension.left = (_origin.x + _origin.x) +
					 (_origin.y + (_origin.y + _dimension.height));
	dimension.right = ((_origin.x + _dimension.width) + 
					  (_origin.x + _dimension.width)) +
					  (_origin.y + (_origin.y + dimension.height));
}

bool Entity::operator>(const Entity &rhs) const
{
	return (dimension.left < rhs.dimension.right && dimension.right > rhs.dimension.left &&
			dimension.bot > rhs.dimension.top && dimension.top < rhs.dimension.bot);
}

void Game::init_allegro()
{
	if (!al_init()) {
		std::cout << stderr << " Failed to initialize allegro.\n";
		return_value =  1;
	}
	else std::cout << stderr << " al_init success.\n";
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

void Game::start_timer()
{
	al_start_timer(timer);
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
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE: game_is_running = false; break;
		case ALLEGRO_EVENT_KEY_DOWN: save_key_state(); break;
		case ALLEGRO_EVENT_KEY_UP: save_key_state(); break;
		case ALLEGRO_EVENT_MOUSE_AXES: save_mouse_action(); break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: save_mouse_action(); break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP: save_mouse_action(); break;
		default: break;
	}
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
			std::cout << "PEW" << std::endl;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			mouse_button_is_down = false;
			break;
		default: break;
	}
}

void Game::update()
{
	if (can_update) {
		update_entities();
		can_update = false;
	}
}

void Game::update_entities()
{
	update_player();
	update_projectile();
	update_aimer();
	update_target();
	check_collision();
	remove_dead_entities();
}

void Game::update_player()
{
	if (w_key_is_down) player.coord.y -= player.y_speed;
	if (s_key_is_down) player.coord.y += player.y_speed;
	if (a_key_is_down) player.coord.x -= player.x_speed;
	if (d_key_is_down) player.coord.x += player.x_speed;
	if (esc_key_is_down) game_is_running = false;
	//if (t_was_pressed) t_toggle = !t_toggle;

	if (entity_is_out_of_bounds(&player.coord, 0))
		clamp_entity_to_screen(&player.coord, 0);

}

void Game::update_projectile()
{
	// Spawn a projectile if the mouse button was pressed
	if (mouse_button_is_down) {
		mouse_button_is_down = false;
		projectile.count++;
		if (projectile.count > 10)
			std::cout << projectile.count << std::endl;
		projectile.set_sides(projectile, player.coord, projectile.dimension);
		projectile.velocity = get_vector(projectile.coord,
			Coords{ (float)mouse_x,
				   (float)mouse_y }, 80);
	} // Continue update on living projectile
	else if (projectile.count > 0) {
		projectile.coord.x += projectile.velocity.normal_x *
			projectile.x_speed;
		projectile.coord.y += projectile.velocity.normal_y *
			projectile.y_speed;
			projectile.set_sides(projectile.coord, projectile.dimension);

		if (entity_is_out_of_bounds(&projectile.coord, 0)) {
			std::cout << "OUT OF BOUNDS" << std::endl;
			destroy_entity(&projectile);
		}
	}
}

void Game::update_aimer()
{
	//aimer.coord.x = player.coord.x;
	//aimer.coord.y = player.coord.y;
	aimer.velocity = get_vector(aimer.coord, Coords{(float)mouse_x, (float)mouse_y}, 0);

	int offset = 40;
	if (entity_is_out_of_bounds(&aimer.coord, offset)) {
		clamp_entity_to_screen(&aimer.coord, offset);
		std::cout << "aimer oob" << std::endl;
	}
}

void Game::update_target()
{
	/* For now the target is just the mouse position
	 * but eventually we want this to be some sprite or
	 * enemy */
}

void Game::check_collision()
{
	if (projectile.count > 0)
		if (entities_collided(projectile, target)) {
			std::cout << "HIT!" << std::endl;
			projectile_is_dead = true;
		}
}

bool Game::entity_is_out_of_bounds(Coords *entity_coord,int _offset)
{
	if (entity_coord->x < 1) return true;
	else if (entity_coord->x > SCREEN_WIDTH) return true;
	else if (entity_coord->y < 1) return true;
	else if (entity_coord->y > SCREEN_HEIGHT) return true;
	else return false;
}

void Game::clamp_entity_to_screen(Coords *entity_coord, int offset)
{
	if (entity_coord->x < 1) 			   entity_coord->x = 1;
	if (entity_coord->x > SCREEN_WIDTH)  entity_coord->x = SCREEN_WIDTH - 1;
	if (entity_coord->y < 1) 			   entity_coord->y = 1;
	if (entity_coord->y > SCREEN_HEIGHT) entity_coord->y = SCREEN_HEIGHT -1;
}

bool Game::entities_collided(Entity &_entity0, Entity &_entity1)
{
	return _entity0 > _entity1;
}

void Game::destroy_entity(Entity *entity)
{
	if (entity->count > 0) entity->count--;
}

void Game::remove_dead_entities()
{
	if (projectile_is_dead) {
		destroy_entity(&projectile);
		projectile_is_dead = false;
	}
}

void Game::draw()
{

	if (can_redraw) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		draw_entity();
		al_flip_display();
	}
}

void Game::draw_entity() // TODO: Split draw functions for each entity member
{
	draw_target();
	draw_aimer();
	draw_projectile();

	can_redraw = false;
	//draw_undulation();
}

void Game::draw_target()
{
	al_draw_filled_rectangle(target.coord.x,
			                 target.coord.y,
			                 target.coord.x + target.dimension.width,
		                     target.coord.y + target.dimension.height,
			                 al_map_rgb(150, 150, 0));
}

void Game::draw_aimer()
{
	for (int length = 0; length < 80; length++) {
		aimer.coord.x += aimer.velocity.normal_x;
		aimer.coord.y += aimer.velocity.normal_y;
		al_draw_pixel(aimer.coord.x, aimer.coord.y , al_map_rgb(0, 255, 0));
	}
	aimer.coord.x = player.coord.x; // I don't know why, but 
	aimer.coord.y = player.coord.y; // assigning these here
									  // works. If you put them in
									  // update_aimer(), it breaks
}

void Game::draw_projectile()
{
	if (projectile.count > 0) {
		al_draw_pixel(projectile.coord.x, projectile.coord.y,
			          al_map_rgb(255,0,255));
	}
}

void Game::draw_undulation()
{
	static float red;
	for (int dimension_width = 0; dimension_width < 200; dimension_width++) {
		undulate_color(&red);
		for (int i = 0; i < 40; i++) {
			al_draw_pixel((player.coord.x + dimension_width) + i, player.coord.y + i, al_map_rgb(red, red, 255));
		}
	}
}

Vector2d Game::get_vector(Coords &_start,  Coords _end, int _offset)
{

	Vector2d start_vector(_start.x, _start.y); // Make two vectors
	Vector2d end_vector(_end.x, _end.y); // ----------------
	Vector2d side(start_vector, end_vector); // Find their base and height (x,y)
	Vector2d velocity(side.x, side.y); // Magnitude, or hypotenuse
	float distance = velocity.get_distance(); // Find the displacement
	velocity.get_normal(distance); // Normalize the vector

	// Apply offset
	// TODO Find formula for getting new offset coords
	while (_offset) {
		_start.x += velocity.normal_x;
		_start.y += velocity.normal_y;
		_offset--;
	}
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

void Game::cleanup()
{
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
}
