# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltimsit- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/14 14:44:15 by ltimsit-          #+#    #+#              #
#    Updated: 2019/11/14 17:23:17 by ltimsit-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d
SRCS = main.c parse_map.c display_tools.c hook.c
FSAN = -fsanitize=address
FLAGS = 
OBJS = $(SRCS:.c=.o)
CC = gcc $(INC) $(FLAGS)
INC = -I libft wolf.h mlx.h
LIBMLX = libmlx.a
LIBFT = libft/libft.a
FRAMEWORK = -framework OpenGL -framework AppKit

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@gcc $(FLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LIBMLX) $(FRAMEWORK)

$(LIBFT) : force
	@make -C libft
force :

%.o : %.c
	gcc $(FLAGS) $(INC) -c $<

clean :
	@make clean -C libft
	@rm -f $(OBJS)

fclean : clean
	@make fclean -C libft
	@rm -rf $(NAME)

re : fclean all
