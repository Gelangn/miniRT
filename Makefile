# Nombre del proyecto (ejecutable)
NAME = miniRT

# gcc lib/libft/get_next_line.c lib/libft/get_next_line_utils.c src/fdf.c src/main.c -Wall -Wextra -Werror -Imlx -O3 -Llib/mlx -lmlx -framework OpenGL -framework AppKit -g3 -fsanitize=address -o fdf
# gcc lib/libft/*.c lib/libft/*.h lib/mlx/mlx.h inc/fdf.h src/*.c -Wall -Wextra -Werror -Imlx -O3 -Llib/mlx -lmlx -framework OpenGL -framework AppKit -g3 -fsanitize=address (Error al iniciar ./a.out ERROR Usage: ./fdf file )

# Compilador y banderas
CC = gcc
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
	CFLAGS = -Wall -Wextra -Werror -Ilib/lib/minilibx_mms_20200219_beta -O3
	 -g3 -fsanitize=address
	FFLAGS = -Lmlx_linux -lmlx_Linux -L/usr/lib
	 -Ilib/lib/minilibx_mms_20200219_beta -lXext -lX11 -lm -lz
else ifeq ($(OS),MacOS)
#	@echo "Compilando en MacOS"
	CFLAGS = -Wall -Wextra -Werror -Imlx
	FFLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit
else
#	@echo "Compilando en otro sistema operativo"
	CFLAGS = -Wall -Wextra -Werror -Imlx
	FFLAGS = -Lmlx -lmlx -lX11 -lXext -lm
endif

# Seudonimos carpetas, ejemplo uso: $S main.c
S = src/

# Otros seudonimos
LIB = lib/libft

# Lista de archivos fuente
SRC = *.c 
OBJ = $S$(SRC:.c = .o)

# Dependencias
DEPS = $(LIB)/libft.a
ifeq ($(OS),Darwin)
	MLX = lib/mlx
else
	MLX = lib/mlx_linux
endif

# Reglas

# Regla por defecto
all: $(NAME)
	@echo
	@echo "*** Compilación completada ***"
	@echo
# Regla de compilación
%.o: %.c 
	@$(CC) $(CFLAGS) -o $@ -c $<

# Regla para construir el archivo estático
$(NAME): $(OBJ)
	@echo
	@echo "*** Compilando libft ***"
	@echo
	@make -C $(LIB) all
	@make -C $(LIB) bonus
	@echo
	@echo "*** Compilando mlx ***"
	@echo
	@make -C $(MLX) 2>/dev/null
	@echo
	@echo "*** Creando ejecutable ***"
	@echo
# mv $(LIB) #$(NAME)
	@$(CC) $(OBJ) $(CFLAGS) $(FFLAGS) -L$(LIB) -lft -o $(NAME)
# ar rcs $@ $^

# Regla para limpiar archivos objeto
clean:
	@make -C $(LIB) clean
	@make -C $(MLX) clean

# Regla para limpiar archivos objeto y el archivo estático
fclean: clean
	@make -C $(LIB) fclean
	@make -C $(MLX) clean
	@rm -f $(NAME) 

# Regla para recompilar
re: fclean all

.PHONY: all clean fclean re
