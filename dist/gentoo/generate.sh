#!/bin/sh
d=`mktemp -d`
fd="$d/app-office/annual"
ebuild='annual-9999.ebuild'
mkdir -p "$fd" &&
cp $ebuild "$fd/" &&
cd "$fd/" &&
ebuild $ebuild manifest &&
ebuild $ebuild merge
