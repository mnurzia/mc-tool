SOURCES = main.cpp Biome.cpp Layer.cpp Map.cpp OceanGen.cpp Generator.cpp
OLDSOURCES = cubiomes/finders.c cubiomes/generator.c cubiomes/layers.c
CFLAGS = -g -Wall -fsanitize=address

all:
	gcc $(CFLAGS) -c cubiomes/finders.c
	gcc $(CFLAGS) -c cubiomes/generator.c
	gcc $(CFLAGS) -c cubiomes/layers.c
	g++ $(CFLAGS) -c main.cpp
	g++ $(CFLAGS) -c Biome.cpp
	g++ $(CFLAGS) -c Layer.cpp
	g++ $(CFLAGS) -c Map.cpp
	g++ $(CFLAGS) -c OceanGen.cpp
	g++ $(CFLAGS) -c Generator.cpp -o generator_cpp.o
	gcc finders.o generator.o layers.o main.o Biome.o Layer.o Map.o OceanGen.o generator_cpp.o -lasan -lstdc++ -lm -o main 

clean:
	rm -rf *.o
