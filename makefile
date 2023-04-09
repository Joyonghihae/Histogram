
# target: dependency1 dependency 2...
#	command required to achieve the target

./bin/DP-1 : ./obj/dp-1.o
	cc ./obj/dp-1.o -o ./bin/DP-1

./bin/DP-2 : ./obj/dp-2.o
	cc ./obj/dp-2.o -o ./bin/DP-2

./bin/DC : ./obj/dc.o
	cc ./obj/dc.o -o ./bin/DC
	

./obj/dp-1.o : ./src/dp-1.c ./inc/header.h
	cc -c ./src/dp-1.c -o ./obj/dp-1.o


./obj/dp-2.o : ./src/dp-2.c ./inc/header.h
	cc -c ./src/dp-2.c -o ./obj/dp-2.o
	

./obj/dc.o : ./src/dc.c ./inc/header.h
	cc -c ./src/dc.c -o ./obj/dc.o


# Other Targets

all: ./bin/DP-1 ./bin/DP-2 ./bin/DC
	

clean:
	rm ./bin/*
	rm ./obj/*
