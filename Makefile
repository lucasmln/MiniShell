NAME			= Minishell

CFLAGS			= -Wall -Wextra -Werror #-g3 #-fsanitize=address

SRC				=	./srcs/builtins/builtins.c \
					./srcs/builtins/builtins_2.c \
					./srcs/builtins/builtins_3.c \
					./srcs/builtins/builtins_error.c \
					./srcs/builtins/change_env_var.c \
					./srcs/builtins/get_var.c \
					./srcs/builtins/get_var_2.c \
					./srcs/parsing/quote.c \
					./srcs/parsing/redir.c \
					./srcs/parsing/redir_utils.c \
					./srcs/utils/main_utils.c \
					./srcs/utils/manip_str.c \
					./srcs/main.c \

ROAD_S			= $(SRC)

ROAD_O			= $(OBJ)

ROAD_B			= $(SRCB)


OBJ				= $(SRC:.c=.o)

CC				= clang

HEADER_DIR		=  -I./includes/minishell.h 

$(NAME) :		$(OBJ)
				@cd libft/libft && make && cp libft.a ../../lib
				@cd libft/ft_printf && make && cp libftprintf.a ../../lib
				$(CC) $(OBJ) lib/libftprintf.a lib/libft.a -ltermcap -o $(NAME)

LIBFT			= libft

all :			$(NAME)

clean :
				@cd libft/libft && make clean
				@cd libft/ft_printf && make clean
				@rm -f $(ROAD_O)

fclean :
				@cd libft/libft && make fclean
				@cd libft/ft_printf && make fclean
				@rm -f $(ROAD_O)
				@rm -f $(NAME)

re : 			fclean all

.PHONY: 		all clean fclean re
