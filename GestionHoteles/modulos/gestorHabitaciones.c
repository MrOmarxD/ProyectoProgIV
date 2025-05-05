#include "gestorHabitaciones.h"
#include "gestorRegistros.h"
#include <stdio.h>

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

void modificarHabitacion(Habitacion *habitacion){
	char numero[20];

	printf("\n--- MODEFICAR HABITACION ---\n");
		printf("Ingrese el numero de la habitacion a modificar: ");
	    fflush(stdout);

	    while (getchar() != '\n');

	    fgets(numero, 20, stdin);
	    numero[strcspn(numero, "\n")] = '\0';

	    if (!recuperarHabitacionBD(numero, habitacion)) {
	        return;
	    }

	    printf("Habitacion encontrada. Dejar en blanco para no modificar.\n");

	    int tipo;
	    printf("Tipo actual: %s\n", habitacion->tipo);
	    printf("Elija el nuevo tipo de la habitacion\n");
	    printf("1. Individual\n");
		printf("2. Doble\n");
		printf("3. Suite\n");
		printf("4. Familiar\n");
		printf("Seleccione una opcion: ");
		fflush(stdout);
	    if (scanf("%d", &tipo) == 1) {
	        while (getchar() != '\n'); // Limpiar el buffer de entrada
	        switch(tipo) {
	            case 1:
	                strcpy(habitacion->tipo, "Individual");
	                break;
	            case 2:
	                strcpy(habitacion->tipo, "Doble");
	                break;
	            case 3:
	                strcpy(habitacion->tipo, "Suite");
	                break;
	            case 4:
	                strcpy(habitacion->tipo, "Familiar");
	                break;
	            default:
	                printf("No se modificará el tipo.\n");
	                fflush(stdout);
	                break;
	        }
	    } else {
	        while (getchar() != '\n'); // Limpiar el buffer de entrada
	    }

	    printf("Ingrese nuevo precio en euros €: ");
		fflush(stdout);
		char precioStr[20];
		fgets(precioStr, 20, stdin);
		precioStr[strcspn(precioStr, "\n")] = '\0'; // Eliminar el salto de línea
		sscanf(precioStr, "%d", &habitacion->precio);

		int estado;
			    printf("Estado actual: %s\n", habitacion->estado);
			    printf("Elija el nuevo estado de la habitacion.\n");
			    printf("1. Disponible\n");
				printf("2. Ocupada\n");
				printf("3. Mantenimiento\n");
				printf("Seleccione una opcion: ");
				fflush(stdout);
			    if (scanf("%d", &estado) == 1) {
			        while (getchar() != '\n'); // Limpiar el buffer de entrada
			        switch(estado) {
			            case 1:
			                strcpy(habitacion->estado, "Disponible");
			                break;
			            case 2:
			                strcpy(habitacion->estado, "Ocupada");
			                break;
			            case 3:
			                strcpy(habitacion->estado, "Mantenimiento");
			                break;
			            default:
			                printf("No se modificará el estado.\n");
			                fflush(stdout);
			                break;
			        }
			    } else {
			        while (getchar() != '\n'); // Limpiar el buffer de entrada
			    }

		printf("Ingrese la nueva capacidad: ");
		fflush(stdout);
		char capacidadStr[20];
		fgets(capacidadStr, 20, stdin);
		capacidadStr[strcspn(capacidadStr, "\n")] = '\0'; // Eliminar el salto de línea
		sscanf(capacidadStr, "%d", &habitacion->capacidad);

	    printf("Ingrese nueva descripcion: ");
	    fflush(stdout);
	    char nuevaDescripcion[100];


	    fgets(nuevaDescripcion, 20, stdin);
	    nuevaDescripcion[strcspn(nuevaDescripcion, "\n")] = '\0';
	    if (strlen(nuevaDescripcion) > 0) {
	    	strcpy(habitacion->descripcion, nuevaDescripcion);
	    }


	    modificarHabitacionBD(habitacion);
}


void establecerEstadoHabitacion(Habitacion *habitacion){
	char numero[20];

		printf("\n--- MODEFICAR ESTADO DE LA HABITACION ---\n");
			printf("Ingrese el numero de la habitacion a modificar: ");
		    fflush(stdout);

		    while (getchar() != '\n');

		    fgets(numero, 20, stdin);
		    numero[strcspn(numero, "\n")] = '\0';

		    if (!recuperarHabitacionBD(numero, habitacion)) {
		        return;
		    }

		    printf("Habitacion encontrada. Dejar en blanco para no modificar.\n");


		    int estado;
			printf("Estado actual: %s\n", habitacion->estado);
			printf("Elija el nuevo estado de la habitacion.\n");
			printf("1. Disponible\n");
			printf("2. Ocupada\n");
			printf("3. Mantenimiento\n");
			printf("Seleccione una opcion: ");
			fflush(stdout);
			if (scanf("%d", &estado) == 1) {
				while (getchar() != '\n'); // Limpiar el buffer de entrada
				switch(estado) {
					case 1:
						strcpy(habitacion->estado, "Disponible");
						break;
					case 2:
						strcpy(habitacion->estado, "Ocupada");
						break;
					case 3:
						strcpy(habitacion->estado, "Mantenimiento");
						break;
					default:
						printf("No se modificará el estado.\n");
						fflush(stdout);
						break;
				}
			} else {
				while (getchar() != '\n'); // Limpiar el buffer de entrada
			}
			modificarHabitacionBD(habitacion);
}

void buscarHabitacion(Habitacion *habitacion){
	char numero[20];

	printf("\n--- BUSCAR HABITACION ---\n");
	printf("Ingrese el numero de la habitacion que quiera buscar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(numero, 20, stdin);
	numero[strcspn(numero, "\n")] = '\0';

	buscarHabitacionBD(numero);
}
