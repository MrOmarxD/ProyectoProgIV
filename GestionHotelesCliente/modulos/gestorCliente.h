#include <iostream>
#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef MODULOS_GESTORCLIENTE_H_
#define MODULOS_GESTORCLIENTE_H_



typedef struct {
    char dni[10];
    char nombre[50];
    char apellido[50];
    char telefono[15];
    char email[50];
}Cliente;

void mostrarClientes(SOCKET s);
void pedirHabitacion(SOCKET s);
void crearReserva(SOCKET s);

#endif /* MODULOS_GESTORCLIENTE_H_ */
