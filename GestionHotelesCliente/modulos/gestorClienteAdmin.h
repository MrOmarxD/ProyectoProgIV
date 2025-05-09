#ifndef MODULOS_GESTORCLIENTEADMIN_H_
#define MODULOS_GESTORCLIENTEADMIN_H_

#include "gestorMenus.h"
#include "gestorCliente.h"

#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"



void gestionClienteAdmin(SOCKET s);
void crearClienteAd(SOCKET s);
void modificarClienteAd(SOCKET s);
void listarClientesAd(SOCKET s);
void buscarClienteAd(SOCKET s);



#endif /* MODULOS_GESTORCLIENTEADMIN_H_ */
