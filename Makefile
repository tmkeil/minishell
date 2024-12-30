CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline

NAME = minishell
INCLUDES = -Iheaders
OBJSDIR = objs/
SRCSDIR = srcs/
LIBFTDIR = libft/
LIBFT = libft/libft.a

LEXING = lexer.c lexer_utils.c lexer_utils2.c expand.c expand2.c
TABLE = create_exec_table.c
EXECUTING = execution.c get_cmd_arguments.c getpath.c execution_utils.c
BUILTINS = cd.c env.c exit.c export.c pwd.c unset.c echo.c
CLEARING = cleaners.c
UTILS = utils.c utils2.c utils3.c utils4.c

SRCS = minishell.c $(addprefix lexing/, $(LEXING)) $(addprefix create_execution_table/, $(TABLE)) $(addprefix executing/, $(EXECUTING)) \
		$(addprefix builtins/, $(BUILTINS)) $(addprefix clearing/, $(CLEARING)) $(addprefix utils/, $(UTILS))

BONUS_SRCS = minishell.c $(addprefix lexing/, $(LEXING)) $(addprefix create_execution_table/, $(TABLE)) $(addprefix executing/, $(EXECUTING)) \
		$(addprefix builtins/, $(BUILTINS)) $(addprefix clearing/, $(CLEARING)) $(addprefix utils/, $(UTILS))

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
	rm -rf $(NAME)
	make -C $(LIBFTDIR) fclean

ct:
	rm -rf tmp_err_bash tmp_err_minishell tmp_out_bash tmp_out_minishell

re: fclean all

.PHONY: all clean fclean re bonus
