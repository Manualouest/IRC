NAME = ircserv

INCLUDES = -Iincludes/

FLAGS = -g -Werror -Wall -Wextra -std=c++98

SRC = src/Server.cpp\
		src/ChannelCommands.cpp\
		src/Utils.cpp\
		src/Commands/Join.cpp\
		src/Commands/User.cpp\
		src/Commands/Pass.cpp\
		src/Commands/Nick.cpp\
		src/Commands/Utils.cpp\
		src/Commands/Privmsg.cpp\
		src/Commands/Part.cpp\
		src/Commands/Quit.cpp\
		src/Commands/Mode.cpp\
		src/Commands/Invite.cpp\
		src/Commands/Topic.cpp

OBJ = $(addprefix obj/, $(SRC:.cpp=.o))

BSRC = src/bot/bot.cpp

BOBJ = $(addprefix obj/, $(BSRC:.cpp=.o))

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	c++ $(FLAGS) $(INCLUDES) $< -c -o $@

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(FLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

bot: $(BOBJ)
	c++ $(FLAGS) $(BOBJ) -o bot

clean:
	rm -rf obj/

fclean: clean
	rm -rf $(NAME)
	rm -rf bot

re: fclean all