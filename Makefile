# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#              #
#    Updated: 2022/05/23 20:29:52 by cleticia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

RM = rm -rf

FLAGS = -Wall -Wextra -Werror

SRC = src/parse/env_list.c src/parse/env_to_matrix.c src/heredoc/exec_heredoc.c\
	src/executor/exec_redir_prep_std.c src/executor/exec_cmd.c\
	src/parse/free_list.c src/executor/exec_redir_utils.c\
	src/parse/mloop_aux.c src/executor/exec_final.c src/executor/exec_utils.c\
	src/executor/executor_redir.c src/executor/executor_redir_ii.c\
	src/executor/exec_declare.c src/parse/parse_utils.c src/parse/parse_order.c\
	src/minishell_main.c src/parse/parse_node.c src/parse/remove_quote.c\
	src/parse/parse_cmd.c src/parse/parse_lexer.c src/parse/parse_doublequote.c\
	src/parse/parse_rebuild.c src/parse/parse_sintaxe.c\
	src/parse/parse_varname.c src/heredoc/set_redir.c\
	src/heredoc/exec_heredoc_pipe.c src/executor/dup_close.c\
	src/heredoc/set_parse_redir.c src/executor/signals_exec.c\
	src/parse/sintaxe_verification.c src/parse/split_minishell.c\
	src/parse/split_utils.c src/executor/exec_pipe.c builtins/cd.c\
	builtins/cd_utils.c builtins/echo.c builtins/env.c builtins/exit.c\
	builtins/export.c builtins/pwd.c builtins/unset.c\
	src/heredoc/exec_read_heredoc.c src/executor/exec_cmd_builtin.c\

OBJS = $(SRC:.c=.o)

LFT = ./src/lib/libft.a

LIB = -L ./src/lib/ -lft -L/usr/include -lreadline

INC = -I ./inc/ -I ./src/lib/

all: $(NAME)

$(NAME): $(LFT) $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) $(INC) $(LIB) $(LFT) -o $@
	@echo "Minishell compile!"

.c.o:
	@$(CC) $(FLAGS) -c $< -o $@

$(LFT):
	@echo "Compiling libft..."
	@make -s -C ./src/lib
	@echo "Libft ready!"

clean:
	@make -s $@ -C ./src/lib
	@$(RM) $(OBJS)
	@echo "Clean done!"
	
fclean:
	@$(RM) $(NAME) $(OBJS)
	@make -s $@ -C ./src/lib
	@echo "All binary files deleted!"

re: fclean all

.PHONY: all clean fclean re
