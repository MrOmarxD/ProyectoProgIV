#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

//#include "gestorRegistros.h"
#include "gestorMenus.h"

#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"

void gestionUsuarios(SOCKET s);
void crearUsuario(SOCKET s);
void eliminarUsuario(SOCKET s);
void listaUsuarios(SOCKET s);

#endif /* GESTORUSUARIOS_H */
