#ifndef GESTORUSUARIOS_H_
#define GESTORUSUARIOS_H_
#include <stdbool.h>
#include "gestorRegistros.h"
#include "menuPrincipal.h"
#include "../bd/gestionBD.h"
#include <stdio.h>

typedef struct {
    int id;
    char nombre[50];
    char rol[20];
    char usuario[20];
    char password[20];
} Usuario;

void gestionUsuarios(int usuario_actual, const char* LOG_FILE);

#endif /* MODULOS_GESTORUSUARIOS_H */
