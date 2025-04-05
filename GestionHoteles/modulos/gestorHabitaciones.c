#include "gestorHabitaciones.h"
#include "gestorRegistros.h"
#include <stdio.h>
#include "gestorMenus.h"

void gestionHabitaciones(int usuario_actual, const char* LOG_FILE) {
	Habitacion h;
    int opcion;
    printf("\n--- GESTIÓN DE HABITACIONES ---\n");
    printf("1. Añadir nueva habitación\n");
    printf("2. Modificar habitación\n");
    printf("3. Cambiar estado de habitación\n");
    printf("4. Listar habitaciones\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    switch (opcion) {
                case 1:
                	crearHabitacion(&h);
                	fflush(stdout);
                    break;
                case 2:
                	printf("Modificar habitación\n");
                	fflush(stdout);
                    break;
                case 3:
                	establecerEstadoHabitacion(&h);
                	fflush(stdout);
                    break;
                case 4:
                	printf("Listar habitaciones\n");
                	fflush(stdout);
                    break;
                case 0:
                	mostrarMenuPrincipal();
                	break;
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de habitaciones", LOG_FILE);
}

void crearHabitacion(Habitacion *habitacion) {
	printf("Ingrese el número de la habitación: \n");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(habitacion->numero, 10, stdin);
	habitacion->numero[strcspn(habitacion->numero, "\n")] = '\0'; // Eliminar el salto de línea

	int tipo;
	    do {
	        printf("Elija el tipo de la Habitacion\n");
	        printf("1. Individual\n");
	        printf("2. Doble\n");
	        printf("3. Suite\n");
	        printf("4. Familiar\n");
	        printf("Seleccione una opcion: ");
	        fflush(stdout);
	        scanf("%d", &tipo);

	        // Limpiar el buffer de entrada
	        while (getchar() != '\n');

	        switch(tipo) {
	            case 1:
	                printf("\nHa seleccionado: Individual\n\n");
	                fflush(stdout);
	                strcpy(habitacion->tipo, "Individual");
	                break;
	            case 2:
	                printf("\nHa seleccionado: Doble\n\n");
	                fflush(stdout);
	                strcpy(habitacion->tipo, "Doble");
	                break;
	            case 3:
	                printf("\nHa seleccionado: Suite\n\n");
	                fflush(stdout);
	                strcpy(habitacion->tipo, "Suite");
	                break;
	            case 4:
	                printf("\nHa seleccionado: Familiar\n\n");
	                fflush(stdout);
	                strcpy(habitacion->tipo, "Familiar");
	                break;
	            default:
	                printf("\nTipo no valida. Por favor, intente de nuevo.\n");
	                fflush(stdout);
	                tipo = 0;
	                break;
	        }
	    } while(tipo == 0);

	    printf("Ingrese precio en euros €: ");
		fflush(stdout);
		char precioStr[20];
		fgets(precioStr, 20, stdin);
		precioStr[strcspn(precioStr, "\n")] = '\0'; // Eliminar el salto de línea
		sscanf(precioStr, "%d", &habitacion->precio);

		int estado;
			do {
				printf("Elija el estado de la Habitacion\n");
				printf("1. Disponible\n");
				printf("2. Ocupada\n");
				printf("3. Mantenimiento\n");
				printf("Seleccione una opcion: ");
				fflush(stdout);
				scanf("%d", &estado);

				// Limpiar el buffer de entrada
				while (getchar() != '\n');

				switch(estado) {
					case 1:
						printf("\nHa seleccionado: Disponible\n\n");
						fflush(stdout);
						strcpy(habitacion->estado, "Disponible");
						break;
					case 2:
						printf("\nHa seleccionado: Ocupada\n\n");
						fflush(stdout);
						strcpy(habitacion->estado, "Ocupada");
						break;
					case 3:
						printf("\nHa seleccionado: Mantenimiento\n\n");
						fflush(stdout);
						strcpy(habitacion->tipo, "Mantenimiento");
						break;
					default:
						printf("\nEstado no valida. Por favor, intente de nuevo.\n");
						fflush(stdout);
						estado = 0;
						break;
				}
			} while(estado == 0);

		printf("Ingrese capacidad: ");
		fflush(stdout);
		char capacidadStr[20];
		fgets(capacidadStr, 20, stdin);
		capacidadStr[strcspn(capacidadStr, "\n")] = '\0'; // Eliminar el salto de línea
		sscanf(capacidadStr, "%d", &habitacion->capacidad);

		printf("Ingrese la descripcion: \n");
		fflush(stdout);

		fgets(habitacion->descripcion, 100, stdin);
		habitacion->descripcion[strcspn(habitacion->descripcion, "\n")] = '\0'; // Eliminar el salto de línea

    crearHabitacionBD(habitacion);
}

void establecerEstadoHabitacion(Habitacion *habitacion){
	int estado;
	do {
		printf("Elija el estado de la Habitacion\n");
		printf("1. Disponible\n");
		printf("2. Ocupada\n");
		printf("3. Mantenimiento\n");
		printf("Seleccione una opcion: ");
		fflush(stdout);
		scanf("%d", &estado);

		// Limpiar el buffer de entrada
		while (getchar() != '\n');

		switch(estado) {
			case 1:
				printf("\nHa seleccionado: Disponible\n\n");
				fflush(stdout);
				strcpy(habitacion->estado, "Disponible");
				break;
			case 2:
				printf("\nHa seleccionado: Ocupada\n\n");
				fflush(stdout);
				strcpy(habitacion->estado, "Ocupada");
				break;
			case 3:
				printf("\nHa seleccionado: Mantenimiento\n\n");
				fflush(stdout);
				strcpy(habitacion->tipo, "Mantenimiento");
				break;
			default:
				printf("\nEstado no valida. Por favor, intente de nuevo.\n");
				fflush(stdout);
				estado = 0;
				break;
		}
	} while(estado == 0);
}
