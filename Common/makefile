# FILE		: makefile
# PROJECT	: HISTO-SYSTEM
# PROGRAMMER	: Eunyoung Kim, Yujin Jeong
# FIRST VERSION	: 2023-04-10
# DESCRIPTION	: This makefile is to compile and link the common source modules


# DEPENDENCIES
./obj/common.o: ./src/common.c ./inc/common.h
	cc -c ./src/common.c -o ./obj/common.o

# OTHER TARGETS

clean:
	rm ./bin/*
	rm ./obj/*
