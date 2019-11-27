#!/bin/sh

usage() {
	echo "usage: render.sh [ -s CSS ]*"
}

title=""
created=""

meta() {
	echo "$1" |sed 's/<[^>]*>//g'
}

while { if [ -z "$nometa" ]; then read line; else false; fi; } do
	case "$line" in
		'<pmeta id="title">'*) title=$(meta "$line") ;;
		'<pmeta id="created">'*) created=$(meta "$line") ;;
		"<pmeta"*) echo ">> some other pmeta: $(meta "$line")" >&2 ;;
		*) nometa=1;;
	esac
done


cat <<HEADER
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<link href="https://fonts.googleapis.com/css?family=Nunito:600,800&display=swap&subset=cyrillic,latin-ext" rel="stylesheet"> 
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

[ -n "$created" ] && echo "<header>$created</header>"
[ -n "$title" ] && echo "<h1>&gt;&nbsp;$title</h1>"

(echo "$line"; cat) |pulldown-cmark

cat <<FOOTER
</div>
</body>
</html>
FOOTER