#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H
#include <stdbool.h>
#include "gestorRegistros.h"
//#include "../bd/gestionBD.h"
#include <stdio.h>
#include "gestorMenus.h"

typedef struct {
    int id;
    char nombre[50];
    char rol[20];
    char usuario[20];
    char password[20];
} Usuario;

void gestionUsuarios(int usuario_actual, const char* LOG_FILE);
void crearUsuario(Usuario *user);

#endif /* GESTORUSUARIOS_H */
