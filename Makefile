# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/25 15:25:19 by pbremond          #+#    #+#              #
#    Updated: 2022/10/17 19:16:43 by pbremond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
_BLACK			= \033[0;30m
_RED 			= \033[0;31m
_GREEN			= \033[0;32m
_BROWN			= \033[0;33m
_BLUE			= \033[0;34m
_PURPLE			= \033[0;35m
_CYAN			= \033[0;36m
_LIGHT_GRAY		= \033[0;37m
_DARK_GRAY		= \033[1;30m
_LIGHT_RED		= \033[1;31m
_LIGHT_GREEN	= \033[1;32m
_YELLOW			= \033[1;33m
_LIGHT_BLUE		= \033[1;34m
_LIGHT_PURPLE	= \033[1;35m
_LIGHT_CYAN		= \033[1;36m
_WHITE			= \033[1;37m
_NC 			= \033[0m

# Inverted, i.e. colored backgrounds
_IGREY			= \x1b[40m
_IRED			= \x1b[41m
_IGREEN			= \x1b[42m
_IYELLOW		= \x1b[43m
_IBLUE			= \x1b[44m
_IPURPLE		= \x1b[45
_ICYAN			= \x1b[46m
_IWHITE			= \x1b[47m

# Color reset
_COLOR_RESET	= \033[0m

# Folders
INCLUDES = includes
SRC_DIR = src
OUTS = objs

# Source Files
SRC =	$(BUILTIN_SRC) $(LEXER_SRC) $(PARSER_SRC) $(REDIR_SRC)\
		main.c\
		exec.c aux.c\
		prompt.c utils.c\
		signal_catching.c\
		debug.c

BUILTIN_SRC_FILES =		env/expand_env.c	env/env.c				export/export.c\
	unset/unset.c		pwd/pwd.c			echo/echo.c				cd/cd.c\
	env/path/exec_path.c					env/path/path.c			exit/exit.c

LEXER_SRC_FILES =		lexer.c				lexer_aux.c				lexer_utils.c\
	lexer_utils_2.c		quote_checking.c

PARSER_SRC_FILES =		parser.c			parser_utils.c			env_expanding.c\
	env_expanding_2.c	string_parsing.c	tilde_expanding.c		redirs_mapping.c\
	pre_parser.c		pre_parser_2.c		token_padding.c

REDIR_SRC_FILES =		luigi.c				mario.c					plumber.c\
	redirections.c		wario.c				waluigi.c

LEXER_SRC =		$(addprefix lexer/,			$(LEXER_SRC_FILES))
PARSER_SRC =	$(addprefix parser/,		$(PARSER_SRC_FILES))
REDIR_SRC =		$(addprefix redirections/,	$(REDIR_SRC_FILES))
BUILTIN_SRC =	$(addprefix builtins/,		$(BUILTIN_SRC_FILES))
		
SRC_PLUS_PATH = $(addprefix $(SRC_DIR)/, $(SRC))

# Output Files
OUT = $(subst $(SRC_DIR)/, $(OUTS)/, $(patsubst %.c, %.o, $(SRC_PLUS_PATH)))

BONUS_SRCS = main_bonus.c

LIBFT_PATH = libft

# This project requires the libreadline library to be installed! On MacOS,
# you can get it through homebrew. On Linux systems, you can install it with
# whatever packet manager. Debian based: `apt install libreadline-dev'.
ifeq ($(shell uname -s),Darwin)

READLINE_LIB = -L${HOME}/.brew/opt/readline/lib
READLINE_INC = -I${HOME}/.brew/opt/readline/include
LIBFT = libft_macos.a
LIBS = -lft_macos -lreadline

else

LIBFT = libft_linux.a
LIBS = -lft_linux -lreadline

endif

NAME = minishell
CC = clang
CFLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME): $(LIBFT_PATH)/$(LIBFT) $(OUT)
	@echo "$(_PURPLE)Linking $(NAME)$(_COLOR_RESET)"
	@$(CC) $(CFLAGS) $(OUT) -o $(NAME) -L./$(LIBFT_PATH) $(READLINE_LIB) $(LIBS)
	@echo "$(_GREEN)DONE$(_COLOR_RESET)"

# $(LIBFT_PATH)/$(LIBFT):
# 	@echo "$(_PURPLE)Compiling $(basename $(LIBFT))$(_COLOR_RESET)"
# 	@make -C $(LIBFT_PATH)/

$(OUT): $(OUTS)/%.o : $(SRC_DIR)/%.c
	@echo "$(_BLUE)Compiling $(basename $(notdir $*.o)) $(_COLOR_RESET)"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@ -I./$(INCLUDES) $(READLINE_INC)

re: fclean $(NAME)

fclean: clean
	@echo "$(_RED)Cleaning output files$(_COLOR_RESET)"
	@rm -rf $(NAME)

clean:
	@echo "$(_RED)Cleaning object files$(_COLOR_RESET)"
	@rm -rf $(OUTS)
	
.PHONY: clean fclean re all
