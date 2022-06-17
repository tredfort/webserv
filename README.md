# webserv

Implementation of web server with c++ with school 42

## How to Start

build:

```bash
make
```

for execute, you are able to specify config file, or leave it empty to start with standard config

```bash
./webserv [pathToConfigFile]
```

## How to test with nginx

Just go to the sandbox folder and do "make" with Makefile :D

There is **nginx.conf** file which you're able to edit and html folder with content, this folder copies to the
container.

## Architecture notes

Find it here:

https://github.com/tredfort/webserv/wiki/Architecture-overview