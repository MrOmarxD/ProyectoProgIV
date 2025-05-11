#ifndef GESTOR_FACTURAS_H
#define GESTOR_FACTURAS_H

#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
    int id;
    char id_reserva[10];
    char dni_cliente[10];
    float monto;
    char metodo_pago[20];
    char fecha[11];
    char numero_Factura[20];
    char estado[20];
}Factura;

void gestionFacturacion(SOCKET s);
void crearFactura(SOCKET s);
void buscarFactura(SOCKET s);

#endif /* GESTOR_FACTURAS_H */
