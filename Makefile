# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/12 20:33:55 by pmagalha          #+#    #+#              #
#    Updated: 2024/01/29 10:24:53 by pmagalha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                  SOURCE FILES                                #
################################################################################

SRC = main.c lexer.c init.c list_aux.c env.c expander.c parser.c
OBJS = $(SRC:.c=.o)
DEPS = libft/libft.a

################################################################################
#                           EXECUTABLES & LIBRARIES                            #
################################################################################

NAME = minishell
LIBFT = libft/libft.a

################################################################################
#                                     FLAGS                                    #
################################################################################

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

################################################################################
#                                    RULES                                     #
################################################################################

all: deps $(NAME)

deps:
		$(MAKE) -C ./libft

GREEN		= \033[38;5;47m
YELLOW		= \033[38;5;226m
RED			= \033[38;5;196m
CYAN 		= \033[1;36m
RESET 		= \033[0m
BLUE 		= \033[0;34m
PURPLE		= \033[38;5;93m
ORANGE		= \033[38;5;202m

$(NAME): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
	@echo "\n$(RED)   __  __   _           _         _              _   _ $(RESET)"
	@echo "$(ORANGE)  |  \/  | (_)         (_)       | |            | | | |$(RESET)"
	@echo "$(YELLOW)  | \  / |  _   _ __    _   ___  | |__     ___  | | | |$(RESET)"
	@echo "$(GREEN)  | |\/| | | | | '_ \  | | / __| | '_ \   / _ \ | | | |$(RESET)"
	@echo "$(BLUE)  | |  | | | | | | | | | | \__ \ | | | | |  __/ | | | |$(RESET)"
	@echo "$(PURPLE)  |_|  |_| |_| |_| |_| |_| |___/ |_| |_|  \___| |_| |_|$(RESET)"
	@echo "$ \n$(RESET)"

$(LIBFT):
	@echo "[$(CYAN)compiling$(RESET)]:\t $@"
	@make -C $(LIBFT_DIR)

$(OBJS): %.o: %.c
	$(CC) -c $< -o $@
	@echo "[$(BLUE)compiling$(RESET)]:\t $<"
	
clean:
	$(MAKE) $@ -C ./libft
	@rm -rf $(OBJS)

fclean: clean
	$(MAKE) $@ -C ./libft
	@rm -rf $(NAME)
	@echo " \n$(RED)::::: Removing $(NAME) executable ::::: $(RESET)\n "

re: fclean all
	@echo "[$(GREEN)DONE$(RESET)]: $(GREEN)Minishell$(RESET)"