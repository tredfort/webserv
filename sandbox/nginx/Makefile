all: build
	docker run -it --rm -p 8080:80 -p 8081:81 --name web webserver

build:
	docker build -t webserver .
