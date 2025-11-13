# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/13 14:59:33 by rdellaza          #+#    #+#              #
#    Updated: 2025/11/13 16:20:34 by rdellaza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread -g
# TODO: Add -fsanitize=thread later for thread debugging

# Directories
INC_DIR = includes
OBJ_DIR = obj

# Source files (add all here on the go)
SRCS = main.c \
       parsing.c \
       init.c

# TODO: Add more source files above this line dude

# Object files (placed in obj/ directory)
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Colors for pretty output
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
RESET = \033[0m

# Default target
all: $(NAME)

# Create executable
$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) ready!$(RESET)"

# Compile .c files to .o files in obj/ directory
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

# Clean everything
fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
