# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/28 11:50:25 by lmoulin           #+#    #+#              #
#    Updated: 2020/07/28 13:42:53 by lmoulin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC =			gcc

FLAGS =			-Wall -Wextra -Werror

RM =			rm -rf

DIR_HEADERS =	./includes/

DIR_SRCS =		./srcs/

DIR_OBJS =		./objs/

DIR_LIBFT =		./libft/libft/

DIR_PRINTF =	./libft/ft_printf/


SRC =		cd.c echo.c export.c ls.c main.c pwd.c unset.c \


SRCS =			$(addprefix $(DIR_SRCS), $(SRC))

COMPIL =		$(FLAGS)

OBJS =			$(SRCS:.c=.o)

NAME =			MiniShell

all:			$(NAME)

$(NAME) :		$(OBJS)
				@make -C $(DIR_PRINTF)
				@cp $(DIR_PRINTF)libftprintf.a ./lib
				@cp $(DIR_LIBFT)libft.a ./lib
				@$(CC) $(COMPIL) -I $(DIR_HEADERS) $(OBJS) ./lib/libft.a ./lib/libftprintf.a -o $(NAME)
				@mv ./srcs/*.o $(DIR_OBJS)
				@echo "\033[1;34mMiniShell\t\033[1;33mCompilation\t\033[0;32m[OK]\033[0m"

%.o: %.c
				@gcc $(FLAGS) -I $(DIR_HEADERS) -c $< -o $@
				@echo "Compiled "$<" successfully!"

norme:
				@make norme -C $(DIR_PRINTF)
				@norminette $(DIR_SRCS)
				@norminette $(DIR_HEADERS)
				@echo "\033[1;34mMiniShell\t\033[1;33mNorminette\t\033[0;32m[OK]\033[0m"

clean:
				@$(RM) $(DIR_OBJS)*
				@make clean -C $(DIR_PRINTF)
				@echo "\033[1;34mMinishell\t\033[1;33mCleaning obj\t\033[0;32m[OK]\033[0m"

fclean:	
				@$(RM) $(DIR_OBJS)*
				@echo "\033[1;34mMinishell\t\033[1;33mCleaning obj\t\033[0;32m[OK]\033[0m"
				@$(RM) $(NAME)
				@make fclean -C $(DIR_PRINTF)
				@$(RM) ./lib/*
				@echo "\033[1;34mMiniShell\t\033[1;33mCleaning lib\t\033[0;32m[OK]\033[0m"

re:				fclean all

.PHONY:			all, clean, fclean, re
