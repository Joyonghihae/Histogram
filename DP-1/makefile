# FILE		: makefile
# PROJECT	: HISTO-SYSTEM
# PROGRAMMER	: Eunyoung Kim, Yujin Jeong
# FIRST VERSION	: 2023-04-10
# DESCRIPTION	: This makefile is to compile and link DP-1 application


# FINAL BINARY TARGET
./bin/DP-1.o: ./obj/DP-1.o 
	cc ./obj/DP-1.o ../Common/obj/common.o -o ../Common/bin/DP-1 -lncurses -lpthread

# DEPENDENCIES
./obj/DP-1.o: ./src/dp-1.c ./inc/dp-1.h
	cc -c ./src/dp-1.c -o ./obj/DP-1.o


# OTHER TARGETS
all: ./bin/DP-1

clean:
	rm ./obj/*
