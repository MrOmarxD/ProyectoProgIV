#ifndef GESTORCLIENTES_H
#define GESTORCLIENTES__
#include <stdbool.h>

typedef struct {
    char dni[10];
    char nombre[50];
    char apellidos[50];
    char telefono[15];
    char email[50];
} Cliente;

void gestionClientes(int usuario_actual, char LOG_FILE, bool ejecutar);

#endif /* MODULOS_GESTORCLIENTES_H_ */
