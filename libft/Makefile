# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/06 18:19:07 by jharrach          #+#    #+#              #
#    Updated: 2023/03/08 13:54:29 by jharrach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				= libft.a

SRC_1				+= ft_atoi
SRC_1				+= ft_bzero
SRC_1				+= ft_calloc
SRC_1				+= ft_isalnum
SRC_1				+= ft_isalpha
SRC_1				+= ft_isascii
SRC_1				+= ft_isdigit
SRC_1				+= ft_isprint
SRC_1				+= ft_memchr
SRC_1				+= ft_memcmp
SRC_1				+= ft_memcpy
SRC_1				+= ft_memmove
SRC_1				+= ft_memset
SRC_1				+= ft_strchr
SRC_1				+= ft_strdup
SRC_1				+= ft_strlcat
SRC_1				+= ft_strlcpy
SRC_1				+= ft_strlen
SRC_1				+= ft_strncmp
SRC_1				+= ft_strnstr
SRC_1				+= ft_strrchr
SRC_1				+= ft_tolower
SRC_1				+= ft_toupper

SRC_2				+= ft_itoa
SRC_2				+= ft_putchar_fd
SRC_2				+= ft_putendl_fd
SRC_2				+= ft_putnbr_fd
SRC_2				+= ft_putstr_fd
SRC_2				+= ft_split
SRC_2				+= ft_striteri
SRC_2				+= ft_strjoin
SRC_2				+= ft_strmapi
SRC_2				+= ft_strtrim
SRC_2				+= ft_substr

SRC_B				+=	ft_lstadd_back
SRC_B				+= ft_lstadd_front
SRC_B				+= ft_lstclear
SRC_B				+= ft_lstdelone
SRC_B				+= ft_lstiter
SRC_B				+= ft_lstlast
SRC_B				+= ft_lstmap
SRC_B				+= ft_lstmap
SRC_B				+= ft_lstnew
SRC_B				+= ft_lstsize

SRC_PRINTF			+= ft_placeholder
SRC_PRINTF			+= ft_printf_c
SRC_PRINTF			+= ft_printf_d
SRC_PRINTF			+= ft_printf_p
SRC_PRINTF			+= ft_printf_s
SRC_PRINTF			+= ft_printf_u
SRC_PRINTF			+= ft_printf_x
SRC_PRINTF			+= ft_printf

SRC_PRINTF_FD		+= ft_dprintf_c
SRC_PRINTF_FD		+= ft_dprintf_d
SRC_PRINTF_FD		+= ft_dprintf_p
SRC_PRINTF_FD		+= ft_dprintf_s
SRC_PRINTF_FD		+= ft_dprintf_u
SRC_PRINTF_FD		+= ft_dprintf_x
SRC_PRINTF_FD		+= ft_dprintf

SRC_EXTRA			+= ft_strcmp
SRC_EXTRA			+= ft_atol
SRC_EXTRA			+= ft_containsdigit
SRC_EXTRA			+= ft_islong
SRC_EXTRA			+= ft_isspace

SRC_GET_NEXT_LINE	+= get_next_line

CFLAGS				= -Wall -Wextra -Werror
ARFLAGS				= rcs

SRC_DIR				= src/
OBJ_DIR				= obj/

SRC					+= $(addprefix src_1/, $(SRC_1))
SRC					+= $(addprefix src_2/, $(SRC_2))
SRC					+= $(addprefix src_b/, $(SRC_B))
SRC					+= $(addprefix src_printf/, $(SRC_PRINTF))
SRC					+= $(addprefix src_dprintf/, $(SRC_PRINTF_FD))
SRC					+= $(addprefix src_get_next_line/, $(SRC_GET_NEXT_LINE))
SRC					+= $(addprefix src_extra/, $(SRC_EXTRA))
OBJ					= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC)))

all: $(NAME)

$(NAME): $(OBJ)
	@$(AR) $(ARFLAGS) $(NAME) $(OBJ)
	@echo "make: $@"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | objdir
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

objdir:
	@mkdir -p $(OBJ_DIR)src_1
	@mkdir -p $(OBJ_DIR)src_2
	@mkdir -p $(OBJ_DIR)src_b
	@mkdir -p $(OBJ_DIR)src_printf
	@mkdir -p $(OBJ_DIR)src_dprintf
	@mkdir -p $(OBJ_DIR)src_get_next_line
	@mkdir -p $(OBJ_DIR)src_extra

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "make: clean"

fclean: clean
	@$(RM) $(NAME)
	@echo "make: fclean"

re: fclean all

.PHONY: all clean fclean re objdir
