#! /bin/sh
# Tests conversion of gmx files
#
# Copyright (C) 2008-2009 Dominik Köppl
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.


if [[ $# -ne 2 ]]; then 
	echo "Usage: $0 program test"
	exit 0
fi

program="$1"
testpath='tests'
tmpfile=`mktemp`
trap 'rm -fr $tmpfiles' 1 2 3 15

case $2 in
0)
	infile=gmx.gmxa
	checkfile=gmx.ok
	;;
1)
	infile=outlook.csv
	checkfile=outlook.ok
	;;
2)
	infile=vcard.vcf
	checkfile=vcard.ok
	;;
*)
	echo "Unknown parameter : $2"
	exit 1
esac
./$program -i "$testpath/$infile" -o "$tmpfile"
diff "$testpath/$checkfile" "$tmpfile"
result=$?
rm -fr $tmpfiles
exit $result
