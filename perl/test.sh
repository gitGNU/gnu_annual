#! /bin/sh
#
# Tests conversion of various files with 2annual
# part of annual - Reminder for annual events
# Keeps track of all your anniversaries and hopefully reminds you at the right time.
# Copyright (C) 2011 Dominik Köppl
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

if [[ $# -ne 2 ]]; then 
	echo "Usage: $0 program test"
	exit 0
fi

program="$1"
testpath="`dirname $0`/tests"
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
$program -i "$testpath/$infile" -o "$tmpfile"
diff "$testpath/$checkfile" "$tmpfile"
result=$?
rm -fr $tmpfiles
exit $result
