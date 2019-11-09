
.PHONY: all
all:
	make $$(ls ????????-*.md |sed 's/.md/.html/')
	./index.sh >index.md
	make index.html

%.html: %.md
	./render.sh -s "post.css" <$< >$@

index.html: index.md
	./render.sh -s "/menu.css" <$< >$@

.PHONY: clean
clean:
	rm -f *.html
