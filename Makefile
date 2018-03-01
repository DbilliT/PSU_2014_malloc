##
## Makefile for Makefile in /home/bechad_p/rendu/PSU_2014_malloc
## 
## Made by Pierre Bechade
## Login   <bechad_p@epitech.net>
## 
## Started on  Tue Feb 10 11:18:33 2015 Pierre Bechade
## Last update Thu Feb 12 09:43:53 2015 Pierre Bechade
##

NAME		=	libmy_malloc_$(HOSTTYPE).so

LIB_NAME	=	libmy_malloc.so

RM		=	rm -f

CC		=	gcc

CFLAGS		=	-fPIC -W -Wall -Werror -lpthread -I./include/

SRC		=	src/malloc.c		\
			src/realloc.c 		\
			src/free.c		\
			src/parse_conf.c

OBJS		=	$(SRC:.c=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
			gcc -shared -Wl,-soname,$(NAME) -o $(NAME) $(OBJS) $(CFLAGS)
			ln -s $(NAME) $(LIB_NAME)

clean:
			$(RM) $(OBJS)

fclean: 		clean
			$(RM) $(NAME) $(LIB_NAME)

re: 			fclean clean all
