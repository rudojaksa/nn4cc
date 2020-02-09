OBJ = ../../src/aux.o

f2s: f2s.c $(OBJ) Makefile
	gcc -O0 -g -I../../src -o $@ $< $(OBJ)

%.o: %.c Makefile
	gcc -O0 -g -I../../src -c -o $@ $<

clean:
	rm -f f2s

