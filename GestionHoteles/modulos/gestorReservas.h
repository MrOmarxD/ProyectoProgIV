#ifndef GESTORRESERVAS_H_
#define GESTORRESERVAS_H_

typedef struct {
    int id;
    char dni_cliente[10];
    int id_habitacion;
    char fecha_entrada[11];
    char fecha_salida[11];
    float monto;
} Reserva;

void gestionReservas();

#endif /* MODULOS_GESTORRESERVAS_H_ */
