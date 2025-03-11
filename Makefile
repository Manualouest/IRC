NAME = ircserv

INCLUDES = -Iincludes/

FLAGS = -g -Werror -Wall -Wextra -std=c++98

SRC = src/Server.cpp\
		src/Commands.cpp\
		src/ChannelCommands.cpp\
		src/Utils.cpp

OBJ = $(addprefix obj/, $(SRC:.cpp=.o))

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	c++ $(FLAGS) $(INCLUDES) $< -c -o $@

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(FLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

clean:
	rm -rf obj/

fclean: clean
	rm -rf $(NAME)

re: fclean all