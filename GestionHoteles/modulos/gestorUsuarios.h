#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

#include <winsock2.h>
#include <stdbool.h>
#include "gestorRegistros.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char nombre[50];
    char rol[20];
    char turno[20];
    int salario;
    char usuario[20];
    char password[20];
}Usuario;

void crearUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void eliminarUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void modificarUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void buscarUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void listarUsuarios(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void iniciarSesion(SOCKET comm_socket, char *recvBuff, char *sendBuff);

#endif /* GESTORUSUARIOS_H */
