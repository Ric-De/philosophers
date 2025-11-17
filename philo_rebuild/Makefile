# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/16 17:12:40 by rdellaza          #+#    #+#              #
#    Updated: 2025/11/16 17:13:01 by rdellaza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS =	main.c \
		parsing.c \
		init.c \
		utils.c \
		routine.c \
		actions.c \
		actions_eat.c \
		monitor.c \
		stop_control.c \
		cleanup.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "✅ Philosophers compiled successfully!"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@echo "🧹 Object files cleaned!"

fclean: clean
	@$(RM) $(NAME)
	@echo "🧹 Executable removed!"

re: fclean all

.PHONY: all clean fclean re
