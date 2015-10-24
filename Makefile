SRC		= $(SRC_FOLDER)main.c		\

SRC_FOLDER	= src/

CC		= gcc

CFLAGS		= -W -Wall

RM		= rm -rf

CFLAGS		= -Iinclude

OBJ		= $(SRC:.c=.o)

NAME		= auto-compile

$(NAME) : $(OBJ)
	  $(CC) -o $(NAME) $(OBJ)

all	: $(NAME)

clean	:
	  $(RM) $(OBJ)

fclean	: clean
	  $(RM) $(NAME)

re	: fclean all
