
flex scanner.l
bison -d -v scanner.y

gcc -c *.c
gcc -o scanner *.o 
./scanner entrada2.txt

rm scanner.tab.c
rm scanner.tab.o
rm main.o
rm scanner.tab.h
rm lex.yy.o
rm lex.yy.c
rm scanner.output
rm scanner
rm util.o
