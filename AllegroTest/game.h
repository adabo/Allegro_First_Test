#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
class Game
{
public:
	Game();
	void go();
	void handle_events();
	void update();
	void draw();
};
