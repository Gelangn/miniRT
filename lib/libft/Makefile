#########
# libft #
#########
# Path: Cursus42/.../libft/Makefile

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
NAME = libft.a

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Werror -Wextra

# Archivos fuente incluido get_next_line
SRC = 	ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_strchr.c \
		ft_strcmp.c\
		ft_strrchr.c \
		ft_strncmp.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_atof.c \
		ft_calloc.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		get_next_line.c \
		get_next_line_utils.c \
		ft_printf.c \
		ft_str_nbr.c \
		ft_hex.c \
		ft_trim_str.c

BONUS_SRC = 	ft_lstnew_bonus.c \
				ft_lstadd_front_bonus.c \
				ft_lstsize_bonus.c \
				ft_lstlast_bonus.c \
				ft_lstadd_back_bonus.c \
				ft_lstdelone_bonus.c \
				ft_lstclear_bonus.c \
				ft_lstiter_bonus.c \
				ft_lstmap_bonus.c

# Archivos objeto
OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

# Dependencias
DEPS = libft.h get_next_line.h ft_printf.h

# *** Reglas ***

# Regla por defecto
all: $(NAME)

# Regla de compilación
%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -o $@ -c $<

# Regla para construir el archivo estático
$(NAME): $(OBJ)
#	@echo "\033[H\033[J"
	@echo "$(RED)*** Compilando libft ***$(DEFAULT)"
	@gcc $(CFLAGS) -c $(SRC)
	@ar rcs $@ $^

# Regla para construir el archivo estático con bonus
bonus:
#	@echo "\033[H\033[J"
	@echo "$(RED)*** Compilando BONUS libft ***$(DEFAULT)"
	@gcc $(CFLAGS) -c $(BONUS_SRC)
	@ar rcs $(NAME) $(BONUS_OBJ)

# Regla para limpiar archivos objeto
clean:
#	@echo "\033[H\033[J"
	@echo "$(RED)*** Limpiando LIBFT ***$(DEFAULT)"
	@rm -f $(OBJ) $(BONUS_OBJ)
#	@echo "\033[H\033[J"
	@echo "$(RED)*** LIBFT Limpiada ***$(DEFAULT)"

# Regla para limpiar archivos objeto y el archivo estático
fclean: clean
	@echo
	@rm -f $(NAME)

# Regla para recompilar
re: fclean all

.PHONY: all clean fclean re
