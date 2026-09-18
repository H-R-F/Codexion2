# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/16 21:57:00 by coder             #+#    #+#              #
#    Updated: 2026/09/18 02:01:10 by hrf              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread

SRCS        = coder/main.c \
              coder/utils.c \
              coder/init.c \
              coder/heap.c \
              coder/dongles.c \
              coder/coder.c \
              coder/monitor.c \
              coder/cleanup.c

OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c coder/codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
