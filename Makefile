CPP= c++
FLAGS= -Wall -Werror -Wextra
SRC= server/Server.cpp main.cpp server/clients.cpp Client.cpp
OBJ= $(SRC:.cpp=.o)
HEADERS= headers/Server.hpp headers/Client.hpp
NAME= ft_irc

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