# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/13 14:59:33 by rdellaza          #+#    #+#              #
#    Updated: 2025/11/13 18:40:48 by rdellaza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================================================================
# PROGRAM NAME
# ============================================================================
NAME = philo

# ============================================================================
# COMPILER & FLAGS
# ============================================================================
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
CFLAGS += -MMD -MP

# Optional: Thread sanitizer for debugging (make SANITIZE=1)
ifdef SANITIZE
	CFLAGS += -fsanitize=thread
endif

# ============================================================================
# DIRECTORIES
# ============================================================================
OBJ_DIR = obj
INC_DIR = includes

# ============================================================================
# INCLUDE PATHS
# ============================================================================
INCLUDES = -I$(INC_DIR)

# ============================================================================
# SOURCE FILES (NO WILDCARDS - explicit list)
# ============================================================================
SRCS = main.c \
       parsing.c \
       helpers_parsing.c \
       init.c \
       cleanup.c \
       utils.c \
       routine.c \
       actions.c \
       helpers_forks.c \
       monitor.c

# ============================================================================
# OBJECT & DEPENDENCY FILES
# ============================================================================
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# ============================================================================
# COLORS FOR PRETTY OUTPUT
# ============================================================================
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

# ============================================================================
# MAIN RULES
# ============================================================================

all: $(NAME)

# Link the final executable
$(NAME): $(OBJS)
	@echo "$(YELLOW)🔗 Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) ready!$(RESET)"

# Compile object files with dependency generation
$(OBJ_DIR)/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)🔨 Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# All objects depend on header - if header changes, recompile everything
$(OBJS): $(INC_DIR)/philo.h Makefile

# ============================================================================
# CLEAN RULES
# ============================================================================

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)🧹 Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)

re: fclean all

# ============================================================================
# PHONY TARGETS
# ============================================================================

.PHONY: all clean fclean re

# ============================================================================
# OPTIONAL: Debug build with thread sanitizer
# ============================================================================

sanitize:
	@$(MAKE) SANITIZE=1

.PHONY: sanitize

# ============================================================================
# DEPENDENCIES (include auto-generated dependency files)
# ============================================================================

-include $(DEPS)