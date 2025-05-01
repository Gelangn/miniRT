/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:00:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/01 22:37:03 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include "../inc/minirt.h"
#include <signal.h>
#include <string.h> /* For memset */
#include <unistd.h> /* For POSIX definitions */

static t_global	*g_global = NULL;

/* Signal handler function */
void	handle_signal(int signum)
{
	char	*message;

	// Seleccionar mensaje apropiado según la señal
	if (signum == SIGINT)
		message = "\nPrograma interrumpido por usuario (Ctrl+C)";
	else if (signum == SIGTERM)
		message = "\nPrograma terminado externamente";
	else if (signum == SIGQUIT)
		message = "\nSeñal de salida recibida (Ctrl+\\)";
	else if (signum == SIGSEGV)
		message = "\nError de segmentación detectado";
	else if (signum == SIGFPE)
		message = "\nExcepción de punto flotante";
	else
		message = "\nSeñal no manejada recibida";
	// Usar finish() para limpieza consistente
	finish(g_global, message);
	// Nunca llegaremos aquí, pero por si acaso
	exit(128 + signum);
}

/* Initialize signal handlers */
void	init_signal_handlers(t_global *global)
{
	struct sigaction sa;

	// Inicializar completamente la estructura sigaction para evitar bytes no inicializados
	memset(&sa, 0, sizeof(struct sigaction));

	g_global = global;
	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);

	sa.sa_flags = SA_RESTART | SA_NODEFER;

	sigaction(SIGINT, &sa, NULL); /* Ctrl+C */
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL); /* Ctrl+\ */
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGFPE, &sa, NULL);

	// Desbloquear SIGINT explícitamente
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}