# FILE		: makefile
# PROJECT	: HISTO-SYSTEM
# PROGRAMMER	: Eunyoung Kim, Yujin Jeong
# FIRST VERSION	: 2023-03-18
# DESCRIPTION	: This makefile compile and link HISTO-SYSTEM system project

Histogram-main := Common DC DP-1 DP-2

all:
	@$(foreach dir,$(Histogram-main),$(MAKE) -C $(dir);)

clean:
	@$(foreach dir,$(Histogram-main),$(MAKE) -C $(dir) clean;)
