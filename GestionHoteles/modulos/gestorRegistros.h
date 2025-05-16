#ifndef GESTOR_REGISTROS_H
#define GESTOR_REGISTROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>

// Estructura para registro de actividad
typedef struct {
    int id;
    int id_usuario;
    char fecha[20];  // Formato: YYYY-MM-DD HH:MM:SS
    char operacion[50];
    char detalles[255];
} RegistroActividad;

// Funciones para registrar actividad
int registrarActividad(int id_usuario, const char* operacion, const char* detalles, const char* log_file);
int obtenerUltimoIdRegistro(const char* log_file);

// Funciones para consultar registros (comunicación cliente-servidor)
void listarRegistros(SOCKET s, char* recvBuff, char* sendBuff);
void buscarRegistrosPorUsuario(SOCKET s, char* recvBuff, char* sendBuff);
void obtenerRegistrosPorFecha(SOCKET s, char* recvBuff, char* sendBuff);

// Funciones auxiliares para manejo de fechas
int str_to_date(const char* fecha_str, struct tm* fecha);
int comparar_fechas(const char* fecha1_str, const char* fecha2_str);

#endif
