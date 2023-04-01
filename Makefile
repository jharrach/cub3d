# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rburgsta <rburgsta@student.42.de>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 14:53:36 by jharrach          #+#    #+#              #
#    Updated: 2023/04/01 17:56:17 by rburgsta         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

OBJ_DIR		= obj/
SRC_DIR		= src/
MLX42_DIR	= mlx42/
MLX42_B_DIR	= $(MLX42_DIR)build/
LIBFT_DIR	= libft/

SRC			= main ray ray_collision drawings init \
			  parsing parsing_map parsing_utils destroy
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

$(NAME): $(MLX42) $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX42) $(LOADLIBES) -o $@

$(MLX42) :
	@cmake -S $(MLX42_DIR) -B $(MLX42_B_DIR) 2>&1 | sed -e 's/^/mlx42: /;'
	@make -C $(MLX42_B_DIR) $(CMAKE_TRD) 2>&1 | sed -e 's/^/mlx42: /;'

$(LIBFT) :
	@make -C $(LIBFT_DIR)  2>&1 | sed -e 's/^/libft: /;'

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

pfclean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(NAME)

pre: pfclean all

.PHONY: all clean fclean re objdir pfclean pre
