#ifndef GESTOR_FACTURAS_H
#define GESTOR_FACTURAS_H

#include "gestorClientes.h"

// Estructura para factura
typedef struct {
    int id;
    int id_reserva;
    char dni_cliente[10];
    float monto;
    char metodo_pago[20];
    char fecha[11];
} Factura;

// Funciones para cada opción del menú de facturación
void gestionFacturacion(int usuario_actual, const char* LOG_FILE);
void generarNuevaFactura(int* usuario_actual);
void buscarFactura(int* usuario_actual);
void listarFacturasPorFecha(int* usuario_actual);
void generarInformeFacturacion(int* usuario_actual);

#endif /* GESTOR_FACTURAS_H */
