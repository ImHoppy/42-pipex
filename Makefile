#SETUP
NAME		=	pipex
CC			=	clang
CFLAGS		=	-Wall -Wextra -Werror -I$(HEADER_DIR) -g3
RM			=	rm -rf

#FILES AND PATH
HEADER_SRCS	=	pipex.h
HEADER_DIR	=	includes/
HEADER		=	$(addprefix $(HEADER_DIR), $(HEADER_SRCS))

MPATH_SRCS	=	pipex.c childs.c utils.c
MPATH_DIR	=	mandatory/
MPATH		=	$(addprefix $(MPATH_DIR), $(MPATH_SRCS))
OBJ_M		=	$(MPATH:.c=.o)

BPATH_SRCS	=	pipex_bonus_test.c
BPATH_DIR	=	bonus/
BPATH		=	$(addprefix $(BPATH_DIR), $(BPATH_SRCS))
OBJ_B		=	$(BPATH:.c=.o)

SHARED_SRCS	=	ft_strncmp.c ft_split.c ft_strjoin.c ft_strlen.c ft_strdup.c ft_strchr.c ft_calloc.c
SHARED_DIR	=	shared/
SHARED 		=	$(addprefix $(SHARED_DIR), $(SHARED_SRCS))
OBJ_F		=	$(SHARED:.c=.o)

#COMMANDS
%.o: %.c  $(HEADER)
		@printf "	"
		${CC} ${CFLAGS} -c $< -o $@
		@printf "$(BLUE)	-> $< compiled!$(DEFAULT)\n"

$(NAME):	 $(OBJ_F) $(OBJ_M)
		$(CC) $(OBJ_F) $(OBJ_M) -o $(NAME)
		@printf "$(GREEN)-> $(NAME) created!$(DEFAULT)\n"

postbuild:
	@printf "$(BLUE)=> Creating $(NAME)!$(DEFAULT)\n"

all:	 $(NAME)

bonus:	$(OBJ_F) $(OBJ_B)
		@printf "	"
		$(CC) $(OBJ_F) $(OBJ_B) -o $(NAME)
		@printf "$(GREEN)-> $(NAME) bonus created!$(DEFAULT)\n"

clean:
		$(RM) $(OBJ_M)
		$(RM) $(OBJ_F)
		$(RM) $(OBJ_B)
		@printf "$(RED)-> Object files deleted!$(DEFAULT)\n"
fclean:	clean
		$(RM) $(NAME)
		@printf "$(RED)-> $(NAME) deleted!$(DEFAULT)\n"

run: all
	./$(NAME) infile ls cat outfile
	@echo
	@cat outfile

re:	fclean all

.PHONY:		all clean fclean bonus re

#COLORS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	COLOR = \033
endif
ifeq ($(UNAME_S),Darwin)
	COLOR = \x1b
endif
RED 	= $(COLOR)[0;31m
GREEN 	= $(COLOR)[0;32m
YELLOW 	= $(COLOR)[0;33m
BLUE 	= $(COLOR)[0;34m
DEFAULT = $(COLOR)[0m