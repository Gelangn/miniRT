# Colours
DEFAULT 	= \033[0m
GREEN 		= \033[38;5;46m
WHITE 		= \033[38;5;15m
GREY 		= \033[38;5;8m
ORANGE 		= \033[38;5;202m
RED 		= \033[38;5;160m
YELLOW 		= \033[38;5;226m
BLUE 		= \033[38;5;27m

# Nombre del proyecto (ejecutable)
NAME = miniRT

# Compilador y banderas
CC = gcc

# Seudonimos carpetas, ejemplo uso: $S main.c
SRC_DIR = src/
OBJ_DIR = obj/

# Otros seudonimos
LIBS = lib
LIB = $(LIBS)/libft

# Dependencias
ifeq ($(OS),Darwin)
	LIB_MLX = $(LIBS)/minilibx_mms_20200219
else
	LIB_MLX = $(LIBS)/minilibx-linux
endif
DEPS = $(LIB)/libft.a $(LIB_MLX)/libmlx.a

# Archivos fuente
SRC_FILES = 	camera_controls.c \
				color.c \
				events_handle.c \
				events_mouse.c \
				events_rotation.c \
				events_screenshot.c \
				events_utils.c \
				freeing.c \
				init_scene.c \
				matrix_operations.c \
				matrix.c \
				minirt.c \
				parser_objects.c \
				parser_scene.c \
				parser_scene_utils.c \
				parser_utils.c \
				raytracer_advanced.c \
				raytracer_color.c \
				raytracer_core.c \
				raytracer_cylinder_caps.c \
				raytracer_cylinder.c \
				raytracer_inside_test.c \
				raytracer_intersecs.c \
				raytracer_lighting.c \
				raytracer_normals.c \
				raytracer_objects.c \
				raytracer_properties.c \
				raytracer_reflection.c \
				raytracer_refractor.c \
				raytracer_render.c \
				render.c \
				save.c \
				scene_reader.c \
				shadows.c \
				signal_handlers.c \
				utils.c \
				window.c

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

# Archivos objeto
OBJ_FILES = $(SRC_FILES:.c=.o)

OS := $(shell uname)
ifeq ($(OS),Windows_NT)
#	@echo "Compilando en Windows NT"
	CFLAGS = -Wall -Werror -Wextra -Imlx
	FFLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit
else ifeq ($(OS),Darwin)
#	@echo "Compilando en Darwin"
	CFLAGS = -Wall -Wextra -Werror -Imlx
	FFLAGS = -Llib/mlx -lmlx -framework OpenGL -framework AppKit
else ifeq ($(OS),Linux)
#	@echo "Compilando en Linux"
	CFLAGS = -Wall -Wextra -Werror -I./inc -I./lib/minilibx-linux -O0 -g0
	# Flags normales (sin sanitize)
	FFLAGS = -L./lib/minilibx-linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz
	# Flags con sanitize
	SANITIZE_FLAGS = -g3 -O3 -fsanitize=address
else ifeq ($(OS),MacOS)
#	@echo "Compilando en MacOS"
	CFLAGS = -Wall -Wextra -Werror -Imlx
	FFLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit
else
#	@echo "Compilando en otro sistema operativo"
	CFLAGS = -Wall -Wextra -Werror -Imlx
	FFLAGS = -Lmlx -lmlx -lX11 -lXext -lm
endif

# *** Reglas ***

# Regla por defecto
all: libs obj_dir $(NAME)

obj_dir:
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)*** Creando directorio de objetos: $(OBJ_DIR) ***$(DEFAULT)"

# Regla de compilación
$(OBJ_DIR)%.o:$(SRC_DIR)%.c 
	@$(CC) $(CFLAGS) -o $@ -c $<
#	@echo "\033[H\033[J"
	@echo "$(GREEN)*** Compilación de $< completada ***$(DEFAULT)"

# Regla para construir el archivo estático
$(NAME): $(OBJ)
#	@echo "\033[H\033[J"
	@echo "$(YELLOW)\n*** Creando ejecutable ***$(DEFAULT)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(DEPS) $(FFLAGS)
	@echo "$(GREEN)*** Compilación completada ***$(DEFAULT)"

libs:
	@echo "\033[H\033[J"
	@echo "$(GREY)*** Compilación de las librerías ***$(DEFAULT)"
#	@echo "\033[H\033[J"
	@echo "$(BLUE)*** PASO 1 - Comienzo compilación LIBFT ***$(DEFAULT)"
	@make --no-print-directory -C $(LIB) all >/dev/null
#	@echo "\033[H\033[J"
	@echo "$(BLUE)*** PASO 2 - Comienzo compilación BONUS LIBFT ***$(DEFAULT)"
	@make --no-print-directory -C $(LIB) bonus >/dev/null
	@echo "$(GREEN)*** Compilación libft completada ***$(DEFAULT)"
#	@echo "\033[H\033[J"
	@echo "$(BLUE)*** PASO 3 - Comienzo compilación MLX ***$(DEFAULT)"
	@make --no-print-directory -C $(LIB_MLX) 2>/dev/null >/dev/null
	@echo "$(GREEN)*** Compilación de las librerías completada ***$(DEFAULT)"

# Regla para compilar en modo DEBUG
debug:
	@echo
	@echo "$(GREY)*** Compilación en modo DEBUG ***$(DEFAULT)"
	@echo
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(DEPS) $(FFLAGS)
	@echo
	@echo "$(GREEN)*** Compilación completada ***$(DEFAULT)"
	@echo

sanitize:
	@echo "\033[H\033[J"
	@echo "$(GREY)*** Compilación en modo SANITIZE ***$(DEFAULT)"
	@echo "\033[H\033[J"
	@echo "$(BLUE)*** PASO 1 - Comienzo compilación libft ***$(DEFAULT)"
	@make -C $(LIB) all
	@echo "\033[H\033[J"
	@echo "$(BLUE)*** PASO 2 - Comienzo compilación BONUS libft ***$(DEFAULT)"
	@make -C $(LIB) bonus
	@echo
	@echo "$(GREEN)*** Compilación libft completada ***$(DEFAULT)"
	@echo "\033[H\033[J"
	@echo "$(BLUE)*** PASO 3 - Comienzo compilación mlx ***$(DEFAULT)"
	@make -C $(LIB_MLX)
	@echo
	@echo "$(YELLOW)*** Creando ejecutable con AddressSanitizer ***$(DEFAULT)"
	@echo
	@$(CC) $(CFLAGS) $(SANITIZE_FLAGS) -o $(NAME) $(SRC) $(DEPS) $(FFLAGS)
	@echo
	@echo "$(GREEN)*** Compilación con sanitize completada ***$(DEFAULT)"
	@echo

# Regla para limpiar archivos objeto
clean:
	@echo "\033[H\033[J"
	@make -C $(LIB) clean
	@make -C $(LIB_MLX) clean
	@rm -rf $(OBJ_DIR)

# Regla para limpiar archivos objeto y el archivo estático
fclean: clean
	@make -C $(LIB) fclean
	@make -C $(LIB_MLX) clean
	@rm -f $(NAME)

# Regla para recompilar
re: fclean all

.PHONY: all debug sanitize clean fclean re
