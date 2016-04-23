all: Load Change Print Query Clean

Load: Load.c header.h 
	gcc -o Load Load.c header.c
Change: Change.c header.h
	gcc -o Change Change.c header.c
Print: Print.c header.h
	gcc -o Print Print.c header.c
Query: Query.c header.h
	gcc -o Query Query.c header.c
Clean: Clean.c header.h
	gcc -o Clean Clean.c header.c

clean:
	rm -f Load  Change  Print Query Clean *~ core
