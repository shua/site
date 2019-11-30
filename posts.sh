#!/bin/sh

title() {
	title=$(head "$1" |grep '<pmeta id="title">' |sed 's/<[^>]*>//g')
	if [ -n "$title" ]; then
		echo "$title"
	else
		echo "$1" |sed 's/.md//g; s/^........-//g'
	fi
}

html() {
	echo "$1" |sed 's/.md/.html/'
}

cat <<HEADER
Some blog posts
===============

HEADER

while read post; do
	echo "- [$(title "$post")]($(html $post))"
done

