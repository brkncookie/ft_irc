CFLAGS = -Wall -Werror -Wextra -std=c++98
CC = c++
SRCS = srcs/Server.cpp srcs/Channel.cpp srcs/User.cpp srcs/main.cpp
OBJS = $(addprefix objs/, $(SRCS:.cpp=.o))
RM = rm -rf
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

objs/%.o: %.cpp include/Server.hpp include/User.hpp include/Channel.hpp
	mkdir -p objs/srcs
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) objs

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all
