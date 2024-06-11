AntiVirus: AntiVirus.o 
	gcc -m32 -g -Wall -o AntiVirus AntiVirus.o

AntiVirus.o: AntiVirus.c 
	gcc -g -Wall -m32 -c -o AntiVirus.o AntiVirus.c 

.PHONY: clean 1

clean:
	rm -f *.o AntiVirus