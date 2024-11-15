# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/11 14:38:20 by pmolzer           #+#    #+#              #
#    Updated: 2024/09/09 23:35:04 by pmolzer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers
SRCS =  $(wildcard sources/*.c)
OBJS = ${SRCS:.c=.o}

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -pthread -g -fsanitize=thread

RM = rm -rf

all: ${NAME}
${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean: 
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re