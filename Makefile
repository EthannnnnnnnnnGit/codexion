# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ethan <ethan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/08 10:39:26 by zqian             #+#    #+#              #
#    Updated: 2026/06/09 17:46:10 by ethan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP

INC_DIR = includes
SRC_DIR = srcs
OBJ_DIR = objs

CORE_SRCS = \
core/main.c \
core/free.c

PARSING_SRCS = \
parsing/parsing.c \
parsing/parsing_utils.c \
parsing/init.c


SRCS = $(addprefix $(SRC_DIR)/, $(CORE_SRCS) $(PARSING_SRCS) $(UTILS_SRCS) $(DISORDER_SRCS) $(OPERATION_SRCS) $(ALGORITHM_SRCS) $(BENCHMARK_SRCS))
SRCS_BON = $(addprefix $(SRC_DIR)/, core/free.c $(PARSING_SRCS) $(UTILS_SRCS) $(DISORDER_SRCS) $(OPERATION_SRCS) $(ALGORITHM_SRCS) $(BENCHMARK_SRCS) $(CHECKER_SRCS)) \
	$(GNL)
SRCS_LIBFT = $(addprefix $(LIBFT_DIR)/, $(addsuffix .c, $(FILE_LIBFT))) libft/libft.h
SRCS_PRINTF = $(addprefix $(PRINTF_DIR)/, $(addsuffix .c, $(FILE_PRINTF))) printf/ft_printf.h

OBJS = $(foreach src,$(SRCS),$(OBJ_DIR)/$(notdir $(src:.c=.o)))
DEPS = $(OBJS:.o=.d)
OBJS_BON = $(foreach src,$(SRCS_BON),$(OBJ_DIR)/$(notdir $(src:.c=.o)))
DEPS_BON = $(OBJS_BON:.o=.d)

vpath %.c $(SRC_DIR)/core $(SRC_DIR)/parsing $(SRC_DIR)/utils $(SRC_DIR)/disorder $(SRC_DIR)/operation $(SRC_DIR)/algorithms $(SRC_DIR)/benchmark $(SRC_DIR)/checker get_next_line

all: $(NAME)

bonus: $(NAME_BON)

$(NAME): $(PRINTF) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -L$(PRINTF_DIR) -lft -lftprintf -o $@

$(NAME_BON): $(PRINTF) $(OBJS_BON) 
	$(CC) $(CFLAGS) $(OBJS_BON) -L$(LIBFT_DIR) -L$(PRINTF_DIR) -lft -lftprintf -o $@

$(PRINTF): $(SRCS_LIBFT) $(SRCS_PRINTF)
	$(MAKE) -C $(PRINTF_DIR) bonus

$(OBJ_DIR)/%.o: %.c $(INC_DIR)/push_swap.h $(PRINTF)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -I$(PRINTF_DIR) -c $< -o $@

clean:
	$(MAKE) -C $(PRINTF_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(PRINTF_DIR) fclean
	rm -f $(NAME) $(NAME_BON)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re bonus