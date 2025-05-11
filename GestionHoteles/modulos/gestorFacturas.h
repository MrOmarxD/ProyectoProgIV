#ifndef GESTOR_FACTURAS_H
#define GESTOR_FACTURAS_H

#include "gestorClientes.h"
#include "gestorRegistros.h"
#include <winsock2.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

void generarNuevaFactura(int* usuario_actual);
void buscarFactura(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void crearFactura(SOCKET comm_socket, char *recvBuff, char *sendBuff);

#endif /* GESTOR_FACTURAS_H */
