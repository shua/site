CSS="/menu.css"

.PHONY: all
all: index.html 404.html b/index.html

%.html: %.md
	./render.sh -s $(CSS) <$< >$@

b/index.html: b/????????-*.md
	make -e CSS="/post.css" $$(ls b/????????-*.md |sed 's/.md/.html/')
	{ cd b; ./index.sh; } |./render.sh -s "/menu.css" >$@

.PHONY: clean
clean:
	rm -f *.html b/*.html
