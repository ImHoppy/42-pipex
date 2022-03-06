#SETUP
NAME		=	pipex
CC			=	clang
CFLAGS		=	-Wall -Wextra -Werror -I$(HEADER_DIR)
RM			=	rm -rf

#FILES AND PATH
HEADER_SRCS	=	pipex.h
HEADER_DIR	=	includes/
HEADER		=	$(addprefix $(HEADER_DIR), $(HEADER_SRCS))

MPATH_SRCS	=	pipex.c childs.c utils.c
MPATH_DIR	=	mandatory/
MPATH		=	$(addprefix $(MPATH_DIR), $(MPATH_SRCS))
OBJ_M		=	$(MPATH:.c=.o)

BPATH_SRCS	=	pipex_bonus.c error_bonus.c here_doc_bonus.c\
				files_bonus.c free_bonus.c child_bonus.c
BPATH_DIR	=	bonus/
BPATH		=	$(addprefix $(BPATH_DIR), $(BPATH_SRCS))
OBJ_B		=	$(BPATH:.c=.o)

SHARED_SRCS	=	ft_strncmp.c ft_split.c ft_strjoin.c ft_strlen.c
SHARED_DIR	=	shared/
SHARED 		=	$(addprefix $(SHARED_DIR), $(SHARED_SRCS))
OBJ_F		=	$(SHARED:.c=.o)

#COMMANDS
%.o: %.c $(HEADER) $(MAKE)
		@${CC} ${CFLAGS} -c $< -o $@

$(NAME):	$(OBJ_F) $(OBJ_M)
		@$(CC) $(OBJ_F) $(OBJ_M) -o $(NAME)
		@printf "$(GREEN)$(NAME) created!$(DEFAULT)\n"

all:	$(NAME)

bonus:	$(OBJ_F) $(OBJ_B)
		@$(CC) $(OBJ_F) $(OBJ_B) -o $(NAME)
		@printf "$(GREEN)$(NAME) created!$(DEFAULT)\n"

clean:
		@$(RM) $(OBJ_M)
		@$(RM) $(OBJ_F)
		@$(RM) $(OBJ_B)
		@printf "$(YELLOW)Object files deleted!$(DEFAULT)\n"

fclean:	clean
		@$(RM) $(NAME)
		@printf "$(RED)$(NAME) deleted!$(DEFAULT)\n"

re:				fclean all

.PHONY:		all clean fclean bonus re

#COLORS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	COLOR = \033
endif
ifeq ($(UNAME_S),Darwin)
	COLOR = \x1b
endif
RED 	= $(COLOR)[1;31m
GREEN 	= $(COLOR)[1;32m
YELLOW 	= $(COLOR)[1;33m
DEFAULT = $(COLOR)[0m