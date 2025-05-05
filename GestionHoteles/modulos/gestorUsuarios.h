#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H
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

void crearUsuario(Usuario *user);
void modificarUsuario(Usuario *user);
void buscarUsuario(Usuario *usuario);

#endif /* GESTORUSUARIOS_H */
