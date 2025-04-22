#ifndef GESTOR_FACTURAS_H
#define GESTOR_FACTURAS_H

#include "gestorClientes.h"
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

void gestionFacturacion(int usuario_actual, const char* LOG_FILE);
void generarNuevaFactura(int* usuario_actual);
void buscarFactura(int* usuario_actual);

#endif /* GESTOR_FACTURAS_H */
