# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaacosta <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 18:02:27 by jaacosta          #+#    #+#              #
#    Updated: 2025/08/09 19:08:17 by jaacosta         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror  ##cc -fsanitize=thread -g -Werror -Wall -Wextra *.c -o philo -lpthread
#AR = ar -rcs
RM = rm -f

SRCS =		philo_checks.c philo_inits.c philo_logic.c philo_parse.c \
			philo_routine.c philo_utils.c philo.c \

HEADER = philo.h

OBJS = $(SRCS:.c=.o)

B_OBJS = $(B_SRCS:.c=.o)

all:		$(NAME)

##$(NAME):	$(OBJS) philo.h
##		$(AR) $(NAME) $(OBJS)
$(NAME):	$(OBJS) philo.h
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c	$(HEADER) Makefile
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJS) $(B_OBJS)

fclean:		clean
		$(RM) $(NAME) $(OBS) $(B_OBJS)

re:
		$(RM) $(NAME) $(OBJS) $(B_OBJS)
		make all

.PHONY: all, clean, fclean, re

