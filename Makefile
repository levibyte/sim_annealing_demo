default:
	rm -rf ztest
	g++ -Wfatal-errors logic.cpp main.cpp -g -I ./deps/SDL2_ttf-2.0.14 -I ./deps/SDL2-2.0.5/include/ -I ./deps/SDL2_image-2.0.1/ -L  ./deps/SDL2_image-2.0.1/lib/ -L ./deps/SDL2_ttf-2.0.14/.libs -L ./deps/SDL2-2.0.5/lib -lSDL2_ttf -lSDL2 -lSDL2_image -o ztest