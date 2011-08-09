#!/bin/sh
function makeList()
{
	echo "SET(annual_SRCS `ls *.cpp | tr '\n' ' '`) " 
	echo "SET(annual_RES_SRC `ls *.qrc  | tr '\n' ' '`)" 
	echo -n "SET(annual_MOC_H "
	for i in *.h; do
		[[ -z `grep Q_OBJECT $i` ]] && continue
		echo -n "$i "
	done
	echo ")"

}

makeList > CMakeListsFiles.txt
