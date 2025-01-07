CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline

NAME = minishell
INCLUDES = -Iheaders
OBJSDIR = objs/
SRCSDIR = srcs/
LIBFTDIR = libft/
LIBFT = libft/libft.a

LEXING = lexer.c lexer_utils.c lexer_utils2.c expand.c expand2.c expand3.c
TABLE = cmd_list.c cmd_list2.c cmd_list3.c cmd_list4.c
EXECUTING = execution.c getpath.c execution2.c pipes_redirects.c
BUILTINS = cd.c env.c exit.c export.c pwd.c unset.c echo.c
CLEARING = cleaners.c cleaners2.c
UTILS = utils.c utils2.c utils3.c utils_main.c

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

valgrind: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(NAME) < valgrind_test.txt

.PHONY: all clean fclean re bonus
