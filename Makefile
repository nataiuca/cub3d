
# Program name
NAME		= cub3D

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g #-O3
#MATHFLAGS	= -lm

# ============================================================================ #
# ⚠️  ELIMINAR PARA ENTREGA: Detección de sistema operativo
# ============================================================================ #
# Detect OS
UNAME		:= $(shell uname)

ifeq ($(UNAME), Darwin)
	# macOS - SOLO PARA DESARROLLO
	MLX_DIR		= minilibx
	MLX_FLAGS	= -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	MLX_MAKE	= make -C $(MLX_DIR)
else
	# Linux - VERSIÓN PARA CAMPUS
	MLX_DIR		= minilibx_linux
	MLX_FLAGS	= -L$(MLX_DIR) -lmlx -lX11 -lXext -lm
	MLX_MAKE	= make -C $(MLX_DIR)
endif
# ============================================================================ #
# ⬆️  REEMPLAZAR TODO EL BLOQUE DE ARRIBA POR ESTAS 3 LÍNEAS PARA ENTREGA:
# MLX_DIR		= minilibx_linux
# MLX_FLAGS	= -L$(MLX_DIR) -lmlx -lX11 -lXext -lm
# MLX_MAKE	= make -C $(MLX_DIR)
# ============================================================================ #

# ============================================================================ #
# OPCIÓN MLX42 DE CODAM
MLX42_DIR= ./MLX42
MLX42_INC= $(MLX42_DIR)/include/MLX42
MLX42_NAME= $(MLX42_DIR)/build/libmlx42.a
MLX42_FLAGS= -ldl -lglfw -pthread -lm

# ============================================================================ #

# Directories
SRCDIR		= src
OBJDIR		= obj
INCDIR		= inc
#LIBFT_DIR	= libft

# Source files - VERSIÓN OPTIMIZADA
SOURCES		= main.c \
			  parsing/read_file.c \
			  parsing/config_parser.c \
			  parsing/map_parser.c \
			  parsing/config_validator.c \
			  parsing/map_validator.c \
			  parsing/parser_utils.c \
			  engine/raycasting.c \
			  engine/player.c \
			  init_structs.c \
			  utils/cleanup.c \
			  utils/error.c \
			  graphics/init_mlx.c
#			  graphics/render.c \
			  graphics/draw.c \
			  controls/events.c \
			  controls/movement.c \
			  
#			  parsing/parse_map.c \

# Add src/ prefix
SRCS		= $(addprefix $(SRCDIR)/, $(SOURCES))

# Object files
OBJECTS		= $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Header files
INCLUDES	= -I $(INCDIR) -I $(MLX42_INC) #-I $(MLX_DIR) -I $(LIBFT_DIR)

# Libraries
LIBFT		= $(INCDIR)/libft.a #$(LIBFT_DIR)/libft.a

