NAME=webserv

CC = clang++

# TODO:: remove -g flag
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

SRC = srcs/main.cpp \
	srcs/server/Server.cpp \
	srcs/server/Socket.cpp \
	srcs/config/Config.cpp \
	srcs/config/Configurator.cpp \
	srcs/utils/utils.cpp \
	srcs/repository/Repository.cpp \
	srcs/model/WebClient.cpp \
	srcs/model/Request.cpp \
	srcs/model/Response.cpp \
	srcs/requestParser/RequestParser.cpp \
	srcs/requestHandler/RequestHandler.cpp \
	srcs/requestHandler/FileReader.cpp \

OBJ=$(SRC:.cpp=.o)

HEADERS = \
	srcs/server/Server.hpp \
	srcs/server/Socket.hpp \
	srcs/config/Config.hpp \
	srcs/config/Configurator.hpp \
	srcs/utils/utils.hpp \
	srcs/repository/Repository.hpp \
	srcs/interfaces/ICrudRepository.hpp \
	srcs/model/WebClient.hpp \
	srcs/interfaces/IEntity.hpp \
	srcs/model/Request.hpp \
    srcs/model/Response.hpp \
    srcs/requestParser/RequestParser.hpp \
    srcs/requestHandler/RequestHandler.hpp \
    srcs/requestHandler/FileReader.hpp \


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
