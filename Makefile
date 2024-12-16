CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline

NAME = minishell
INCLUDES = -Iheaders
OBJSDIR = objs/
SRCSDIR = srcs/
LIBFTDIR = libft/
LIBFT = libft/libft.a

SRCS = minishell.c display_intro.c lexer.c utils.c lexer_utils.c lexer_utils2.c recursive_descent_parser.c
BONUS_SRCS = minishell.c display_intro.c lexer.c utils.c lexer_utils.c lexer_utils2.c recursive_descent_parser.c

SRCS_PATHS = $(addprefix $(SRCSDIR), $(SRCS))
BONUS_PATHS = $(addprefix $(SRCSDIR), $(BONUS_SRCS))

OBJS = $(addprefix $(OBJSDIR), $(SRCS:.c=.o))
OBJS_BONUS = $(addprefix $(OBJSDIR), $(BONUS_SRCS:.c=.o))

all: $(NAME)

bonus: $(OBJS_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) $(INCLUDES) $(LIBS) -o $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) $(LIBS) -o $(NAME)

$(OBJSDIR)%.o: $(SRCSDIR)%.c
	mkdir -p $(OBJSDIR)
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
