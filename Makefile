# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ctirions <ctirions@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/21 13:59:47 by ctirions          #+#    #+#              #
#    Updated: 2022/11/21 14:18:27 by ctirions         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLACK       = $(shell tput -Txterm setaf 0)
RED         = $(shell tput -Txterm setaf 1)
GREEN       = $(shell tput -Txterm setaf 2)
YELLOW      = $(shell tput -Txterm setaf 3)
LIGHTPURPLE	= $(shell tput -Txterm setaf 4)
PURPLE      = $(shell tput -Txterm setaf 5)
BLUE        = $(shell tput -Txterm setaf 6)
WHITE       = $(shell tput -Txterm setaf 7)
RESET       = $(shell tput -Txterm sgr0)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =	webserv.cpp \
		srcs/parsing/config.cpp \
		srcs/parsing/serverBlock.cpp \
		srcs/parsing/utils.cpp \
		srcs/sockets/server.cpp \
		srcs/sockets/socket.cpp

OBJS = $(SRCS:.cpp=.o)

NAME =	webserv

.cpp.o:
			@$(CXX) $(CXXFLAGS) -c -I ./ $< -o $(<:.cpp=.o)
			@echo "$(LIGHTPURPLE)$< compiled$(RESET)"

$(NAME):	$(OBJS)
			@$(CXX) -o $(NAME) $(CXXFLAGS) $(OBJS)
			@echo "$(GREEN)$(NAME) created$(RESET)"

all:		$(NAME)

clean:
			@rm -f $(OBJS)
			@echo "$(RED)Cleaned!$(RESET)"

fclean:
			@rm -f $(OBJS)
			@rm -f $(NAME)
			@echo "$(RED)Fcleaned!$(RESET)"

re:			fclean all

.PHONY:		all clean fclean re