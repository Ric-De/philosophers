# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/06 12:21:16 by mfontser          #+#    #+#              #
#    Updated: 2024/12/03 13:03:54 by mfontser         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------------------------COLORS------------------------------------------------------#

NC = \033[0m
GREEN = \033[1;92m
RED = \033[1;91m
CYAN = \e[1;96m
YELLOW = \e[1;93m
BLUE =\e[1;94m
ORANGE =\e[1;38;2;255;128;0m
PINK = \e[1;38;5;213m

#------------------------------------------------VARIABLES---------------------------------------------------#

FILES =  philo.c

FILES += parsing/parsing.c parsing/check_valid_format_and_size.c parsing/check_extrem_and_int_conversion.c

FILES += initialitations/initialitations.c initialitations/init_data_struct.c initialitations/build_philos_and_forks.c \
		 initialitations/init_philos_struct.c

FILES += simulation/run_simulation.c simulation/controller.c simulation/get_time.c simulation/actions.c \
		 simulation/print_state.c simulation/finish_simulation.c

FILES += error/error_messages.c

FILES += free/free_1.c free/free_2.c

FILES += libft_utils/ft_atoi.c libft_utils/ft_isspace.c libft_utils/ft_strlen.c libft_utils/ft_strncmp.c

SRCDIR = src/
SRCS = 	$(addprefix $(SRCDIR), $(FILES))

OBJDIR = obj/
OBJS = $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))

INCLUDES = -I ./inc

NAME = philo

HEADER = inc/philo.h
CC = cc 
RM = rm -rf 
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

#--------------------------------------------------NORMAS----------------------------------------------------#

#Metodo implicito

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADER) Makefile  
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)Compiling... $(END)$(patsubst $(DIR_BUILD)%,%,$@)"

# Mis metodos

all: ${NAME}

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) ${OBJS} -o $(NAME)
	@echo " $(BLUE)                                             ⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "                                              ⠈⠻⠿⠿⣿⣿⡆        "
	@echo "                                        ⢠⣶⣤⣤⣄⠀⠀⠀⢀⣴⣿⠋⠀⠀⣠⣄⣀⣀⠀⠀⠀"
	@echo "                                        ⠀⠉⣽⣿⠟  ⢰⣿⣟⣁   ⠙⠛⣿⣿⠇⠀⠀"
	@echo "	                                 ⣾⣿⣥⣄  ⠈⠛⠛⠿⠟  ⣠⣾⣟⣁⠀⠀⠀"
	@echo "	                                 ⠈⠉⠉⠉⠀        ⠙⠛⠛⠛⠁⠀⠀"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
	@echo "	$(END)$(PINK)	             ⢀⠀⢀⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀              "
	@echo "		        ⢀⣤⣶⣿⣿⣿⣆⠘⠿⠟⢻⣿⣿⡇⢐⣷⣦⣄⡀⠀⠀⠀⠀⠀⠀              "
	@echo "		       ⢸⣿⣿⣿⣧⡄⠙⣿⣷⣶⣶⡿⠿⠿⢃⣼⡟⠻⣿⣿⣶⡄⠀⠀⠀⠀              "
	@echo "		     ⢰⣷⣌⠙⠉⣿⣿⡟⢀⣿⣿⡟⢁⣤⣤⣶⣾⣿⡇⠸⢿⣿⠿⢃⣴⡄⠀⠀              "
	@echo "		     ⢸⣿⣿⣿⣿⠿⠋⣠⣾⣿⣿⠀⣾⣿⣿⣛⠛⢿⣿⣶⣤⣤⣴⣿⣿⣿⡆⠀              "
	@echo "		    ⣴⣤⣄⣀⣠⣤⣴⣾⣿⣿⣿⣿⣆⠘⠿⣿⣿⣷⡄⢹⣿⣿⠿⠟⢿⣿⣿⣿⠀$(END)              "
	@echo " $(NC)       ||||	$(END)$(PINK)    ⢸⣿⣿⡿⠛⠛⣻⣿⣿⣿⣿⣿⣿⣷⣦⣼⣿⣿⠃⣸⣿⠃⢰⣶⣾⣿⣿⡟$(END)⠀ $(NC)|||| $(END)         "
	@echo "	$(NC)||||	$(END)$(PINK)     ⢿⡏⢠⣾⣿⣿⡿⠋⣠⣄⡉⢻⣿⣿⡿⠟⠁⠀⠛⠛⠀⠘⠿⠿⠿⠋⠀$(END) ⠀$(NC)|||| $(END)         "
	@echo "	$(NC)\__/	$(END)$(PINK)     ⠀⠁⠘⢿⣿⣿⣷⣤⣿⣿⠗⠀⣉⣥⣴⣶⡶⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀$(END)  $(NC)\__/  $(END)        "
	@echo " $(NC)        ||  $(END) $(YELLOW)       ⢸⠇$(END)$(PINK)⠀⣤⣀⡉⠛⠛⠋⣉⣠⣴⠿⢿⣿⠿⠋$(END)⠀$(YELLOW)⠸⡇⠀$(END)⠀⠀⠀⠀⠀⠀$(NC)⠀⠀⠀||⠀$(END)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"⠀⠀⠀
	@echo "	$(YELLOW)⣲⣷⣷          ⡾$(END)⠀$(PINK)⠀⠈⠻⢿⣿⣿⣿⣿⡿⠋⣠⠟⠁⠀$(END)⠀$(YELLOW)⠀⠀⢷⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣾⣖⠀$(END)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "	$(YELLOW)⠈⠛⠛⠷⣤⢀      ⢰⠇⠀$(END)$(PINK)⠀⠀⠀⠀⠀⠀⠀⠀⣤⡾⠋⠀⠀$(END)$(YELLOW)⠀⠀ ⠀⠸⡆⠀⠀⠀⠀⠀⢀⣤⠾⠛⠛⠁$(END)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"⠀
	@echo "	$(NC) ||$(END)  $(YELLOW)⠙⠳⢦⣀   ⡿⠀$(END)⠀$(PINK)⠀⠀⠀⠀⠀⠀⠀⢰⡿⠁⠀$(END)$(YELLOW)⠀⠀⠀⠀⠀⠀⠀⢿⠀⠀⣀⡴⠞⠋⠀$(END)$(NC)⠀⠀||⠀$(END)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "	$(NC) ||$(END)	$(YELLOW) ⠁⠛⠾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠘⠷⠛⠁⠀⠀$(END)⠀⠀⠀ $(NC)||⠀⠀⠀$(END)⠀⠀⠀     "
	@echo "	$(NC) \`´	$(END)		                   $(NC)\`´  $(END)         " 
	@echo "	$(CYAN)         ✨ PHILOSOPHERS ARE READY ✨⠀⠀⠀⠀     $(END)                "
	@echo "	"

clean:
	@${RM} ${OBJDIR}
	@echo "$(RED)PHILO OBJECTS DELETED$(END)"

fclean:
	@${RM} ${OBJDIR}
	@echo "$(RED)PHILO OBJECTS DELETED$(END)"
	@${RM} ${NAME}
	@echo "$(RED) PHILO EXEC DELETED$(END)"

re: fclean all
	@echo "$(GREEN) PHILO RE DONE$(END)"

.PHONY: all clean fclean re

#
#
#				⠀⠀⠀⠀⠀⠀								⠀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#				⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⠿⠿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀
#				⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣶⣤⣤⣄⠀⠀⠀⢀⣴⣿⠋⠀⠀⣠⣄⣀⣀⠀⠀⠀
#				⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⣽⣿⠟⠀⠀⢰⣿⣟⣁⠀⠀⠀⠙⠛⣿⣿⠇⠀⠀
#				⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣥⣄⠀⠀⠈⠛⠛⠿⠟⠀⠀⣠⣾⣟⣁⠀⠀⠀
#				⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#						⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠀⢀⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#						⠀⠀⠀⠀⠀⢀⣤⣶⣿⣿⣿⣆⠘⠿⠟⢻⣿⣿⡇⢐⣷⣦⣄⡀⠀⠀⠀⠀⠀⠀
#						⠀⠀⠀⠀⢸⣿⣿⣿⣧⡄⠙⣿⣷⣶⣶⡿⠿⠿⢃⣼⡟⠻⣿⣿⣶⡄⠀⠀⠀⠀
#						⠀⠀⢰⣷⣌⠙⠉⣿⣿⡟⢀⣿⣿⡟⢁⣤⣤⣶⣾⣿⡇⠸⢿⣿⠿⢃⣴⡄⠀⠀
#						⠀⠀⢸⣿⣿⣿⣿⠿⠋⣠⣾⣿⣿⠀⣾⣿⣿⣛⠛⢿⣿⣶⣤⣤⣴⣿⣿⣿⡆⠀
#						⠀⣴⣤⣄⣀⣠⣤⣴⣾⣿⣿⣿⣿⣆⠘⠿⣿⣿⣷⡄⢹⣿⣿⠿⠟⢿⣿⣿⣿⠀
#		   ||||	    	⠀⢸⣿⣿⡿⠛⠛⣻⣿⣿⣿⣿⣿⣿⣷⣦⣼⣿⣿⠃⣸⣿⠃⢰⣶⣾⣿⣿⡟⠀||||
#		   ||||		    ⠀⠀⢿⡏⢠⣾⣿⣿⡿⠋⣠⣄⡉⢻⣿⣿⡿⠟⠁⠀⠛⠛⠀⠘⠿⠿⠿⠋⠀⠀||||
#		   \__/		    ⠀⠀⠀⠁⠘⢿⣿⣿⣷⣤⣿⣿⠗⠀⣉⣥⣴⣶⡶⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀ \__/
#            ||           ⢸⠇⠀⣤⣀⡉⠛⠛⠋⣉⣠⣴⠿⢿⣿⠿⠋⠀⠸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀||⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#			⣲⣷⣷   		 ⡾⠀⠀⠈⠻⢿⣿⣿⣿⣿⡿⠋⣠⠟⠁⠀⠀⠀⠀⢷⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣾⣖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#		    ⠈⠛⠛⠷⣤⢀      ⢰⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⡾⠋⠀⠀⠀⠀⠀⠀⠸⡆⠀⠀⠀⠀⠀⢀⣤⠾⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#			||	  ⠙⠳⢦⣀  ⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⢿⠀⠀⣀⡴⠞⠋⠀⠀⠀||⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#			||		 ⠁⠛⠾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠷⠛⠁⠀⠀⠀⠀⠀ ||⠀⠀⠀⠀⠀⠀
#			`´						                   `´
#				⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ✨ PHILO DONE ✨⠀⠀⠀⠀
#⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
	#@echo "$(RED)"
	#@echo "			⢀⠔⠉⠉⠢⣀⠔⠉⠉⠹⣦⣄"
	#@echo "			⡇⠀⠀⠀⠀⠀⠁⠀⠀ ⠀⣿⠇"
	#@echo "			⠡⡀  $(CYAN)Philo$(END) $(RED)⣼⡟"
	#@echo "			⠀⠈⠢⡀⠀⠀⠀⠀⢀⡾⠋"
	#@echo "			⠀⠀⠀⠀⠙⠲⣦⠞⠋ $(END)"
	#@echo "		$(NC)	⠀⠀⠀⠀⠀⠀⢸⠀⠀$(END)⠀$(YELLOW)⠀⠀⠀⠀⠀⢀⠠⠂⠉⣴⣿⠏⠀$(END)"
	#@echo "		$(NC)	⠀⠀⠀⠀⠀⠀⢸⠀⠀$(END)⠀$(YELLOW)⠀⠀⠀⡠⠊⠀⢀⡤⠞⠛⠋⠉⣩⣿⡿$(END)"
	#@echo "		$(NC)	⠀⠀⠀⠀⠀⠀⢸⠀⠀$(END)⠀$(YELLOW)⠀⡠⠊⠀⡠⠞⠁⠀⠀⣀⣴⣾⠟⠋$(END)"
	#@echo "		$(NC)	⠀⠀⠀⠀⠀⠀⢸$(END)$(YELLOW)⡠⠒⠉⠁⠀⠀⣊⣀⣤⣴⠿⠛⠋⠁"
	#@echo "			⠀⠀⠀⠀⠀⣠⠊⠀$(END)$(CYAN)♥$(END)⠀⠀$(YELLOW)⠀⠀⠀⢿⡆"
	#@echo "			⠀⠀⠀⠀⠀⠙⣄⠀⠀⠀⠀⠀⠀⠀⣾⠇$(END)"
	#@echo "		$(NC)	⠀⠀⠀⠀⠀⠀⢸$(END)$(YELLOW)⠳⢦⣄⡀⠀⠠⢴⣅$(END)"
	#@echo "		$(NC)	⠀⠀⠀⠀⠀⠀⠸$(END)$(YELLOW)⠀⡔⠁⠀⠀⠀⠀⣀⣉⡻⢦"
	#@echo "			⠀⠀⠀⠀⠀⠀⠀⠀⢁⠀⠀⠀⢀⡾⠉⠀⠀⠀⠻⡄"
	#@echo "			⠀⠀⠀⠀⠀⠀⠀⠀⡘⠀⠀⠀⣿⠁⠀⠀⠀⠀⠀⠹⠦⣄"
	#@echo "			⠀⠀⠀⠀⠀⠀⠀⣰⠁⠀⢀⣴⣿⡄⠀⠀⠀⠀⠀⣨⣤⡿"
	#@echo "			⠀⠀⠀⠀⠀⠀⠀⠹⠶⠖⠋⠀⠈⠻⢶⣤⣤⣤⠞⠋"
	#@echo "$(END)"