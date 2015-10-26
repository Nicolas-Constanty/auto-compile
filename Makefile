##
## Makefile for  in /home/babiole/Documents/auto-compile
##
## Made by Babiole,,,
## Login   <babiole@epitech.net>
##
## Started on  lundi 26  Babiole,,,
## Last update lundi 26  Babiole,,,
##

CC		= gcc

RM		= rm -f

NAME		= auto-compile

SRCS		= src/main.c

OBJS		= $(SRCS:.c=.o)

CFLAGS		= -I./include -Wall -Wextra -W

$(NAME):	$(OBJS)

		$(CC) -o $(NAME) $(OBJS) $(LIB) 

all:		$(NAME)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:		all clean fclean re
