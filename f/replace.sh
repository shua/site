#!/bin/sh

cat ../$1.css  \
|grep 'url\|/\*\|weight' \
|sed 's/.*url(//; s/).*//; s#/\* ##; s# \*/##; s/ *font-weight: *//; s/;$//' \
|paste - - - \
|while read l; do \
	name=$1_$(echo "$l" |cut -f1,2 |tr '\t' '_').woff2; \
	url=$(echo "$l" |cut -f3); \
	(set -x; curl -s "$url" >$name); \
	echo "$url" "/f/$name" >>replace.txt; \
done
