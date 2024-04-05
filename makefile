default:
	@mkdir bin/debug -p
	@g++ -c src/main.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ main.o -o bin/debug/main -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/main
debug:
	@mkdir bin/debug -p
	@g++ -c src/main.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ main.o -o bin/debug/main -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/main
release:
	@mkdir bin/release -p
	@g++ -c src/main.cpp -std=c++14 -m64 -O3 -Wall -I include
	@g++  main.o -o bin/release/main -s -lSDL2main -lSDL2 -lSDL2_image
	@./bin/release/main
raycast:
	@mkdir bin/debug -p
	@g++ -c src/raycast.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ raycast.o -o bin/debug/raycast -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/raycast
gtest:
	@mkdir bin/debug -p
	@g++ -c src/graphTesting.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ graphTesting.o -o bin/debug/graphTesting
	@./bin/debug/graphTesting
thingy:
	@mkdir bin/debug -p
	@g++ -c src/thingy.cpp -std=c++14 -m64 -g -Wall -I include
	@g++ thingy.o -o bin/debug/thingy -lSDL2main -lSDL2 -lSDL2_image
	@./bin/debug/thingy