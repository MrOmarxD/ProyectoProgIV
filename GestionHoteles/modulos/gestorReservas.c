#include "gestorReservas.h"
#include "gestorRegistros.h"
#include <stdio.h>
#include "gestorMenus.h"

void gestionReservas(int usuario_actual, const char* LOG_FILE) {
    int opcion;
    Reserva r;

    printf("\n--- GESTIÓN DE RESERVAS ---\n");
    printf("1. Crear nueva reserva\n");
    printf("2. Modificar reserva\n");
    printf("3. Borrar reserva\n");
    printf("4. Buscar reservas por cliente\n");
    printf("5. Listar reservas activas\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    switch (opcion) {
                case 1:
                	crearNuevaReserva(&r);
                    break;
                case 2:
                	modificarReserva(&r);
                    break;
                case 3:
                	eliminarReservaBD();
                    break;
                case 4:
                	buscarReservas(&r);
                    break;
                case 5:
                	listarReserva();
                    break;
                case 0:
                	mostrarMenuPrincipal();
                	break;
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de reservas", LOG_FILE);

}

void crearNuevaReserva(Reserva *reserva) {
        printf("Ingrese id cliente: ");
        fflush(stdout);

        while (getchar() != '\n');

        fgets(reserva->dni_cliente, 50, stdin);
        reserva->dni_cliente[strcspn(reserva->dni_cliente, "\n")] = '\0';

        printf("Ingrese id habitacion: ");
        	fflush(stdout);
        	char id_habita[20];
        	fgets(id_habita, 20, stdin);
        	id_habita[strcspn(id_habita, "\n")] = '\0'; // Eliminar el salto de línea
        	sscanf(id_habita, "%d", &reserva->id_habitacion);

        printf("Ingrese fecha (AAAA/MM/DD): ");
        fflush(stdout);

           fgets(reserva->fecha_entrada, 11, stdin);
           reserva->fecha_entrada[strcspn(reserva->fecha_entrada, "\n")] = '\0';

           printf("Fecha de fin (AAAA/MM/DD): ");
           fflush(stdout);

           while (getchar() != '\n');

           fgets(reserva->fecha_salida, 11, stdin);
           reserva->fecha_salida[strcspn(reserva->fecha_salida, "\n")] = '\0';



        int estado;
       	do {
       		printf("Elija el estado de la habitacion\n");
       		printf("1. Confirmada\n");
       		printf("2. Pendiente\n");
       		printf("Seleccione el estado: ");
       		fflush(stdout);
       		scanf("%d", &estado);

       		// Limpiar el buffer de entrada
       		while (getchar() != '\n');

       		switch(estado) {
       			case 1:
       				printf("\nHa seleccionado: Confirmada\n\n");
       				fflush(stdout);
       				strcpy(reserva->estado, "Confirmada");
       				break;
       			case 2:
       				printf("\nHa seleccionado: Pendiente\n\n");
       				fflush(stdout);
       				strcpy(reserva->estado, "Pendiente");
       				break;

       			default:
       				printf("\nEstado no valido. Por favor, intente de nuevo.\n");
       				fflush(stdout);
       				estado = 0;
       				break;
       			}
             } while(estado == 0);

        	printf("Ingrese monto: ");
              	fflush(stdout);
              	char montoString[20];
              	fgets(montoString, 20, stdin);
              	montoString[strcspn(montoString, "\n")] = '\0'; // Eliminar el salto de línea
              	sscanf(montoString, "%d", &reserva->monto);

              	printf("Ingrese la observacion: \n");
              			fflush(stdout);

              			fgets(reserva->observaciones, 100, stdin);
              			reserva->observaciones[strcspn(reserva->observaciones, "\n")] = '\0';



        crearReservaBD(reserva);
}
void modificarReserva(Reserva *r){

	char idUReserva[10];

	printf("\n--- MODEFICAR RESERVA ---\n");
	printf("Ingrese el id de la reserva a modificar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(idUReserva, 10, stdin);
	idUReserva[strcspn(idUReserva, "\n")] = '\0';

	if (!recuperarReservaBD(idUReserva, r)) {
		return;
	}

	printf("Reserva encontrada. Dejar en blanco para no modificar.\n");

	printf("Ingrese nuevo id usuario: ");
		fflush(stdout);
		char nuevoId[20];


		fgets(nuevoId, 20, stdin);
		nuevoId[strcspn(nuevoId, "\n")] = '\0';
		if (strlen(nuevoId) > 0) {
			strcpy(r->dni_cliente, nuevoId);
		}

	printf("Id habitacion actual: %d\n", r->id_habitacion);
	printf("Ingrese nuevo id habitacion: ");
	fflush(stdout);
		char idHaStr[20];
		fgets(idHaStr, 20, stdin);
		idHaStr[strcspn(idHaStr, "\n")] = '\0'; // Eliminar el salto de línea
		sscanf(idHaStr, "%d", &r->id_habitacion);
	printf("fecha entrada actual: %s\n", r->fecha_entrada);
			printf("Ingrese nueva fecha entrada: ");
			fflush(stdout);
			char fE[50];

			fgets(fE, 50, stdin);
			fE[strcspn(fE, "\n")] = '\0';
			if (strlen(fE) > 0) {
				strcpy(r->fecha_entrada, fE);
			}
   printf("fecha salida actual: %s\n", r->fecha_salida);
					printf("Ingrese nueva fecha salida: ");
					fflush(stdout);
					char fS[50];

					fgets(fS, 50, stdin);
					fS[strcspn(fS, "\n")] = '\0';
					if (strlen(fS) > 0) {
						strcpy(r->fecha_salida, fS);
					}

	printf("Estado actual: %s\n", r->estado);
	printf("Elija el nuevo estado de la reserva.\n");
	printf("1. Confirmada\n");
	printf("2. Pendiente\n");
	printf("Seleccione una opcion: ");
	fflush(stdout);
	int opcion;
	if (scanf("%d", &opcion) == 1) {
		while (getchar() != '\n'); // Limpiar el buffer de entrada
		switch(opcion) {
			case 1:
				strcpy(r->estado, "Confirmada");
				break;
			case 2:
				strcpy(r->estado, "Pendiente");
				break;
			default:
				printf("No se modificará el rol.\n");
				fflush(stdout);
				break;
		}
	} else {
		while (getchar() != '\n'); // Limpiar el buffer de entrada
	}

	printf("Ingrese monto en euros €: ");
	fflush(stdout);
	char montoStr[20];
	fgets(montoStr, 20, stdin);
	montoStr[strcspn(montoStr, "\n")] = '\0'; // Eliminar el salto de línea
	sscanf(montoStr, "%d", &r->monto);

	printf("Ingrese nueva observacion: ");
	fflush(stdout);
	char observacionStr[20];


	fgets(observacionStr, 20, stdin);
	observacionStr[strcspn(observacionStr, "\n")] = '\0';
	if (strlen(observacionStr) > 0) {
		strcpy(r->observaciones, observacionStr);
	}

	modificarReservaBD(r);
}
void buscarReservas(Reserva *r){
	char id_usuario[20];

	printf("\n--- BUSCAR RESERVA ---\n");
	printf("Ingrese el dni del cliente que quiera buscar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(id_usuario, 20, stdin);
	id_usuario[strcspn(id_usuario, "\n")] = '\0';

	buscarReservaBD(id_usuario);
}


