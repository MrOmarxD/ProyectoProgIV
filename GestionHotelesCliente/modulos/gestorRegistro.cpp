#include "gestorRegistro.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;

// Estructura para almacenar un registro de actividad
struct RegistroActividad {
    string fechaHora;
    int idUsuario;
    string tipoActividad;
    string descripcion;
};

// Función auxiliar para leer registros del archivo
vector<RegistroActividad> leerRegistrosDeArchivo(const char* archivo_registro) {
    vector<RegistroActividad> registros;
    ifstream archivo(archivo_registro);

    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo de registros: " << archivo_registro << endl;
        return registros;
    }

    string linea;
    while (getline(archivo, linea)) {
        RegistroActividad reg;
        size_t pos1 = linea.find("]");
        size_t pos2 = linea.find("]", pos1 + 1);
        size_t pos3 = linea.find("]", pos2 + 1);

        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            reg.fechaHora = linea.substr(1, pos1 - 1);

            string userStr = linea.substr(pos1 + 9, pos2 - pos1 - 9);
            reg.idUsuario = stoi(userStr);

            reg.tipoActividad = linea.substr(pos2 + 2, pos3 - pos2 - 2);
            reg.descripcion = linea.substr(pos3 + 4);

            registros.push_back(reg);
        }
    }

    archivo.close();
    return registros;
}

// Función auxiliar para mostrar registros
void mostrarRegistros(const vector<RegistroActividad>& registros) {
    if (registros.empty()) {
        cout << "No se encontraron registros." << endl;
        return;
    }

    cout << "\n===== REGISTROS DE ACTIVIDAD =====\n" << endl;
    cout << left << setw(20) << "FECHA/HORA" << setw(10) << "USUARIO" << setw(15) << "TIPO" << "DESCRIPCIÓN" << endl;
    cout << string(80, '-') << endl;

    for (const auto& reg : registros) {
        cout << left << setw(20) << reg.fechaHora;
        cout << setw(10) << reg.idUsuario;
        cout << setw(15) << reg.tipoActividad;
        cout << reg.descripcion << endl;
    }

    cout << "\nTotal de registros: " << registros.size() << endl;
}

// Comparador de fechas (formato YYYY-MM-DD HH:MM:SS)
bool fechaEnRango(const string& fecha, const string& inicio, const string& fin) {
    // Si las fechas de inicio o fin están vacías, se consideran sin límite
    if (inicio.empty() || fin.empty()) {
        return true;
    }
    return (fecha >= inicio && fecha <= fin);
}

void mostrarMenuRegistros(SOCKET s) {
    char opcion;
    bool salir = false;

    while (!salir) {
        cout << "\n===== GESTOR DE REGISTROS DE ACTIVIDAD =====" << endl;
        cout << "1. Listar todos los registros" << endl;
        cout << "2. Buscar registros por ID de usuario" << endl;
        cout << "3. Buscar registros por rango de fechas" << endl;
        cout << "0. Volver al menú principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer

        switch (opcion) {
            case '1':
                listarRegistros(s);
                break;

            case '2':
                buscarRegistrosPorUsuario(s);
                break;

            case '3':
                obtenerRegistrosPorFecha(s);
                break;

            case '0':
                salir = true;
                break;

            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
                break;
        }
    }
}

void listarRegistros(SOCKET s) {
    cout << "\n=== LISTADO DE REGISTROS DE ACTIVIDAD ===\n\n";

    // Leer todos los registros del archivo
    vector<RegistroActividad> registros = leerRegistrosDeArchivo(g_config.log_file);

    // Mostrar los registros
    mostrarRegistros(registros);

    // Pausa para que el usuario pueda leer los resultados
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

void buscarRegistrosPorUsuario(SOCKET s) {
    int id_usuario;

    cout << "\nIngrese el ID del usuario a buscar: ";
    cin >> id_usuario;
    cin.ignore(); // Limpiar buffer

    // Leer todos los registros del archivo
    vector<RegistroActividad> todosRegistros = leerRegistrosDeArchivo(g_config.log_file);

    // Filtrar por ID de usuario
    vector<RegistroActividad> registrosFiltrados;
    for (const auto& reg : todosRegistros) {
        if (reg.idUsuario == id_usuario) {
            registrosFiltrados.push_back(reg);
        }
    }

    // Mostrar los registros filtrados
    cout << "\nRegistros para el usuario con ID " << id_usuario << ":" << endl;
    mostrarRegistros(registrosFiltrados);

    // Pausa para que el usuario pueda leer los resultados
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

void obtenerRegistrosPorFecha(SOCKET s) {
    string fecha_inicio, fecha_fin;

    cout << "\nBuscar registros por rango de fechas" << endl;
    cout << "Formato de fecha: YYYY-MM-DD" << endl;
    cout << "Fecha de inicio: ";
    getline(cin, fecha_inicio);

    cout << "Fecha de fin: ";
    getline(cin, fecha_fin);

    // Completar las fechas para comparación
    if (!fecha_inicio.empty() && fecha_inicio.length() == 10) {
        fecha_inicio += " 00:00:00"; // Agregar la hora mínima
    }

    if (!fecha_fin.empty() && fecha_fin.length() == 10) {
        fecha_fin += " 23:59:59"; // Agregar la hora máxima
    }

    // Leer todos los registros del archivo
    vector<RegistroActividad> todosRegistros = leerRegistrosDeArchivo(g_config.log_file);

    // Filtrar por rango de fechas
    vector<RegistroActividad> registrosFiltrados;
    for (const auto& reg : todosRegistros) {
        if (fechaEnRango(reg.fechaHora, fecha_inicio, fecha_fin)) {
            registrosFiltrados.push_back(reg);
        }
    }

    // Mostrar los registros filtrados
    cout << "\nRegistros entre " << (fecha_inicio.empty() ? "inicio" : fecha_inicio)
         << " y " << (fecha_fin.empty() ? "fin" : fecha_fin) << ":" << endl;
    mostrarRegistros(registrosFiltrados);

    // Pausa para que el usuario pueda leer los resultados
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

// Implementación de la función registrarActividad utilizada en todo el sistema
void registrarActividad(int usuario_id, const char* tipo_actividad, const char* descripcion, const char* archivo_registro) {
    // Verificar parámetros
    if (tipo_actividad == NULL || descripcion == NULL || archivo_registro == NULL) {
        cerr << "Error: Parámetros inválidos para registrar actividad" << endl;
        return;
    }

    // Obtener fecha y hora actual
    time_t ahora = time(0);
    struct tm *tiempo_local = localtime(&ahora);
    char fecha_hora[50];
    strftime(fecha_hora, sizeof(fecha_hora), "%Y-%m-%d %H:%M:%S", tiempo_local);

    // Abrir archivo en modo append
    ofstream archivo(archivo_registro, ios::app);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de registro: " << archivo_registro << endl;
        return;
    }

    // Escribir el registro en formato: [FECHA_HORA] [Usuario: ID_USUARIO] [TIPO_ACTIVIDAD] - DESCRIPCION
    archivo << "[" << fecha_hora << "] ";
    archivo << "[Usuario: " << usuario_id << "] ";
    archivo << "[" << tipo_actividad << "] - ";
    archivo << descripcion << endl;

    // Cerrar archivo
    archivo.close();
}
