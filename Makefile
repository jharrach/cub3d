# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 14:53:36 by jharrach          #+#    #+#              #
#    Updated: 2023/03/20 15:42:03 by jharrach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

OBJ_DIR		= obj/
SRC_DIR		= src/
MLX42_DIR	= mlx42/
MLX42_B_DIR	= $(MLX42_DIR)build/
LIBFT_DIR	= libft/

SRC			= main
OBJ			= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC)))

MLX42		= $(MLX42_B_DIR)libmlx42.a
LIBFT		= $(LIBFT_DIR)libft.a

CFLAGS		= -Wall -Wextra -Werror
LOADLIBES	= -lglfw -lm

ifneq ($(shell uname), Linux)
GLFW_DIR	= $(shell brew --prefix glfw)
LOADLIBES	+= $(addprefix -L, $(addsuffix /lib/, $(GLFW_DIR)))
endif

all: $(NAME)

$(NAME): $(OBJ)
	@cmake -S $(MLX42_DIR) -B $(MLX42_B_DIR) 2>&1 | sed -e 's/^/mlx42: /;'
	@make -C $(MLX42_B_DIR) 2>&1 | sed -e 's/^/mlx42: /;'
	@make -C $(LIBFT_DIR) 2>&1 | sed -e 's/^/libft: /;'
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX42) $(LOADLIBES) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | objdir
	$(CC) $(CFLAGS) -c $< -o $@

objdir:
	@mkdir -p $(OBJ_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(LIBFT_DIR)obj
	$(RM) -r $(MLX42_B_DIR)

fclean: clean
	$(RM) $(LIBFT)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re objdir
