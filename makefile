default:
	@mkdir bin/debug -p
	@g++ -c src/*.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ RenderWindow.o main.o -o bin/debug/main -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/main
debug:
	@mkdir bin/debug -p
	@g++ -c src/*.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ RenderWindow.o main.o -o bin/debug/main -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/main
release:
	@mkdir bin/release -p
	@g++ -c src/*.cpp -std=c++14 -m64 -O3 -Wall -I include
	@g++ RenderWindow.o main.o -o bin/release/main -s -lSDL2main -lSDL2 -lSDL2_image
	@./bin/release/main
topdown:
	@mkdir bin/debug -p
	@g++ -c src/*.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ RenderWindow.o topdown.o -o bin/debug/topdown -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/topdown
sidescroll:
	@mkdir bin/debug -p
	@g++ -c src/*.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ RenderWindow.o sidescroll.o -o bin/debug/sidescroll -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/sidescroll
raycast:
	@mkdir bin/debug -p
	@g++ -c src/*.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ RenderWindow.o raycast.o -o bin/debug/raycast -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/raycast
