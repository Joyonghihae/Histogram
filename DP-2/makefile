# FILE		: makefile
# PROJECT	: HISTO-SYSTEM
# PROGRAMMER	: Eunyoung Kim, Yujin Jeong
# FIRST VERSION	: 2023-04-10
# DESCRIPTION	: This makefile is to compile and link DP-2 application


# FINAL BINARY TARGET
./bin/DP-2.o: ./obj/DP-2.o 
	cc ./obj/DP-2.o ../Common/obj/common.o -o ../Common/bin/DP-2 

# DEPENDENCIES
./obj/DP-2.o: ./src/dp-2.c
	cc -c ./src/dp-2.c -o ./obj/DP-2.o


# OTHER TARGETS
all: ./bin/DP-2

clean:
	rm ./obj/*
