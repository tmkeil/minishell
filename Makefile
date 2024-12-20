CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline

NAME = minishell
INCLUDES = -Iheaders
OBJSDIR = objs/
SRCSDIR = srcs/
LIBFTDIR = libft/
LIBFT = libft/libft.a

SRCS = lexing/lexer.c lexing/lexer_utils.c lexing/lexer_utils2.c executing/execution.c executing/getpath.c executing/get_cmd_arguments.c \
		clearing/clear_functions.c builtins/builtins.c minishell.c utils/utils.c utils/utils2.c  \
			create_execution_table/create_exec_table.c

BONUS_SRCS = lexing/lexer.c lexing/lexer_utils.c lexing/lexer_utils2.c executing/execution.c executing/getpath.c \
				executing/get_cmd_arguments.c clearing/clear_functions.c builtins/builtins.c minishell.c utils.c utils2.c \
					create_execution_table/create_exec_table.c

SRCS_PATHS = $(addprefix $(SRCSDIR), $(SRCS))
BONUS_PATHS = $(addprefix $(SRCSDIR), $(BONUS_SRCS))

OBJS = $(SRCS_PATHS:$(SRCSDIR)%.c=$(OBJSDIR)%.o)
OBJS_BONUS = $(BONUS_PATHS:$(SRCSDIR)%.c=$(OBJSDIR)%.o)

all: $(NAME)

bonus: $(OBJS_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) $(INCLUDES) $(LIBS) -o $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) $(LIBS) -o $(NAME)

$(OBJSDIR)%.o: $(SRCSDIR)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	rm -rf $(OBJSDIR)
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -rf $(NAME) $(CHECKER)
	make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus
