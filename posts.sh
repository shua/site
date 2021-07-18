#!/bin/sh

title() {
	title=$(grep '<pmeta id="title">' "$1" |sed 's/<[^>]*>//g')
	title=${title:-$(grep '<title>' "$1" |sed 's/<[^>]*>//g')}
	if [ -n "$title" ]; then
		echo "$title"
	else
		echo "$1" |sed 's/.md//g; s/^........-//g'
	fi
}

html() {
	echo "$1" |sed 's/\.md/.html/'
}

cat <<HEADER
Some blog posts
===============

HEADER

while read post; do
	echo "- [$(title "$post")]($(html $post))"
done

