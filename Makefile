# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2023/04/09 01:11:52 by jharrach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		= cub3D

HEADER		= include/cub3d.h
OBJ_DIR		= obj/
SRC_DIR		= src/
DBG_DIR		= debug/
MLX42_DIR	= mlx42/
MLX42_B_DIR	= $(MLX42_DIR)build/
LIBFT_DIR	= libft/

SRC			= main ray ray_collision drawings init \
			  parsing parsing_map parsing_utils destroy rectangle vector entity entity_collision fps door key_input mouse_input hooks minimap button memory
OBJ			= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC)))

MLX42		= $(MLX42_B_DIR)libmlx42.a
LIBFT		= $(LIBFT_DIR)libft.a

CFLAGS		= -Wall -Wextra -Werror
LOADLIBES	= -lglfw -lm

ifneq ($(shell uname), Linux)
GLFW_DIR	= $(shell brew --prefix glfw)
CMAKE_TRD   = -j$(shell sysctl -a | grep machdep.cpu.thread | awk '{print $$2}')
LOADLIBES	+= $(addprefix -L, $(addsuffix /lib/, $(GLFW_DIR)))
else
CMAKE_TRD   = -j$(shell nproc --all)
LOADLIBES   += -ldl -pthread
endif

all: $(NAME)

optimized: pfclean
	@$(MAKE) CFLAGS="-O3"

debug: $(MLX42) $(LIBFT)
	@$(MAKE) CFLAGS="$(CFLAGS) -g" OBJ_DIR="$(DBG_DIR)" NAME="$(DBG_DIR)$(NAME)"

$(NAME): $(MLX42) $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX42) $(LOADLIBES) -o $@

$(MLX42) :
	@cmake -S $(MLX42_DIR) -B $(MLX42_B_DIR) 2>&1 | sed -e 's/^/mlx42: /;'
	@make -C $(MLX42_B_DIR) $(CMAKE_TRD) 2>&1 | sed -e 's/^/mlx42: /;'

$(LIBFT) :
	@make -C $(LIBFT_DIR)  2>&1 | sed -e 's/^/libft: /;'

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER) | objdir
	$(CC) $(CFLAGS) -c $< -o $@

objdir:
	@mkdir -p $(OBJ_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(DBG_DIR)
	$(RM) -r $(LIBFT_DIR)obj
	$(RM) -r $(MLX42_B_DIR)

fclean: clean
	$(RM) $(LIBFT)
	$(RM) $(NAME)

re: fclean all

pfclean:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(DBG_DIR)
	$(RM) $(NAME)

pre: pfclean all

norm:
	norminette $(SRC_DIR) $(HEADER) | less

.PHONY: all clean fclean re objdir pfclean pre norm
