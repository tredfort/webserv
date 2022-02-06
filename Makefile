NAME=webserv

CC = clang++

# TODO:: remove -g flag
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

SRC = srcs/main.cpp \
	srcs/server/Server.cpp \
	srcs/server/Socket.cpp \
	srcs/config/Config.cpp \
	srcs/config/Configurator.cpp \
	srcs/model/Request.cpp \
	srcs/model/Response.cpp \
	srcs/repository/Repository.cpp \
	srcs/model/WebClient.cpp \
	srcs/utils/utils.cpp \

OBJ=$(SRC:.cpp=.o)

HEADERS = \
	srcs/server/Server.hpp \
	srcs/server/Socket.hpp \
	srcs/config/Config.hpp \
	srcs/config/Configurator.hpp \
	srcs/model/Request.hpp \
    srcs/model/Response.hpp \
    srcs/repository/Repository.hpp \
    srcs/model/WebClient.hpp \
	srcs/utils/utils.hpp \

srcs/%.o: srcs/%.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ) $(HEADERS)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

format:
	find . -iname *.hpp -o -iname *.cpp | xargs clang-format -i -style=file

re: fclean all
