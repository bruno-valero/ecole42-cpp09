NAME = RPN
CC = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3

FILES = main.cpp RPN.cpp
OBJS = $(FILES:%.cpp=%.o)

# FLAGS

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
