#!/bin/sh

cat <<HEADER
<html>
<head>
<meta charset="utf-8">
HEADER

while [ "$1" = "-s" ]; do
	echo '<link rel="stylesheet" type="text/css" href="'"$2"'" >'
	shift 2
done

cat <<HEADER
</head>
<body>
<div>
HEADER

pulldown-cmark

cat <<FOOTER
</div>
</body>
</html>
FOOTER