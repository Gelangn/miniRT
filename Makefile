
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
S = src/

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
SRC = $(wildcard $(S)*.c) 

# Archivos objeto
OBJ = $(SRC:.c = .o)

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
	CFLAGS = -Wall -Wextra -Werror -I./inc -I./lib/minilibx-linux -O3 -g3 -fsanitize=address
	FFLAGS = -L./lib/minilibx-linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz
#-L$(LIB) -lft -L$(LIB_MLX) -lmlx -lXext -lX11 -lm -lbsd
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
all: $(NAME)

# Regla de compilación
%.o: %.c 
	@$(CC) $(CFLAGS) -o $@ -c $<

# Regla para construir el archivo estático
$(NAME): $(OBJ)
	@echo "$(BLUE)*** PASO 1 - Comienzo compilación libft ***$(DEFAULT)"
	@make -C $(LIB) all
	@echo
#	sleep 3
	@echo "$(BLUE)*** PASO 2 - Comienzo compilación BONUS libft ***$(DEFAULT)"
	@make -C $(LIB) bonus
	@echo
#	sleep 3
	@echo "$(GREEN)*** Compilación libft completada ***$(DEFAULT)"
	@echo
	@echo "$(BLUE)*** PASO 3 - Comienzo compilación mlx ***$(DEFAULT)"
	@make -C $(LIB_MLX)
#	sleep 3
	@echo
	@echo
	@echo "$(YELLOW)*** Creando ejecutable ***$(DEFAULT)"
	@echo
#	sleep 5
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(DEPS) $(FFLAGS)
	@echo
	@echo "$(GREEN)*** Compilación completada ***$(DEFAULT)"
	@echo

# Regla para limpiar archivos objeto
clean:
	@make -C $(LIB) clean
	@make -C $(LIB_MLX) clean

# Regla para limpiar archivos objeto y el archivo estático
fclean: clean
	@make -C $(LIB) fclean
	@make -C $(LIB_MLX) clean
	@rm -f $(NAME)

# Regla para recompilar
re: fclean all

.PHONY: all clean fclean re
