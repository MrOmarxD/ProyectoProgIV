#include "gestorMenus.h"

/* Variables globales */
using namespace std;
char CONFIG_FILE[100] = "config.dat";
const char* LOG_FILE = "actividad.log";
int usuario_actual = 0; // ID del usuario que ha iniciado sesión

int mostrarMenuPrincipal() {
	int opcion;
	cout<<"\n=============================================================\n";
	cout<<"               MENÚ PRINCIPAL\n";
	cout<<"=============================================================\n";
	cout<<"1. Gestión de Usuarios\n";
	cout<<"2. Gestión de Clientes\n";
	cout<<"3. Gestión de Habitaciones\n";
	cout<<"4. Gestión de Reservas\n";
	cout<<"5. Facturación\n";
	cout<<"6. Ver Registros de Actividad\n";
	cout<<"7. Configuración del Sistema\n";
	cout<<"0. Salir\n";
	cout<<"=============================================================\n";
	cout<<"Seleccione una opción: ";
    scanf("%d", &opcion);

            switch (opcion) {
                case 1:
//                    gestionUsuarios(usuario_actual, LOG_FILE);
                    break;
                case 2:
//                    gestionClientes(usuario_actual, LOG_FILE);
                    break;
                case 3:
//                    gestionHabitaciones(usuario_actual, LOG_FILE);
                    break;
                case 4:
//                    gestionReservas(usuario_actual, LOG_FILE);
                    break;
                case 5:
//                    gestionFacturacion(usuario_actual, LOG_FILE);
                    break;
                case 6:
//                    verRegistrosActividad(usuario_actual, LOG_FILE);
                    break;
                case 7:
                    configuracionSistema();
                    break;
                case 0:
                	cout<<"Cerrando sesión y saliendo del sistema...\n";
//                    registrarActividad(usuario_actual, "Cierre de sesión", LOG_FILE);
                    return 0;
                    break;
            }
            return 1;
}

char eleccionInicial(){
    char opcion;

    cout<<"\n=============================================================\n";
    cout<<"      SISTEMA DE GESTIÓN DE HOTELES - GRUPO 12\n";
    cout<<"=============================================================\n";

	cout<<"1. Iniciar Sesion\n";
	cout<<"2. Registrarse\n";
	cout<<"0. Salir\n";
	cout<<"Elige una opcion: ";
	cin>>opcion;
	cout << "Opción seleccionada: " << opcion << endl;
	return opcion;
}

/* Implementación básica de las funciones de gestión */
void configuracionSistema() {
    int opcion;
    printf("\n--- CONFIGURACIÓN DEL SISTEMA ---\n");
    printf("1. Cambiar rutas de archivos\n");
    printf("2. Configurar parámetros de conexión\n");
    printf("3. Hacer copia de seguridad\n");
    printf("4. Restaurar desde copia de seguridad\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);

    switch (opcion) {
                case 1:
                	printf("Cambiar rutas de archivos\n");
                    break;
                case 2:
                	printf("Configurar parámetros de conexión\n");
                    break;
                case 3:
                	printf("Hacer copia de seguridad\n");
                    break;
                case 4:
                	printf("Restaurar desde copia de seguridad\n");
                    break;
                case 0:
                	mostrarMenuPrincipal();
                	break;
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
            }

//    registrarActividad(usuario_actual, "Acceso a configuración del sistema", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

int mostrarMenuPrincipalCliente(){
	printf("En Proceso...");
	return 0;
}
