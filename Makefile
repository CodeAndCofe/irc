CPP= c++
FLAGS= -Wall -Werror -Wextra #-fsanitize=address -g3
SRC= src/tool.cpp src/Server.cpp src/main.cpp src/mode.cpp src/clients.cpp src/Client.cpp src/join.cpp src/Channel.cpp src/kick.cpp src/topic.cpp src/invite.cpp src/privmsg.cpp
OBJ= $(SRC:.cpp=.o)
HEADERS= headers/Server.hpp headers/Client.hpp headers/Channel.hpp headers/RepErr.hpp 
NAME= ircserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all