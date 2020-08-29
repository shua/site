CSS="/menu.css"
POSTS=2018/*.md 2019/*.md 2020/*.md


.PHONY: all
all: index.html 404.html posts.html feed.xml

index.html: index.md
	./render.sh -t person -s $(CSS) <$< >$@
404.html: 404.md
	./render.sh -s $(CSS) <$< >$@

%.html: %.md
	./render.sh -t post -s $(CSS) <$< >$@

posts.html: $(POSTS)
	make -e CSS="/post.css" $$(ls $(POSTS) |sed 's/.md/.html/')
	ls $(POSTS) |sort -r |./posts.sh |./render.sh -s "/menu.css" >$@

feed.xml: $(POSTS)
	./feed.sh >$@

.PHONY: clean
clean:
	rm -f *.html {2018,2019,2020}/*.html feed.xml
	rm -rf out

out:
	mkdir -p out
	find . -path './20??*' -type d -exec mkdir -p out/{} \;
	find . \( \( -path './out*' -o -path './draft*' \) -prune -o -name '*.html' \) -type f -exec cp {} out/{} \;
	cp feed.xml out/
