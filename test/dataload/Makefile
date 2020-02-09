OBJ = ../../data.o ../../files.o ../../aux.o

dataload: dataload.c $(OBJ) Makefile
	gcc -O0 -g -o $@ $< $(OBJ)

%.o: %.c Makefile
	gcc -O0 -g -c -o $@ $<

clean:
	rm -f dataload

