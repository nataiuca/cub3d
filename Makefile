# Program name
NAME = cub3D

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MATHFLAGS = -lm

# Detect OS
UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	# macOS
	MLX_DIR = minilibx
	MLX_FLAGS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	MLX_MAKE = make -C $(MLX_DIR)
else
	# Linux
	MLX_DIR = minilibx_linux
	MLX_FLAGS = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm
	MLX_MAKE = make -C $(MLX_DIR)
endif

# Directories
SRCDIR = src
OBJDIR = obj
INCDIR = inc
LIBFT_DIR = libft

# Source files
SOURCES = main.c \
          parsing/parse_file.c \
          parsing/parse_map.c \
          graphics/init_mlx.c \
          graphics/raycasting.c \
		  graphics/simple_draw.c \
          controls/events.c \
          utils/cleanup.c \
          utils/error.c

# Add src/ prefix to all source files
SRCS = $(addprefix $(SRCDIR)/, $(SOURCES))

# Object files
OBJECTS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Header files
INCLUDES = -I $(INCDIR) -I $(LIBFT_DIR) -I $(MLX_DIR)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a

# Colors for pretty output
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RESET = \033[0m

# Progress tracking
TOTAL_FILES = $(words $(SOURCES))
COMPILED = 0

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	@echo "$(BLUE)🔨 Compiling MLX in $(MLX_DIR)...$(RESET)"
	@$(MLX_MAKE)
	@echo "$(BLUE)🔗 Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(RESET)"
	@echo "$(YELLOW)📋 Usage: ./$(NAME) <map.cub>$(RESET)"

# Compile object files with progress
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(eval COMPILED=$(shell echo $(($(COMPILED)+1))))
	@echo "$(BLUE)📦 [$(COMPILED)/$(TOTAL_FILES)] Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@echo "$(BLUE)📚 Compiling libft...$(RESET)"
	@make -C $(LIBFT_DIR) --no-print-directory
	@echo "$(GREEN)✅ libft compiled$(RESET)"

# Create example map and textures
setup: $(NAME)
	@echo "$(BLUE)🗂️  Creating example files...$(RESET)"
	@mkdir -p maps textures
	@echo "Creating simple test map..."
	@echo "NO ./textures/wall_north.xpm" > maps/simple.cub
	@echo "SO ./textures/wall_south.xpm" >> maps/simple.cub
	@echo "WE ./textures/wall_west.xpm" >> maps/simple.cub
	@echo "EA ./textures/wall_east.xpm" >> maps/simple.cub
	@echo "" >> maps/simple.cub
	@echo "F 220,100,0" >> maps/simple.cub
	@echo "C 225,30,0" >> maps/simple.cub
	@echo "" >> maps/simple.cub
	@echo "1111111111" >> maps/simple.cub
	@echo "1000000001" >> maps/simple.cub
	@echo "1010000101" >> maps/simple.cub
	@echo "1000N00001" >> maps/simple.cub
	@echo "1010000101" >> maps/simple.cub
	@echo "1000000001" >> maps/simple.cub
	@echo "1111111111" >> maps/simple.cub
	@echo "$(GREEN)✅ Created maps/simple.cub$(RESET)"
	@echo "$(YELLOW)⚠️  Don't forget to add your .xpm texture files to textures/$(RESET)"
	@echo "$(YELLOW)📋 Test with: ./$(NAME) maps/simple.cub$(RESET)"

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@$(MLX_MAKE) clean || true

fclean: clean
	@echo "$(RED)🗑️  Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

# Bonus rule (if you add bonus features later)
bonus: CFLAGS += -DBONUS
bonus: $(NAME)

# Debug compilation with extra flags
debug: CFLAGS += -fsanitize=address -g3
debug: re
	@echo "$(YELLOW)🐛 Debug version compiled with AddressSanitizer$(RESET)"

# Check for common issues
check:
	@echo "$(BLUE)🔍 Running basic checks...$(RESET)"
	@echo "Checking for libft..."
	@test -d $(LIBFT_DIR) && echo "$(GREEN)✅ libft directory found$(RESET)" || echo "$(RED)❌ libft directory missing$(RESET)"
	@echo "Checking for MLX..."
	@test -d $(MLX_DIR) && echo "$(GREEN)✅ MLX directory found$(RESET)" || echo "$(RED)❌ MLX directory missing$(RESET)"
	@echo "Checking for required source files..."
	@for src in $(SRCS); do \
		if [ -f $${src} ]; then \
			echo "$(GREEN)✅ $${src}$(RESET)"; \
		else \
			echo "$(RED)❌ $${src} missing$(RESET)"; \
		fi; \
	done

# Test compilation without linking (useful for checking syntax)
test-compile:
	@echo "$(BLUE)🧪 Testing compilation without linking...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCS)
	@echo "$(GREEN)✅ All files compile without errors$(RESET)"
	@rm -f *.o

# Show help
help:
	@echo "$(BLUE)🎮 cub3D Makefile Help$(RESET)"
	@echo ""
	@echo "$(YELLOW)Available targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)          - Compile the project"
	@echo "  $(GREEN)clean$(RESET)        - Remove object files"
	@echo "  $(GREEN)fclean$(RESET)       - Remove object files and executable"
	@echo "  $(GREEN)re$(RESET)           - Recompile everything"
	@echo "  $(GREEN)bonus$(RESET)        - Compile with bonus features"
	@echo "  $(GREEN)debug$(RESET)        - Compile with debug flags"
	@echo "  $(GREEN)setup$(RESET)        - Create example map and directories"
	@echo "  $(GREEN)check$(RESET)        - Check for required files"
	@echo "  $(GREEN)test-compile$(RESET) - Test compilation without linking"
	@echo "  $(GREEN)help$(RESET)         - Show this help"
	@echo ""
	@echo "$(YELLOW)Usage:$(RESET)"
	@echo "  make setup    # First time setup"
	@echo "  make          # Compile project" 
	@echo "  ./cub3D maps/simple.cub"
	@echo ""
	@echo "$(YELLOW)Required files:$(RESET)"
	@echo "  - libft/ directory with your libft"
	@echo "  - minilibx/ directory (Mac) or minilibx_linux/ directory (Linux)"
	@echo "  - .xpm texture files in textures/"

# Show project structure
structure:
	@echo "$(BLUE)📁 Project Structure:$(RESET)"
	@echo "cub3D/"
	@echo "├── Makefile"
	@echo "├── $(INCDIR)/"
	@echo "│   └── cub3d.h"
	@echo "├── $(SRCDIR)/"
	@echo "│   ├── main.c"
	@echo "│   ├── parsing/"
	@echo "│   │   ├── parse_file.c"
	@echo "│   │   └── parse_map.c"
	@echo "│   ├── graphics/"
	@echo "│   │   ├── init_mlx.c"
	@echo "│   │   └── raycasting.c"
	@echo "│   │   └── simple_draw.c"
	@echo "│   ├── controls/"
	@echo "│   │   └── events.c"
	@echo "│   └── utils/"
	@echo "│       ├── cleanup.c"
	@echo "│       └── error.c"
	@echo "├── $(LIBFT_DIR)/"
	@echo "├── $(MLX_DIR)/"
	@echo "├── maps/"
	@echo "│   └── *.cub"
	@echo "└── textures/"
	@echo "    └── *.xpm"

.PHONY: all clean fclean re bonus debug check test-compile help setup structure