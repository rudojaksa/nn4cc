T    := erplot
PATH := $(PATH):../UTIL
SRC  := $(T:%=%.pl)

all: $T

PACKAGE  := $(shell (cd ..; getversion -pkg))
VERSION  := $(shell (cd ..; getversion -ver))
COPYLEFT := $(shell (cd ..; getversion -cpy))
MKDATE	 := $(shell date '+%Y%m%d.%H%M')
MKHOST	 := $(shell hostname)
SUBVERSION := $(shell (cd ..; mkdist -sub))

$T: %: %.pl erplot.gp.pl *.pl | CONFIG.pl
	perlpp $< > $@
	@chmod 755 $@

CONFIG.pl: $(SRC) Makefile
	@echo "create $@ for $(PACKAGE)-$(VERSION).$(SUBVERSION)"
	@echo "\$$PACKAGE = \"$(PACKAGE)\";" > $@
	@echo "\$$VERSION = \"$(VERSION)\";" >> $@
	@echo "\$$SUBVERSION = \"$(SUBVERSION)\";" >> $@
	@echo "\$$MKDATE = \"$(MKDATE)\";" >> $@
	@echo "\$$MKHOST = \"$(MKHOST)\";" >> $@
	@echo "\$$COPYLEFT = \"$(COPYLEFT)\";" >> $@

erplot.gp.pl: erplot.gp
	echo -e "\$$ERPLOT=<<EOF;\n" > $@
	cat $< >> $@
	echo -e "EOF\n" >> $@

install: all
	makeinstall -f $T

clean:
	rm -f erplot.gp.pl
	rm -f $T

