#!/bin/sh

usage() {
	echo "usage: render.sh [ -s CSS ]*"
}

title=""
created=""

meta() {
	echo "$1" |sed 's/<[^>]*>//g'
}

prettydate() {
	if [ $# -lt 1 ]; then read date; else date=$1; fi
	y=$(echo "$date" |cut -d'-' -f1)
	m=$(echo "$date" |cut -d'-' -f2)
	d=$(echo "$date" |cut -d'-' -f3)
	m=$(printf "Jan\nFeb\nMar\nApr\nJun\nJul\nAug\nSep\nOct\nNov\nDec\n" |sed -n ${m}p)
	d=$(echo "$d" |sed 's/^0//')
	echo "$y $m $d"
}

while { if [ -z "$nometa" ]; then read line; else false; fi; } do
	case "$line" in
		'<pmeta id="title">'*) title=$(meta "$line") ;;
		'<pmeta id="created">'*) created=$(meta "$line" |prettydate) ;;
		'<pmeta id="updated">'*) updated=$(meta "$line" |prettydate) ;;
		"<pmeta"*) echo ">> some other pmeta: $(meta "$line")" >&2 ;;
		*) nometa=1;;
	esac
done

cat <<HEADER
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<link href="/f/Nunito.css" rel="stylesheet">
<link href="/feed.xml" type="application/atom+xml" rel="alternate" title="Blog Atom feed" />
HEADER

while [ $# -gt 0 ]; do
	case "$1" in
	"-s")
		echo '<link rel="stylesheet" type="text/css" href="'"$2"'" >'
		shift 2
		;;
	*)
		echo "Unrecognized option $1" >&2
		usage
		exit 1
		;;
	esac
done

[ -n "$title" ] && echo "<title>$title</title>"

cat <<HEADER
</head>
<body>
<div>
HEADER

echo "<header>"
if [ -n "$created" ]; then
	echo -n "	<created>$created"
	[ -n "$updated" ] && echo -n "<br/><i>updated: $updated</i>"
	echo "</created>"
fi
[ -n "$title" ] && echo "	<ptitle><h1>&gt;&nbsp;$title</h1></ptitle>"
echo "</header>"

(echo "$line"; cat) |pulldown-cmark

cat <<FOOTER
</div>
</body>
</html>
FOOTER