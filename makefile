default:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/main.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ main.o -o bin/debug/main -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/main
debug:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/main.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ main.o -o bin/debug/main -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/main
release:
	@mkdir bin -p
	@mkdir bin/release -p
	@g++ -c src/main.cpp -std=c++14 -m64 -O3 -Wall -I include
	@g++  main.o -o bin/release/main -s -lSDL2main -lSDL2 -lSDL2_image
	@./bin/release/main
raycast:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/raycast.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ raycast.o -o bin/debug/raycast -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/raycast
projector:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/projector.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ projector.o -o bin/debug/projector -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/projector
forces:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/forces.cpp src/Utilities.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ forces.o Utilities.o -o bin/debug/forces -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	@./bin/debug/forces

tileset:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/tileset.cpp -std=c++17 -m64 -g -Wall -I include -I bengine -I btils
	@g++ tileset.o -o bin/debug/tileset -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	@./bin/debug/tileset
raycaster:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/raycaster.cpp -std=c++17 -m64 -g -Wall -I include -I bengine -I btils
	@g++ raycaster.o -o bin/debug/raycaster -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	@./bin/debug/raycaster
physics:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/physics.cpp -std=c++17 -m64 -g -Wall -I include -I bengine -I btils
	@g++ physics.o -o bin/debug/physics_sim -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	@./bin/debug/physics_sim
thingy:
	@mkdir bin -p
	@mkdir bin/debug -p
	@g++ -c src/thingy.cpp -std=c++17 -m64 -g -Wall -I include -I bengine -I btils
	@g++ thingy.o -o bin/debug/thingy -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	@./bin/debug/thingy
