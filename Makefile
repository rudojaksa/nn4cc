ALL := erplot/erplot doc/errors.pdf src/nntrain

   all: $(ALL)
.PHONY: $(ALL)

src/nntrain:
	make -C src

erplot/erplot:
	make -C erplot

doc/errors.pdf:
	make -C doc

# TODO: nntrain should be installed as a symbolic link to nntrain in this directory
#       to stay allways freshly recompiled to the latest topology

install: all
	makeinstall -f src/nntrain erplot/erplot

clean:
	make -C erplot clean
	make -C doc clean
	make -C src clean

README.md: src/help.txt
	src/nntrain -h | man2md > $@

-include ~/.github/Makefile.git

