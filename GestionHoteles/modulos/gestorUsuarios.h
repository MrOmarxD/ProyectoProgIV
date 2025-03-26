#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

typedef struct {
    int id;
    char nombre[50];
    char rol[20];
    char usuario[20];
    char password[20];
} Usuario;

void gestionUsuarios();

#endif /* MODULOS_GESTORUSUARIOS_H */
