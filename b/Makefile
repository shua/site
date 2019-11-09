
%.html: %.md
	./render.sh -s "post.css" <$< >$@

index.html: index.md
	./render.sh -s "/menu.css" <$< >$@

.PHONY: clean
clean:
	rm -f *.html
