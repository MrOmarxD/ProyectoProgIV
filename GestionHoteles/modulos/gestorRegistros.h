#ifndef GESTOR_REGISTROS_H
#define GESTOR_REGISTROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>

typedef struct {
    int id;
    int id_usuario;
    char fecha[20];
    char operacion[50];
    char detalles[255];
} RegistroActividad;

int registrarActividad(int id_usuario, const char* operacion, const char* detalles, const char* log_file);

int obtenerUltimoIdRegistro(const char* log_file);

void listarRegistros(SOCKET s, char* recvBuff, char* sendBuff);

void buscarRegistrosPorUsuario(SOCKET s, char* recvBuff, char* sendBuff);

void obtenerRegistrosPorFecha(SOCKET s, char* recvBuff, char* sendBuff);

#endif