# Colors
GREEN		= \033[0;32m
RED			= \033[0;31m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
RESET		= \033[0m

# Progress
TOTAL		= $(words $(SOURCES))
CURRENT		= 0

# ============================================================================ #
# RULES
# ============================================================================ #

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT) $(MLX42_NAME)
	@echo "$(BLUE)🔨 Compiling MLX42 in $(MLX42_DIR)...$(RESET)"
#	@$(MLX_MAKE) 2>/dev/null || true
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

#$(LIBFT):
#	@echo "$(BLUE)📚 Compiling libft...$(RESET)"
#	@make -C $(LIBFT_DIR) --no-print-directory
#	@echo "$(GREEN)✅ libft compiled$(RESET)"

$(MLX42_NAME):
	@cmake -S $(MLX42_DIR) -B $(MLX42_DIR)/build
	@cmake --build $(MLX42_DIR)/build -j4

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@if [ -d $(OBJDIR) ]; then rm -rf $(OBJDIR); fi
	@rm -rf $(MLX42_DIR)/build
#	@make -C $(LIBFT_DIR) clean --no-print-directory
#	@$(MLX_MAKE) clean 2>/dev/null || true

fclean: clean
	@echo "$(RED)🗑️  Removing executable...$(RESET)"
	@rm -f $(NAME)
#	@make -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

#A PARTIR DE AQUÍ SON COSAS DE TEST, BORRAR PARA ENTREGA, NO??

# Norminette check
norm:
	@echo "$(BLUE)📏 Checking norminette...$(RESET)"
	@norminette $(SRCDIR) $(INCDIR) $(LIBFT_DIR) | grep -v "OK!" || true
	@echo "$(GREEN)✅ Norminette check complete$(RESET)"

# Debug version with sanitizer
debug: CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
debug: re
	@echo "$(YELLOW)🐛 Debug version with AddressSanitizer$(RESET)"

# Fast version (maximum optimization)
fast: CFLAGS = -Wall -Wextra -Werror -O3 -march=native
fast: re
	@echo "$(GREEN)🚀 Ultra-fast version compiled$(RESET)"

# Test map setup
setup:
	@echo "$(BLUE)🗂️  Creating test environment...$(RESET)"
	@mkdir -p maps textures
	@echo "NO ./textures/north.xpm" > maps/test.cub
	@echo "SO ./textures/south.xpm" >> maps/test.cub
	@echo "WE ./textures/west.xpm" >> maps/test.cub
	@echo "EA ./textures/east.xpm" >> maps/test.cub
	@echo "" >> maps/test.cub
	@echo "F 220,100,0" >> maps/test.cub
	@echo "C 135,206,235" >> maps/test.cub
	@echo "" >> maps/test.cub
	@echo "        1111111111111111111111111" >> maps/test.cub
	@echo "        1000000000110000000000001" >> maps/test.cub
	@echo "        1011000001110000000000001" >> maps/test.cub
	@echo "        1001000000000000000000001" >> maps/test.cub
	@echo "111111111011000001110000000000001" >> maps/test.cub
	@echo "100000000011000001110111111111111" >> maps/test.cub
	@echo "11110111111111011100000010001" >> maps/test.cub
	@echo "11110111111111011101010010001" >> maps/test.cub
	@echo "11000000110101011100000010001" >> maps/test.cub
	@echo "10000000000000001100000010001" >> maps/test.cub
	@echo "10000000000000001101010010001" >> maps/test.cub
	@echo "11000001110101011111011110N0111" >> maps/test.cub
	@echo "11110111 1110101 101111010001" >> maps/test.cub
	@echo "11111111 1111111 111111111111" >> maps/test.cub
	@echo "$(GREEN)✅ Test map created: maps/test.cub$(RESET)"
	@echo "$(YELLOW)⚠️  Add your .xpm textures to textures/$(RESET)"

# Performance test
perf: $(NAME)
	@echo "$(BLUE)⚡ Running performance test...$(RESET)"
	@time ./$(NAME) maps/test.cub
	@echo "$(GREEN)✅ Performance test complete$(RESET)"

# Valgrind memory check (Linux only)
valgrind: debug
	@echo "$(BLUE)🔍 Running valgrind...$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes ./$(NAME) maps/test.cub

# ============================================================================ #
# PREPARACIÓN PARA ENTREGA
# ============================================================================ #
prepare-submission:
	@echo "$(YELLOW)🚨 PREPARANDO PARA ENTREGA EN CAMPUS$(RESET)"
	@echo ""
	@echo "$(RED)⚠️  MANUAL CHECKLIST:$(RESET)"
	@echo "1. Eliminar detección de OS en Makefile (líneas 26-41)"
	@echo "2. Eliminar macros de Mac en inc/cub3d.h"
	@echo "3. Verificar que MLX_DIR = minilibx_linux"
	@echo "4. Ejecutar: make norm"
	@echo "5. Ejecutar: make fclean && make"
	@echo "6. Probar en ordenador del campus"
	@echo ""
	@echo "$(BLUE)📝 Verificando norminette...$(RESET)"
	@norminette $(SRCDIR) $(INCDIR) 2>&1 | grep -E "Error|Warning" || \
		echo "$(GREEN)✅ Norminette OK$(RESET)"
	@echo ""
	@echo "$(BLUE)🔍 Archivos que necesitan revisión manual:$(RESET)"
	@echo "- $(YELLOW)Makefile$(RESET) (eliminar bloques marcados con ⚠️)"
	@echo "- $(YELLOW)inc/cub3d.h$(RESET) (eliminar #ifdef __APPLE__)"

help:
	@echo "$(BLUE)🎮 cub3D Makefile Help$(RESET)"
	@echo ""
	@echo "$(YELLOW)Development (Mac):$(RESET)"
	@echo "  $(GREEN)make$(RESET)              - Compile project"
	@echo "  $(GREEN)make setup$(RESET)        - Create test map"
	@echo "  $(GREEN)make debug$(RESET)        - Compile with sanitizer"
	@echo "  $(GREEN)make fast$(RESET)         - Compile with max optimization"
	@echo ""
	@echo "$(YELLOW)Testing:$(RESET)"
	@echo "  $(GREEN)make norm$(RESET)         - Check norminette"
	@echo "  $(GREEN)make perf$(RESET)         - Performance test"
	@echo "  $(GREEN)make valgrind$(RESET)     - Memory leak check (Linux)"
	@echo ""
	@echo "$(YELLOW)Pre-submission:$(RESET)"
	@echo "  $(GREEN)make prepare-submission$(RESET) - Checklist para entrega"
	@echo ""
	@echo "$(YELLOW)Cleaning:$(RESET)"
	@echo "  $(GREEN)make clean$(RESET)        - Remove objects"
	@echo "  $(GREEN)make fclean$(RESET)       - Remove objects + executable"
	@echo "  $(GREEN)make re$(RESET)           - Recompile everything"

.PHONY: all clean fclean #re norm debug fast setup perf valgrind \
		prepare-submission help
