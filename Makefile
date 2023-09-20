ipli: ipli.c list.c functions.c
	gcc -c ipli.c
	gcc -c functions.c
	gcc -c list.c
	gcc -o ipli ipli.o list.o functions.o