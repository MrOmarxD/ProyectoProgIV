#ifndef GESTORRESERVAS_H_
#define GESTORRESERVAS_H_
#include <stdbool.h>

typedef struct {
    int id;
    char dni_cliente[10];
    int id_habitacion;
    char fecha_entrada[11];
    char fecha_salida[11];
    float monto;
} Reserva;

void gestionReservas(int usuario_actual, char LOG_FILE, bool ejecutar);

#endif /* MODULOS_GESTORRESERVAS_H_ */
