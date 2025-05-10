#ifndef GESTORCLIENTES_H
#define GESTORCLIENTES_H

#include "gestorRegistros.h"
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char dni[10];
    char nombre[50];
    char apellido[50];
    char telefono[15];
    char email[50];
}Cliente;

void crearCliente(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void obtenerClientes(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void modificarCliente(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void buscarCliente(SOCKET comm_socket, char *recvBuff, char *sendBuff);

#endif /* GESTORCLIENTES_H */
