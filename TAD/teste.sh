gcc -c intercodeglobals.c 
gcc main.c -o teste intercodeglobals.o -lm
./teste