#include <iostream>
#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>

#include "gestorMenus.h"

#ifndef MODULOS_GESTORCLIENTE_H_
#define MODULOS_GESTORCLIENTE_H_

typedef struct {
    char dni[10];
    char nombre[50];
    char apellido[50];
    char telefono[15];
    char email[50];
}Cliente;

typedef struct {
    char nombre[50];
    char rol[20];
    char usuario[20];
    char password[20];
    char turno[20];
    int salario;
} Usuario;

void mostrarClientes(SOCKET s);
void mostrarHabitaciones(SOCKET s);
void crearReserva(SOCKET s);

#endif /* MODULOS_GESTORCLIENTE_H_ */
