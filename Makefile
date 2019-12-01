# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cylemair <cylemair@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/11 15:09:42 by cylemair          #+#    #+#              #
#    Updated: 2019/11/26 15:50:10 by cylemair         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		=	gcc -g3

CFLAGS	+=	-Werror -Wall -Wextra -I ./includes/

LDFLAGS	+=	-I ./includes/ -L libft/ -lft

NAME	=	minishell

RM		=	rm -rf

SRC		=	srcs/main.c 		\
			srcs/copy_env.c		\
			srcs/exec.c			\
			srcs/std_parse.c	\
			srcs/cd.c			\
			srcs/signal.c		\

OBJS	=	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	make -C libft/
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	make clean -C libft/
	$(RM) $(OBJS)

fclean: clean
	make fclean -C libft/
	$(RM) $(NAME)

re: fclean all
	make re -C libft/

PHONY: all clean fclean re