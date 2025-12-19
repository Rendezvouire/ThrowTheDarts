all:
	g++ main.cpp Game.cpp Menu_State.cpp Game_State.cpp -o throw_the_darts -I/usr/include -L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system