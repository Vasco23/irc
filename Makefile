NAME = irc
SRC_NAME = main.cpp ./server/server.cpp ./server/clients.cpp ./server/channel.cpp\
			./server/commands/pass.cpp ./server/commands/nick.cpp ./server/commands/join.cpp ./server/commands/user.cpp ./server/commands/part.cpp \
			./server/commands/privmsg.cpp ./server/commands/topic.cpp ./server/commands/kick.cpp ./server/commands/invite.cpp ./server/commands/mode.cpp\
			./server/commands/quit.cpp ./server/commands/who.cpp ./server/bot.cpp

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98 #-fsanitize=address
RM = rm -f

#$(VERBOSE).SILENT:
INC = -Iinc

SRC_PATH = ./src

OBJ_PATH = ./objects

OBJS = $(addprefix $(OBJ_PATH)/, $(SRC_NAME:.cpp=.o))

SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))

all: $(NAME)

$(NAME) : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(INC) -o $(NAME)
	clear

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	mkdir -p objects
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@
	@printf "                                                \n"
	@printf " ********************************************** \n"
	@printf " ********* Very good, Very nice!!! ************ \n"
	@printf " ********************************************** \n"
	@printf "                                                \n"

clean:
	$(RM)	$(SRC:=.o)
	clear
	@echo "                                                "
	@echo " ********************************************** "
	@echo " **********  So Clean It Shines!  ************* "
	@echo " ********************************************** "
	@echo "                                                "

fclean: clean
	$(RM)	$(NAME)

re:	fclean	$(NAME)