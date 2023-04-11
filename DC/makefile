# FILE		: makefile
# PROJECT	: HISTO-SYSTEM
# PROGRAMMER	: Eunyoung Kim, Yujin Jeong
# FIRST VERSION	: 2023-04-10
# DESCRIPTION	: This makefile is to compile and link DC application


# FINAL BINARY TARGET
./bin/DC.o: ./obj/DC.o 
	cc ./obj/DC.o ../Common/obj/common.o -o ../Common/bin/DC
# DEPENDENCIES
./obj/DC.o: ./src/dc.c ./inc/dc.h
	cc -c ./src/dc.c -o ./obj/DC.o


# OTHER TARGETS
all: ./bin/DC

clean:
	rm ./obj/*
