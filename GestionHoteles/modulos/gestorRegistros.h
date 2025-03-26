#ifndef GESTORREGISTROS_H_
#define GESTORREGISTROS_H_
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int id;
    int id_usuario;
    char fecha[20];
    char operacion[100];
} Registro;

void registrarActividad(int id_usuario, const char* operacion, char LOG_FILE);
void verRegistrosActividad(int usuario_actual, char LOG_FILE);
bool guardarDatos(const char* archivo, void* datos, size_t tamano, int cantidad);
bool cargarDatos(const char* archivo, void* datos, size_t tamano, int* cantidad);

#endif /* MODULOS_GESTORREGISTROS_H_ */
