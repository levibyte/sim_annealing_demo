default:
	rm -rf ztest
	g++ test.cpp -I /home/levibyte/Downloads/SDL2-2.0.5/include/ -I /home/levibyte/Downloads/SDL2_image-2.0.1/ -L  /remote/am04home1/levons/levi/downloads/SDL2_image-2.0.1/lib/ -L /remote/am04home1/levons/levi/downloads/SDL2-2.0.5/lib -lSDL2 -lSDL2_image -o ztest