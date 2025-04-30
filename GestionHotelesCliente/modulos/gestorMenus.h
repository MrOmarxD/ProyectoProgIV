#ifndef GESTORMENUS_H
#define GESTORMENUS_H

#include <iostream>
#include <winsock2.h>
//#include "gestorUsuarios.h"
#include "gestorCliente.h"
//#include "gestorHabitaciones.h"
//#include "gestorReservas.h"
//#include "gestorFacturas.h"
#include <stdio.h>
#include <stdbool.h>




int mostrarMenuPrincipal();
char eleccionInicial();
void configuracionSistema();
int mostrarMenuPrincipalCliente(SOCKET s);



#endif /* GESTORMENUS_H */
