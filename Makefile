# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/08 10:39:26 by zqian             #+#    #+#              #
#    Updated: 2026/06/10 12:31:26 by eel-kerc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -pthread

INC_DIR = includes
SRC_DIR = srcs
OBJ_DIR = .objs

CORE_SRCS = \
main.c \

PARSING_SRCS = \
parsing/parsing.c \

SRCS = $(addprefix $(SRC_DIR)/, $(CORE_SRCS) $(PARSING_SRCS))

OBJS = $(foreach srcs,$(SRCS),$(OBJ_DIR)/$(notdir $(srcs:.c=.o)))
DEPS = $(OBJS:.o=.d)

vpath %.c $(SRC_DIR)/ $(SRC_DIR)/parsing

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.c $(INC_DIR)/parsing.h 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_BON)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re bonus