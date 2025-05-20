#ifndef GESTOR_REGISTRO_H
#define GESTOR_REGISTRO_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <winsock2.h>

// Incluir el archivo de configuración para acceder a la estructura ConfigData
#include "config.h"

// Declaración de la variable de configuración global
extern ConfigData g_config;

// Declaración de funciones
void mostrarMenuRegistros(SOCKET s);
void listarRegistros(SOCKET s);
void buscarRegistrosPorUsuario(SOCKET s);
void obtenerRegistrosPorFecha(SOCKET s);

// Función para registrar actividad
void registrarActividadCliente(int usuario_id, const char* tipo_actividad, const char* descripcion, const char* archivo_registro);

#endif // GESTOR_REGISTRO_
