NAME	=	kekeijf

SRC		=	main.c		\
			analyze.c	\
			print.c		\
			sym.c

OBJ		=	$(SRC:.c=.o)

INC		=	-I ./includes

CFLAGS	+=	-Wall -Wextra $(INC) -g

CC		=	gcc

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) -o $(NAME) $(OBJ)

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME)

re:			fclean all