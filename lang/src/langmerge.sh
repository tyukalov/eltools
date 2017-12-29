#!/bin/sh
msgmerge --backup=none -U eltools.po $1.po
rm $1.po $1.pot
