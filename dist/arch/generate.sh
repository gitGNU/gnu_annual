#!/bin/sh
d=`mktemp -d`
file='PGKBUILD'
cp $file $d/
cd $d
makepkg &&
makepkg -i
