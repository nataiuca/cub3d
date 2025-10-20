
# Program name
NAME		= cub3D

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g #-fsanitize=address -fno-omit-frame-pointer

# MLX42
MLX42_DIR= ./MLX42
MLX42_INC= $(MLX42_DIR)/include/MLX42
MLX42_NAME= $(MLX42_DIR)/build/libmlx42.a
MLX42_FLAGS= -ldl -lglfw -pthread -lm

# Directories
SRCDIR		= src
OBJDIR		= obj
INCDIR		= inc

# Source files
SOURCES		= main.c \
			  core/init_game.c \
			  core/init_graphics.c \
			  core/cleanup_utils.c \
			  core/cleanup_game.c \
			  parsing/read_file.c \
			  parsing/config_parser.c \
			  parsing/map_parser.c \
			  parsing/config_validator.c \
			  parsing/map_validator.c \
			  parsing/parser_utils.c \
			  engine/raycasting.c \
			  engine/raycasting_utils.c \
			  engine/player.c \
			  engine/collision.c \
			  engine/moves.c \
			  utils/error.c \
			  graphics/draw.c \
			  graphics/textures.c \
			  controls/events.c \
			  minimap/init_minimap.c \
			  minimap/minimap_rays.c \
			  minimap/minimap_draw.c \
			  minimap/minimap_utils.c \
			  sprite/init_sprite.c \
			  sprite/sprite_load.c \
			  sprite/sprite_draw.c \
			  sprite/sprite_update.c

# Add src/ prefix
SRCS		= $(addprefix $(SRCDIR)/, $(SOURCES))

# Object files
OBJECTS		= $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Header files
INCLUDES	= -I $(INCDIR) -I $(MLX42_INC)

# Libraries
LIBFT		= $(INCDIR)/libft.a

# Colors
GREEN		= \033[0;32m
RED			= \033[0;31m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
RESET		= \033[0m

# Progress
TOTAL		= $(words $(SOURCES))
CURRENT		= 0

# RULES

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT) $(MLX42_NAME)
	@echo "$(BLUE)🔨 Compiling MLX42 in $(MLX42_DIR)...$(RESET)"
	@echo "$(BLUE)🔗 Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(MLX42_NAME) $(MLX42_FLAGS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(RESET)"
	@echo "$(GREEN)⚡ Optimized for maximum performance$(RESET)"
	@echo "$(YELLOW)📋 Usage: ./$(NAME) <map.cub>$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@$(eval CURRENT=$(shell echo $$(($(CURRENT)+1))))
	@printf "$(BLUE)[%2d/%2d] Compiling %-40s$(RESET)\n" $(CURRENT) $(TOTAL) "$<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(MLX42_NAME):
	@cmake -S $(MLX42_DIR) -B $(MLX42_DIR)/build
	@cmake --build $(MLX42_DIR)/build -j4

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@if [ -d $(OBJDIR) ]; then rm -rf $(OBJDIR); fi
	@rm -rf $(MLX42_DIR)/build

fclean: clean
	@echo "$(RED)🗑️  Removing executable...$(RESET)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean
