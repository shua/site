CSS="/menu.css"
POSTS={2019,2018}/*.md


.PHONY: all
all: index.html 404.html posts.html

%.html: %.md
	./render.sh -s $(CSS) <$< >$@

posts.html: 2018/*.md 2019/*.md
	make -e CSS="/post.css" $$(ls $(POSTS) |sed 's/.md/.html/')
	ls $(POSTS) |sort -r |./posts.sh |./render.sh -s "/menu.css" >$@

.PHONY: clean
clean:
	rm -f *.html {2018,2019}/*.html
