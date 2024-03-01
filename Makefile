# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/12 20:33:55 by pmagalha          #+#    #+#              #
#    Updated: 2024/02/28 15:48:57 by joao-ppe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                  SOURCE FILES                                #
################################################################################

SRC = main.c lexer.c init.c lexer_utils.c env.c expander.c expander_utils.c parser.c parser_utils.c quotes_utils.c builtins.c
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

CC = cc -g -Wall -Wextra -Werror -g
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