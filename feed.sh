#!/bin/sh

meta() {
	grep '<pmeta id="'"$1"'">' \
	|sed 's/<[^>]*>//g'
}

posts=$(ls $PWD/20??/*.md |sort -r |sed "s#^$PWD/##")
updated=$(cat $posts |meta '\(created\|updated\)' |sort -r |head -n1)

cat <<EOF
<?xml version="1.0" encoding="utf-8"?>
<feed xmlns="http://www.w3.org/2005/Atom">
	<title>is this a blog</title>
	<subtitle>it is</subtitle>
	<link href="http://isthisa.blog"/>
	<updated>$updated</updated>
	<author>
		<name>JD Lloret</name>
		<email>itis@isthisa.email</email>
	</author>
	<id>tag:isthisa.website,2018:feed</id>
EOF

for post in $posts; do
	title=$(meta title <$post)
	updated=$(meta updated <$post)
	if [ -z "$updated" ]; then updated=$(meta created <$post); fi
	summary=$(
		<$post grep -v "<pmeta" \
		|pulldown-cmark \
		|sed 's#</*p>#\n&\n#g' \
		|awk '
			BEGIN   {n=0; p=0}
			/<p>/   {p=p+1; if(p==1){n=n+1}}
			n==1&&p>0 {print $0}
			/<\/p>/ {p=p-1}')
	post=$(echo "$post" |sed 's/\.md$//')

cat <<ENTRY
	<entry>
		<title>$title</title>
		<link href="https://isthisa.website/$post.html"/>
		<id>tag:isthisa.website,2018:$post</id>
		<updated>$updated</updated>
		<summary>
$summary
		</summary>
	</entry>
ENTRY
done

echo "</feed>"
