#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

const float FPS = 60;

int main(int argc, char *argv[])
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool running = true;
	bool redraw = true;

	// Initialize allegro
	if (!al_init()) {
		std::cout << stderr << " Failed to initialize allegro.\n";
		return 1;
	}
	else{
		std::cout << stderr << " al_init success.\n";
	}

	// Initialize Addons
	if (!al_init_primitives_addon()){
		std::cout << stderr << " Failed to initialize primitives addon.\n";
		return 1;
	}
	else{
		std::cout << stderr << " al_init_primitives_addon Success.";
	}

	al_install_keyboard();

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		std::cout << stderr << " Failed to create timer.\n";
		return 1;
	}
	else{
		std::cout << stderr << " al_create_timer success.\n";
	}

	// Create the display
	display = al_create_display(640, 480);
	if (!display) {
		std::cout << stderr << " Failed to create display.\n";
		return 1;
	}
	else{
		std::cout << stderr << " al_create_display success.\n";
	}

	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		std::cout << stderr << " Failed to create event queue.";
		return 1;
	}
	else{
		std::cout << stderr << "al_create_event_queue success.\n";
	}

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Start the timer
	al_start_timer(timer);

	int transpose_value = 100;

	// Game loop
	while (running) {
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);

		// Fetch the event (if one exists)
		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		if (get_event) {
			switch (event.type) {
			case ALLEGRO_EVENT_TIMER:
				redraw = true;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;
			default:
				std::cout << stderr << "Unsupported event received: " << event.type << std::endl;
				break;
			}
		}

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			// Redraw
			al_clear_to_color(al_map_rgb(0, 0, 0));

			/* Place-holder for my draw function
			 * part of the game class
			 *                       |
			 * the_game.draw();
			 *                       |
			 * End place-holder
			 */

			al_flip_display();

			redraw = false;
		}
	}

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
