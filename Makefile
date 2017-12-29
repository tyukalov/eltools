 #    This program is free software: you can redistribute it and/or modify
 #    it under the terms of the GNU General Public License as published by
 #    the Free Software Foundation, either version 3 of the License, or
 #    (at your option) any later version.

 #    This program is distributed in the hope that it will be useful,
 #    but WITHOUT ANY WARRANTY; without even the implied warranty of
 #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #    GNU General Public License for more details.

 #    You should have received a copy of the GNU General Public License
 #    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 # Igor Tyukalov <tyukalov@bk.ru> 

SRC = src
LIB = lib
BIN = bin
PWD = $(shell pwd)
MLANG = $(PWD)/lang

all: omc vl380c libomc motor scc 

PHONY: all omc vl380c libomc motor scc xmlinit scctest omtest tags lang langmerge langinst

omc:
ifeq ($(CC), cl)
	$(CC)  $(SRC)/om.c $(SRC)/libom.c $(LIB)/libargparser.lib 
	copy /Y om.exe $(BIN)
	del *.obj *.exe
else
	$(CC) $(debug) -lm -DUSE_GETTEXT=\"$(PWD)/lang\" $(SRC)/om.c $(SRC)/libom.c $(LIB)/libargparser.a -o om
	mv om ./$(BIN)
endif

vl380c:
	$(CC) $(SRC)/vl380.c $(LIB)/libcsvdblt.a -o vl380
	mv vl380 ./$(BIN)


libomc:
	$(CC) -c $(SRC)/libom.c
	ar rc libom.a libom.o
	mv libom.a ./$(LIB)
	rm libom.o


motor:
	$(CC) $(DEBUG) -lm -DUSE_GETTEXT=\"$(PWD)/lang\" $(SRC)/motor.c $(LIB)/libargparser.a -o motor
	mv motor ./$(BIN)


scc: xmlinit
ifeq ($(CC), cl)
	$(CC) $(SRC)/scc/scc.c $(LIB)/libargparser.lib $(SRC)/scc/libnet.c $(LIB)/init.lib
	copy /Y scc.exe $(BIN)
	del *.obj scc.exe
else
	cp $(BIN)/libinit.so ./
	$(CC) $(DEBUG) -c  -DUSE_GETTEXT=\"$(PWD)/lang\" $(SRC)/scc/scc.c $(SRC)/scc/libnet.c 
	$(CC) scc.o libnet.o $(LIB)/libargparser.a -L. -linit -o scc -lm -lxml2
	rm *.o
	rm libinit.so
	mv scc ./$(BIN)
endif

xmlinit:
ifeq ($(CC), cl)
	$(CC) /DWIN $(SRC)/scc/xmlinit.c $(LIB)/iconv.lib $(LIB)/libxml2.lib /LD /o init.dll
	copy /Y init.dll $(BIN)\init.dll
	copy /Y init.lib $(LIB)\init.lib
	del *.obj *.exp *.dll *.lib
else
	$(CC) $(DEBUG) -fPIC -c -lxml2 -lm $(SRC)/scc/xmlinit.c
	$(CC) -shared -o libinit.so xmlinit.o
	rm xmlinit.o
	mv libinit.so $(BIN)
endif


scctest: 
ifeq ($(OS), win)
	$(BIN)/scc.exe -m=3 test/test.xml point posttrans
else
	@LD_LIBRARY_PATH=$(BIN) $(BIN)/scc -m=3 test/test.xml point posttrans
endif

omtest: omc
	./test/om.sh makesr

tags:
	find src/ include/ -name "*.[ch]" | etags -

langc:
	xgettext --keyword=_ --language=C --add-comments --sort-output -o lang/src/$(name).pot src/*.c src/scc/*.c
	msginit --input=lang/src/$(name).pot --locale=ru --output=lang/src/$(name).po

langinst:
	msgfmt --output-file=lang/ru/LC_MESSAGES/eltools.mo lang/src/eltools.po

langmerge:
	msgmerge --backup=none -U lang/src/eltools.po lang/src/$(name).po
	rm lang/src/$(name).po lang/src/$(name).pot
