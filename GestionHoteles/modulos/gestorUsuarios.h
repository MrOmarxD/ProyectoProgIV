#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H
#include <stdbool.h>

typedef struct {
    int id;
    char nombre[50];
    char rol[20];
    char usuario[20];
    char password[20];
} Usuario;

void gestionUsuarios(int usuario_actual, char LOG_FILE, bool ejecutar);

#endif /* MODULOS_GESTORUSUARIOS_H */
