#!/bin/sh
xgettext --keyword=_ --language=C --add-comments --sort-output -o $1.pot ../../src/*.c ../../src/scc/*.c
msginit --input=$1.pot --locale=ru --output=$1.po
