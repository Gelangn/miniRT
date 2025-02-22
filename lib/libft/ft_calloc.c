/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:08:43 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/10 19:23:44 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;

	p = malloc(count * size * sizeof(char));
	if (!p)
		return (NULL);
	ft_bzero(p, count * size);
	return (p);
}

/* int	main(void)
{
	int	*arr;
	int	num_elements;

	num_elements = 5;
	// Asignar memoria para un array de 5 enteros e inicializarlos a cero
	arr = (int *)calloc(num_elements, sizeof(int));
	if (arr == NULL) {
		perror("Error al asignar memoria.");
		exit(EXIT_FAILURE);
	}
	// Ahora 'arr' es un array de 5 enteros inicializados a cero
	for (int i = 0; i < num_elements; i++) {
		printf("%d ", arr[i]); // Imprime "0 0 0 0 0 "
	}
	// Liberar la memoria asignada con calloc
	free(arr);
	return (0);
} */
