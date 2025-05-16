#ifndef MODULOS_GESTORREGISTRO_H_
#define MODULOS_GESTORREGISTRO_H_

#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>

using namespace std;

// Funciones para gestionar registros desde el menú
void mostrarMenuRegistros(SOCKET s);
void listarRegistros(SOCKET s);
void buscarRegistrosPorUsuario(SOCKET s);
void obtenerRegistrosPorFecha(SOCKET s);



#endif /* MODULOS_GESTORREGISTRO_H_ */
