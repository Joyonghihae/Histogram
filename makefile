
# FILE		: makefile
# PROJECT	: HISTO-SYSTEM
# PROGRAMMER	: Eunyoung Kim, Yujin Jeong
# FIRST VERSION	: 2023-04-10
# DESCRIPTION	: This makefile is to compile and link the common source modules

HISTOSYSTEM := Common DC DP-1 DP-2

all:
	@$(foreach dir,$(CANWETALK),$(MAKE) -C $(dir);)

clean:
	@$(foreach dir,$(CANWETALK),$(MAKE) -C $(dir) clean;)
