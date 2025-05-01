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
	cout<<"Seleccione una opción: " << endl;
	cin >> opcion;

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
    cout<<"\n--- CONFIGURACIÓN DEL SISTEMA ---\n";
    cout<<"1. Cambiar rutas de archivos\n";
    cout<<"2. Configurar parámetros de conexión\n";
    cout<<"3. Hacer copia de seguridad\n";
    cout<<"4. Restaurar desde copia de seguridad\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: " << endl;
    cin>>opcion;

    switch (opcion) {
		case 1:
			cout<<"Cambiar rutas de archivos\n";
			break;
		case 2:
			cout<<"Configurar parámetros de conexión\n";
			break;
		case 3:
			cout<<"Hacer copia de seguridad\n";
			break;
		case 4:
			cout<<"Restaurar desde copia de seguridad\n";
			break;
		case 0:
			mostrarMenuPrincipal();
			break;
		default:
			cout<<"Opción no válida. Intente nuevamente.\n";
	}

//    registrarActividad(usuario_actual, "Acceso a configuración del sistema", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    cout<<"Funcionalidad en desarrollo...\n";
}

int mostrarMenuPrincipalCliente(SOCKET s){
	int opcion;
	cout<<"\n==========================================================\n";
	cout<<" SISTEMA DE GESTION DE HOTELES - CLIENTE REMOTO\n";
	cout<<"==========================================================\n";
	cout<<"1. Ver listado de clientes\n";
	cout<<"2. Ver listado de habitaciones\n";
	cout<<"3. Crear nueva reserva\n";
	cout<<"4. Salir\n\n";
	cout<<"Seleccione una opcion: " << endl;
	cin>> opcion;

	switch (opcion) {
		case 1:
			mostrarClientes(s);
			break;
		case 2:
			mostrarHabitaciones(s);
			break;
		case 3:
			cout<<"Crear nueva reserva\n";
			break;
		case 4:
			cout<<"Salir\n";
			break;
		default:
			cout<<"Opción no válida. Intente nuevamente.\n";
	}

	return opcion;
}
