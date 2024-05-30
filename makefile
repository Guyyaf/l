labb: labb.o 
	gcc -m32 -g -Wall -o labb labb.o

labb.o: labB.c 
	gcc -g -Wall -m32 -c -o labb.o labB.c 

.PHONY: clean 

clean:
	rm -f *.o labb