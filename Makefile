# This Makefile is for the simple c test program included with pyquadtree.  Useful for
# checking to see if the c code passes through valgrind, etc...

qt: qt.c quadtree.c quadtree.h
	gcc -o qt qt.c quadtree.c -std=c99 -lm
