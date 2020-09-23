# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/01 14:39:55 by lmoulin           #+#    #+#              #
#    Updated: 2020/07/28 13:32:49 by lmoulin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re norme

CC = gcc

FLAGS = -Wall -Wextra -Werror

CPP_FLAGS = -Iinclude

NAME = libftprintf.a

SRC_PATH = ./
LIB_PATH = ../libft
INC_PATH = ../includes
OBJ_PATH = ./
OBJLIB_PATH = ../libft/objs

SRC_NAME =	ft_printf.c ft_annexe.c ft_arg.c ft_arg_2.c ft_flags.c \
			ft_flags_2.c ft_exception.c


INC_NAME = ft_printf.h

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))
LIB = $(addprefix $(LIB_PATH)/, $(LIB_NAME))
INC = $(addprefix $(INC_PATH)/, $(INC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(NAME)

$(NAME): $(OBJ)# $(OBJLIB)
	@make -C ../libft
	@ar rc $(NAME) $(OBJ) ../libft/libft.a
	@ranlib $(NAME)
	@echo "\033[1;34mft_printf\t\033[1;33mCompilation\t\033[0;32m[OK]\033[0m"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
#	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) -o $@ -c $<

$(OBJLIB_PATH)/%.o: $(LIB_PATH)/%.c
#	@mkdir $(OBJLIB_PATH) 2> /dev/null || true
	@$(CC) -o $@ -c $<

clean:
	@make clean -C ../libft
	@rm -rf *.o
	@echo "\033[1;34mft_printf\t\033[1;33mCleaning obj\t\033[0;32m[OK]\033[0m"

fclean:
	@rm -rf *.o
	@echo "\033[1;34mft_printf\t\033[1;33mCleaning obj\t\033[0;32m[OK]\033[0m"
	@make fclean -C ../libft
	@rm -rf *.a
	@echo "\033[1;34mft_printf\t\033[1;33mCleaning lib\t\033[0;32m[OK]\033[0m"

re: fclean all

norme:
	@make norme -C ../libft/
	@norminette $(SRC) $(INC)
	@echo "\033[1;34mft_printf\t\033[1;33mNorminette\t\033[0;32m[OK]\033[0m"
