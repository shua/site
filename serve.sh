#!/bin/sh
docker run --rm -it -v $PWD:/usr/share/nginx/html:ro nginx
