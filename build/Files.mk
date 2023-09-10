# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Files.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/12 23:45:12 by mbouthai          #+#    #+#              #
#    Updated: 2023/08/31 05:06:02 by mbouthai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Headers/Includes

HEADERS_DIR	:= include

#HEADERS	=
#HEADERS	:= $(addprefix $(HEADERS_DIR)/,$(HEADERS))

# Sources

SRCS_DIR	:= srcs

CHANNEL_DIR	:= channel
CHANNEL_SRCS	= Channel.cpp
CHANNEL_SRCS	:= $(addprefix $(CHANNEL_DIR)/,$(CHANNEL_SRCS))

COMMANDS_DIR	:= commands
COMMANDS_SRCS	= Commands.cpp
COMMANDS_SRCS	:= $(addprefix $(COMMANDS_DIR)/,$(COMMANDS_SRCS))

SERVER_DIR	:= server
SERVER_SRCS	= Server.cpp
SERVER_SRCS	:= $(addprefix $(SERVER_DIR)/,$(SERVER_SRCS))

USER_DIR	:= user
USER_SRCS	= User.cpp
USER_SRCS	:= $(addprefix $(USER_DIR)/,$(USER_SRCS))

UTILS_DIR	:= utils
UTILS_SRCS	= Utils.cpp
UTILS_SRCS	:= $(addprefix $(UTILS_DIR)/,$(UTILS_SRCS))

SOURCES	= main.cpp \
	$(CHANNEL_SRCS) \
	$(COMMANDS_SRCS) \
	$(SERVER_SRCS) \
	$(USER_SRCS) \
	$(UTILS_SRCS) \
	

SOURCES	:= $(addprefix $(SRCS_DIR)/,$(SOURCES))

# Objects/Output files

OBJS_DIR	= obj
#OBJECTS	:= $(patsubst %,%.o,$(addprefix $(OBJS_DIR)/,$(notdir $(SOURCES))))
OBJECTS	:= $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%.o,$(SOURCES))

# Dependencies

DEPS_DIR	= dep
DEPENDENCIES	:= $(patsubst %,%.d,$(addprefix $(DEPS_DIR)/,$(notdir $(SOURCES))))

DIRS	= $(CHANNEL_DIR) \
	$(COMMANDS_DIR) \
	$(SERVER_DIR) \
	$(USER_DIR) \
	$(UTILS_DIR)

DIRS	:= $(addprefix $(OBJS_DIR)/,$(DIRS))
