#ifndef GESTORCLIENTES_H
#define GESTORCLIENTES__

typedef struct {
    char dni[10];
    char nombre[50];
    char apellidos[50];
    char telefono[15];
    char email[50];
} Cliente;

void gestionClientes();

#endif /* MODULOS_GESTORCLIENTES_H_ */
