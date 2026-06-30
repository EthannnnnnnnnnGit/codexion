# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eel-kerc <eel-kerc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/08 10:39:26 by zqian             #+#    #+#              #
#    Updated: 2026/06/30 13:46:09 by eel-kerc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -pthread -g

INC_DIR = includes
SRCS_DIR = srcs
OBJ_DIR = .objs

CORE_SRCS = \
main.c \

PARSING_SRCS = \
parsing/parsing.c \
parsing/get_params.c

SIMULATION_SRCS = \
simulation/schedulers.c \
simulation/simulation.c \
simulation/monitor.c

UTILS_SRCS = \
utils/init_thread.c

SRCS = $(addprefix $(SRC_DIR)/, $(CORE_SRCS) $(PARSING_SRCS) $(SIMULATION_SRCS) $(UTILS_SRCS))

OBJS = $(foreach srcs,$(SRCS),$(OBJ_DIR)/$(notdir $(srcs:.c=.o)))
DEPS = $(OBJS:.o=.d)

vpath %.c $(SRCS_DIR)/ $(SRCS_DIR)/parsing $(SRCS_DIR)/simulation $(SRCS_DIR)/utils

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.c $(INC_DIR)/*
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_BON)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re bonus