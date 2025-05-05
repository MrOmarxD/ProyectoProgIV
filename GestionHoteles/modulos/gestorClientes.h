#ifndef GESTORCLIENTES_H
#define GESTORCLIENTES_H

#include "gestorRegistros.h"
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

void crearCliente(Cliente *client);
void modificarCliente(char *dni_recibido, int socket_cliente);
void buscarCliente(Cliente *client);

#endif /* GESTORCLIENTES_H */
