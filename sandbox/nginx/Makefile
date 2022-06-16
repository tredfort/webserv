all: build
	docker run -it --rm -p 8080:80 --name web webserver

build:
	docker build -t webserver .
