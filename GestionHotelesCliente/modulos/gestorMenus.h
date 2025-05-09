#ifndef GESTORMENUS_H
#define GESTORMENUS_H

#include <iostream>
#include <winsock2.h>
#include "gestorUsuarios.h"
#include "gestorCliente.h"
#include "gestorClienteAdmin.h"
#include "gestorHabitaciones.h"
#include "gestorReservas.h"
#include "gestorFacturas.h"
#include <stdio.h>
#include <stdbool.h>
#include "config.h"

// Variable global para la configuración
extern ConfigData g_config;  // Declarar como extern para ser visible en todo el programa
extern char CONFIG_FILE[];   // Ruta al archivo de configuración

int mostrarMenuPrincipal(SOCKET s);
char eleccionInicial();
void configuracionSistema(SOCKET s);
int mostrarMenuPrincipalCliente(SOCKET s);

// Añadir funciones para la configuración del sistema
void cambiarRutasArchivos(SOCKET s);
void configurarConexion();
void hacerCopiaSeguridad();
void restaurarCopiaSeguridad();

#endif /* GESTORMENUS_H */
