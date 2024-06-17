# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/12 20:33:55 by pmagalha          #+#    #+#              #
#    Updated: 2024/06/17 18:03:54 by joao-ppe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                  SOURCE FILES                                #
################################################################################

SRC = src/main.c src/lexer.c src/init.c src/lexer_utils.c src/env.c src/expander.c src/expander_utils1.c \
src/parser.c src/parser_utils.c src/quotes_utils.c src/builtins.c src/export.c src/export_utils1.c src/export_utils2.c \
src/unset.c src/free_utils.c src/expander_utils2.c src/expander_utils3.c src/executor.c src/executor_utils1.c src/executor_utils2.c src/error.c \
src/heredoc.c src/heredoc_utils.c src/redirects.c src/signals.c src/builtins/ms_env.c src/builtins/ms_echo.c src/builtins/ms_pwd.c src/builtins/ms_exit.c src/builtins/ms_cd.c \
src/ms_utils1.c src/ms_utils2.c

OBJS = $(SRC:.c=.o)
DEPS = include/libft/libft.a

################################################################################
#                           EXECUTABLES & LIBRARIES                            #
################################################################################

NAME = minishell
LIBFT = include/libft/libft.a

################################################################################
#                                     FLAGS                                    #
################################################################################

CC = cc -g
CFLAGS = -g

################################################################################
#                                    RULES                                     #
################################################################################

all: deps $(NAME)

deps:
		$(MAKE) -C ./include/libft

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
	$(MAKE) $@ -C ./include/libft
	@rm -rf $(OBJS)

fclean: clean
	$(MAKE) $@ -C ./include/libft
	@rm -rf $(NAME)
	@echo " \n$(RED)::::: Removing $(NAME) executable ::::: $(RESET)\n "

re: fclean all
	@echo "[$(GREEN)DONE$(RESET)]: $(GREEN)Minishell$(RESET)"
