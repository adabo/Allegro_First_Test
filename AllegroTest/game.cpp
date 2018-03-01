#include "game.h"

Game::Game(bool Is_running, bool Can_draw)
	: 	is_running(Is_running),
		can_redraw(Can_draw)
{
}

Game::Game()
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
	display = al_create_display(640, 480);
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
}

void Game::display_screen()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
}

void Game::start_timer()
{
	al_start_timer(timer);
}

void Game::init_timeout()
{
	al_init_timeout(&timeout, 0.06);
}

void Game::init_game_engine()
{
}

void Game::handle_events()
{
	// Fetch the event (if one exists)
	bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

	// Handle the event
	if (get_event) {
		switch (event.type) {
		case ALLEGRO_EVENT_TIMER:
			can_redraw = true;
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			is_running = false;
			break;
		default:
			std::cout << stderr << "Unsupported event received: " << event.type << std::endl;
			break;
		}
	}
}

void Game::go()
{
	// Check if we need to redraw
	if (can_redraw && al_is_event_queue_empty(event_queue)) {
		// Redraw
		al_clear_to_color(al_map_rgb(0, 0, 0));
		draw();
		al_flip_display();
		can_redraw = false;
	}
}

void Game::update()
{
}

void Game::draw()
{
}

void Game::cleanup()
{
al_destroy_display(display);
al_destroy_event_queue(event_queue);
}
