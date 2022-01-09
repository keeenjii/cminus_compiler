
flex scanner.l
bison -d -v scanner.y

gcc -c *.c
gcc -o scanner *.o -lm
./scanner entrada6.txt
python binGenerator.py

rm scanner.tab.c
rm scanner.tab.o
rm main.o
rm scanner.tab.h
rm lex.yy.o
rm lex.yy.c
rm scanner.output
rm scanner
rm util.o
rm analyze.o
rm cgen.o
rm symtab.o
rm intercodeglobals.o
rm ass.o
rm assGen.o

