# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/12 20:33:55 by pmagalha          #+#    #+#              #
#    Updated: 2024/05/08 15:43:42 by pmagalha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                  SOURCE FILES                                #
################################################################################

SRC = main.c lexer.c init.c lexer_utils.c env.c expander.c expander_utils1.c \
parser.c parser_utils.c quotes_utils.c builtins.c export.c export_utils1.c export_utils2.c \
unset.c free_utils.c expander_utils2.c expander_utils3.c executor.c executor_utils1.c executor_utils2.c error.c \
heredoc.c heredoc_utils.c redirects.c signals.c builtins/ms_env.c builtins/ms_echo.c builtins/ms_pwd.c builtins/ms_exit.c builtins/ms_cd.c \
ms_utils1.c ms_utils2.c

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

CC = cc -g
CFLAGS = -g

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
