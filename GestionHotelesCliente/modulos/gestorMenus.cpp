#include "gestorMenus.h"

/* Variables globales */
using namespace std;
char CONFIG_FILE[100] = "config.dat";  // Ruta al archivo de configuración
ConfigData g_config;  // Configuración global
int usuario_actual = 0; // ID del usuario que ha iniciado sesión

int mostrarMenuPrincipal(SOCKET s) {
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
                	gestionUsuarios(s);
                	registrarActividad(usuario_actual, "Acceso", "Acceso a gestión de usuarios", g_config.log_file);
                    break;
                case 2:
                	gestionClienteAdmin(s);
                	registrarActividad(usuario_actual, "Acceso", "Acceso a gestión de usuarios", g_config.log_file);
                    break;
                case 3:
                    gestionHabitaciones(s);
                    registrarActividad(usuario_actual, "Acceso", "Acceso a gestión de habitaciones", g_config.log_file);
                    break;
                case 4:
                    gestionReservas(s);
                    registrarActividad(usuario_actual, "Acceso", "Acceso a gestión de reservas", g_config.log_file);
                    break;
                case 5:
                    gestionFacturacion(s);
                    registrarActividad(usuario_actual, "Acceso", "Acceso a gestión de facturación", g_config.log_file);
                    break;
                case 6:
                    mostrarMenuRegistros(s);
                    registrarActividad(usuario_actual, "Acceso", "Acceso a registros de actividad", g_config.log_file);
                    break;
                case 7:
                    configuracionSistema(s);
                    registrarActividad(usuario_actual, "Acceso", "Acceso a configuración del sistema", g_config.log_file);
                    break;
                case 0:
                	cout<<"Cerrando sesión y saliendo del sistema...\n";
                    registrarActividad(usuario_actual, "Cierre de sesión", "El usuario ha cerrado la sesión", g_config.log_file);
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

/* Implementación actualizada de las funciones de gestión */
void configuracionSistema(SOCKET s) {
    int opcion;
    cout<<"\n--- CONFIGURACIÓN DEL SISTEMA ---\n";
    cout<<"1. Cambiar rutas de archivos\n";
    cout<<"2. Configurar parámetros de conexión\n";
    cout<<"3. Hacer copia de seguridad\n";
    cout<<"4. Restaurar desde copia de seguridad\n";
    cout<<"5. Ver configuración actual\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: " << endl;
    cin>>opcion;

    switch (opcion) {
		case 1:
			cambiarRutasArchivos(s);
			registrarActividad(usuario_actual, "Configuración", "Cambio de rutas de archivos", g_config.log_file);
			break;
		case 2:
			configurarConexion();
			registrarActividad(usuario_actual, "Configuración", "Cambio de parámetros de conexión", g_config.log_file);
			break;
		case 3:
			hacerCopiaSeguridad();
			registrarActividad(usuario_actual, "Configuración", "Creación de copia de seguridad", g_config.log_file);
			break;
		case 4:
			restaurarCopiaSeguridad();
			registrarActividad(usuario_actual, "Configuración", "Restauración desde copia de seguridad", g_config.log_file);
			break;
		case 5:
			printConfig(&g_config);
			registrarActividad(usuario_actual, "Configuración", "Visualización de configuración actual", g_config.log_file);
			break;
		case 0:
			mostrarMenuPrincipal(s);
			break;
		default:
			cout<<"Opción no válida. Intente nuevamente.\n";
	}
}

void cambiarRutasArchivos(SOCKET s) {
    int opcion;
    char nueva_ruta[100];

    cout<<"\n--- CAMBIAR RUTAS DE ARCHIVOS ---\n";
    cout<<"1. Archivo de usuarios\n";
    cout<<"2. Archivo de clientes\n";
    cout<<"3. Archivo de habitaciones\n";
    cout<<"4. Archivo de reservas\n";
    cout<<"5. Archivo de registro de actividad\n";
    cout<<"6. Directorio de copias de seguridad\n";
    cout<<"0. Volver al menú de configuración\n";
    cout<<"Seleccione una opción: " << endl;
    cin>>opcion;

    // Limpiar buffer
    cin.ignore(1000, '\n');

    switch (opcion) {
        case 1:
            cout << "Ruta actual: " << g_config.db_users_file << endl;
            cout << "Introduce nueva ruta: ";
            cin.getline(nueva_ruta, 100);
            strcpy(g_config.db_users_file, nueva_ruta);
            registrarActividad(usuario_actual, "Configuración", "Cambio de ruta de archivo de usuarios", g_config.log_file);
            break;
        case 2:
            cout << "Ruta actual: " << g_config.db_clients_file << endl;
            cout << "Introduce nueva ruta: ";
            cin.getline(nueva_ruta, 100);
            strcpy(g_config.db_clients_file, nueva_ruta);
            registrarActividad(usuario_actual, "Configuración", "Cambio de ruta de archivo de clientes", g_config.log_file);
            break;
        case 3:
            cout << "Ruta actual: " << g_config.db_rooms_file << endl;
            cout << "Introduce nueva ruta: ";
            cin.getline(nueva_ruta, 100);
            strcpy(g_config.db_rooms_file, nueva_ruta);
            registrarActividad(usuario_actual, "Configuración", "Cambio de ruta de archivo de habitaciones", g_config.log_file);
            break;
        case 4:
            cout << "Ruta actual: " << g_config.db_reservations_file << endl;
            cout << "Introduce nueva ruta: ";
            cin.getline(nueva_ruta, 100);
            strcpy(g_config.db_reservations_file, nueva_ruta);
            registrarActividad(usuario_actual, "Configuración", "Cambio de ruta de archivo de reservas", g_config.log_file);
            break;
        case 5:
            cout << "Ruta actual: " << g_config.log_file << endl;
            cout << "Introduce nueva ruta: ";
            cin.getline(nueva_ruta, 100);
            strcpy(g_config.log_file, nueva_ruta);
            registrarActividad(usuario_actual, "Configuración", "Cambio de ruta de archivo de registro de actividad", g_config.log_file);
            break;
        case 6:
            cout << "Directorio actual: " << g_config.backup_dir << endl;
            cout << "Introduce nuevo directorio: ";
            cin.getline(nueva_ruta, 100);
            strcpy(g_config.backup_dir, nueva_ruta);
            registrarActividad(usuario_actual, "Configuración", "Cambio de directorio de copias de seguridad", g_config.log_file);
            break;
        case 0:
            configuracionSistema(s);
            return;
        default:
            cout << "Opción no válida." << endl;
            return;
    }

    // Guardar cambios
    if (saveConfig(CONFIG_FILE, &g_config)) {
        cout << "Ruta actualizada y guardada correctamente." << endl;
    } else {
        cout << "Error al guardar la configuración." << endl;
    }
}

void configurarConexion() {
    char nuevo_ip[20];
    int nuevo_puerto;

    cout<<"\n--- CONFIGURAR PARÁMETROS DE CONEXIÓN ---\n";
    cout << "IP actual del servidor: " << g_config.server_ip << endl;
    cout << "Introduce nueva IP (deja vacío para mantener): ";

    // Limpiar buffer
    cin.ignore(1000, '\n');

    cin.getline(nuevo_ip, 20);
    if (strlen(nuevo_ip) > 0) {
        strcpy(g_config.server_ip, nuevo_ip);
    }

    cout << "Puerto actual del servidor: " << g_config.server_port << endl;
    cout << "Introduce nuevo puerto (0 para mantener): ";
    cin >> nuevo_puerto;

    if (nuevo_puerto > 0) {
        g_config.server_port = nuevo_puerto;
    }

    // Guardar cambios
    if (saveConfig(CONFIG_FILE, &g_config)) {
        cout << "Parámetros de conexión actualizados y guardados correctamente." << endl;
    } else {
        cout << "Error al guardar la configuración." << endl;
    }
}

void hacerCopiaSeguridad() {
    cout<<"\n--- CREAR COPIA DE SEGURIDAD ---\n";

    if (createBackup(&g_config)) {
        cout << "Copia de seguridad creada correctamente." << endl;
    } else {
        cout << "Error al crear la copia de seguridad." << endl;
    }
}

void restaurarCopiaSeguridad() {
    char fecha_backup[20];

    cout<<"\n--- RESTAURAR DESDE COPIA DE SEGURIDAD ---\n";
    cout << "Introduce la fecha de la copia a restaurar (formato YYYYMMDD_HHMMSS): ";

    // Limpiar buffer
    cin.ignore(1000, '\n');

    cin.getline(fecha_backup, 20);

    if (restoreFromBackup(&g_config, fecha_backup)) {
        cout << "Sistema restaurado correctamente desde la copia de seguridad." << endl;
    } else {
        cout << "Error al restaurar desde la copia de seguridad." << endl;
    }
}

int mostrarMenuPrincipalCliente(SOCKET s){
	int opcion;
	cout<<"\n==========================================================\n";
	cout<<" SISTEMA DE GESTION DE HOTELES - CLIENTE REMOTO\n";
	cout<<"==========================================================\n";
	cout<<"1. Ver listado de clientes\n";
	cout<<"2. Ver listado de habitaciones\n";
	cout<<"3. Crear nueva reserva\n";
	cout<<"0. Salir\n\n";
	cout<<"Seleccione una opcion: " << endl;
	cin>> opcion;

	switch (opcion) {
		case 1:
			mostrarClientes(s);
			registrarActividad(usuario_actual, "Acceso", "Visualización de listado de clientes", g_config.log_file);
			break;
		case 2:
			mostrarHabitaciones(s);
			registrarActividad(usuario_actual, "Acceso", "Visualización de listado de habitaciones", g_config.log_file);
			break;
		case 3:
			crearReserva(s);
			registrarActividad(usuario_actual, "Operación", "Creación de nueva reserva", g_config.log_file);
			break;
		case 0:
			cout<<"Salir\n";
			registrarActividad(usuario_actual, "Cierre de sesión", "El usuario ha cerrado la sesión", g_config.log_file);
			break;
		default:
			cout<<"Opción no válida. Intente nuevamente.\n";
	}

	return opcion;
}
