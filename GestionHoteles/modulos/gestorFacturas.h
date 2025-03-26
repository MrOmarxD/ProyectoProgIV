#ifndef GESTORFACTURAS_H_
#define GESTORFACTURAS_H_

typedef struct {
    int id;
    int id_reserva;
    char dni_cliente[10];
    float monto;
    char metodo_pago[20];
    char fecha[11];
} Factura;

void gestionFacturacion(int usuario_actual, char LOG_FILE);

#endif /* MODULOS_GESTORFACTURAS_H_ */
