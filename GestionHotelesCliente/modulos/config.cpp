#include "config.h"

using namespace std;

// Cargar configuración desde archivo
bool loadConfig(const char* config_file, ConfigData* config) {
    ifstream file(config_file, ios::binary);

    if (!file.is_open()) {
        cout << "Archivo de configuración no encontrado. Creando configuración por defecto." << endl;
        setDefaultConfig(config);
        saveConfig(config_file, config);
        return false;
    }

    file.read((char*)config, sizeof(ConfigData));
    file.close();

    cout << "Configuración cargada correctamente." << endl;
    return true;
}

// Guardar configuración en archivo
bool saveConfig(const char* config_file, ConfigData* config) {
    ofstream file(config_file, ios::binary);

    if (!file.is_open()) {
        cout << "Error al abrir el archivo de configuración para escritura." << endl;
        return false;
    }

    file.write((char*)config, sizeof(ConfigData));
    file.close();

    cout << "Configuración guardada correctamente en " << config_file << endl;
    return true;
}

// Establecer configuración por defecto
void setDefaultConfig(ConfigData* config) {
    strcpy(config->server_ip, "127.0.0.1");
    config->server_port = 6000;
    strcpy(config->db_users_file, "data/usuarios.dat");
    strcpy(config->db_clients_file, "data/clientes.dat");
    strcpy(config->db_rooms_file, "data/habitaciones.dat");
    strcpy(config->db_reservations_file, "data/reservas.dat");
    strcpy(config->log_file, "actividad.log");
    config->backup_interval = 7;  // Cada 7 días
    strcpy(config->backup_dir, "backups/");
}

// Mostrar configuración actual
void printConfig(const ConfigData* config) {
    cout << "\n=== CONFIGURACIÓN ACTUAL DEL SISTEMA ===" << endl;
    cout << "IP del servidor: " << config->server_ip << endl;
    cout << "Puerto del servidor: " << config->server_port << endl;
    cout << "Archivo de usuarios: " << config->db_users_file << endl;
    cout << "Archivo de clientes: " << config->db_clients_file << endl;
    cout << "Archivo de habitaciones: " << config->db_rooms_file << endl;
    cout << "Archivo de reservas: " << config->db_reservations_file << endl;
    cout << "Archivo de registro: " << config->log_file << endl;
    cout << "Intervalo de copias de seguridad: " << config->backup_interval << " días" << endl;
    cout << "Directorio de copias de seguridad: " << config->backup_dir << endl;
    cout << "=======================================" << endl;
}

// Actualizar un valor específico de la configuración
bool updateConfigValue(const char* config_file, const char* key, const char* value) {
    ConfigData config;

    if (!loadConfig(config_file, &config)) {
        return false;
    }

    bool updated = false;

    if (strcmp(key, "server_ip") == 0) {
        strcpy(config.server_ip, value);
        updated = true;
    } else if (strcmp(key, "server_port") == 0) {
        config.server_port = atoi(value);
        updated = true;
    } else if (strcmp(key, "db_users_file") == 0) {
        strcpy(config.db_users_file, value);
        updated = true;
    } else if (strcmp(key, "db_clients_file") == 0) {
        strcpy(config.db_clients_file, value);
        updated = true;
    } else if (strcmp(key, "db_rooms_file") == 0) {
        strcpy(config.db_rooms_file, value);
        updated = true;
    } else if (strcmp(key, "db_reservations_file") == 0) {
        strcpy(config.db_reservations_file, value);
        updated = true;
    } else if (strcmp(key, "log_file") == 0) {
        strcpy(config.log_file, value);
        updated = true;
    } else if (strcmp(key, "backup_interval") == 0) {
        config.backup_interval = atoi(value);
        updated = true;
    } else if (strcmp(key, "backup_dir") == 0) {
        strcpy(config.backup_dir, value);
        updated = true;
    } else {
        cout << "Clave de configuración no reconocida: " << key << endl;
        return false;
    }

    if (updated) {
        return saveConfig(config_file, &config);
    }

    return false;
}

// Crear una copia de seguridad
bool createBackup(const ConfigData* config) {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", ltm);

    // Crear directorio si no existe
    if (_mkdir(config->backup_dir) != 0 && errno != EEXIST) {
        cout << "Error al crear directorio de copias de seguridad" << endl;
        return false;
    }

    char backup_log[200];
    sprintf(backup_log, "%s/%s_actividad.bak", config->backup_dir, timestamp);

    ifstream src_log(config->log_file, ios::binary);
    ofstream dst_log(backup_log, ios::binary);
    if (src_log && dst_log) {
        dst_log << src_log.rdbuf();
    } else {
        cout << "Error al hacer copia de seguridad del registro de actividad" << endl;
        // Decidir si esto debe ser fatal o no
    }

    // Crear nombre para archivos de backup
    char backup_users[200], backup_clients[200], backup_rooms[200], backup_reservations[200];
    sprintf(backup_users, "%s/%s_usuarios.bak", config->backup_dir, timestamp);
    sprintf(backup_clients, "%s/%s_clientes.bak", config->backup_dir, timestamp);
    sprintf(backup_rooms, "%s/%s_habitaciones.bak", config->backup_dir, timestamp);
    sprintf(backup_reservations, "%s/%s_reservas.bak", config->backup_dir, timestamp);

    // Copiar archivos
    ifstream src_users(config->db_users_file, ios::binary);
    ofstream dst_users(backup_users, ios::binary);
    if (src_users && dst_users) {
        dst_users << src_users.rdbuf();
    } else {
        cout << "Error al hacer copia de seguridad de usuarios" << endl;
        return false;
    }

    ifstream src_clients(config->db_clients_file, ios::binary);
    ofstream dst_clients(backup_clients, ios::binary);
    if (src_clients && dst_clients) {
        dst_clients << src_clients.rdbuf();
    } else {
        cout << "Error al hacer copia de seguridad de clientes" << endl;
        return false;
    }

    ifstream src_rooms(config->db_rooms_file, ios::binary);
    ofstream dst_rooms(backup_rooms, ios::binary);
    if (src_rooms && dst_rooms) {
        dst_rooms << src_rooms.rdbuf();
    } else {
        cout << "Error al hacer copia de seguridad de habitaciones" << endl;
        return false;
    }

    ifstream src_reservations(config->db_reservations_file, ios::binary);
    ofstream dst_reservations(backup_reservations, ios::binary);
    if (src_reservations && dst_reservations) {
        dst_reservations << src_reservations.rdbuf();
    } else {
        cout << "Error al hacer copia de seguridad de reservas" << endl;
        return false;
    }

    cout << "Copia de seguridad creada correctamente: " << timestamp << endl;
    return true;
}

// Restaurar desde copia de seguridad
bool restoreFromBackup(const ConfigData* config, const char* backup_date) {
    char backup_users[200], backup_clients[200], backup_rooms[200], backup_reservations[200];
    sprintf(backup_users, "%s/%s_usuarios.bak", config->backup_dir, backup_date);
    sprintf(backup_clients, "%s/%s_clientes.bak", config->backup_dir, backup_date);
    sprintf(backup_rooms, "%s/%s_habitaciones.bak", config->backup_dir, backup_date);
    sprintf(backup_reservations, "%s/%s_reservas.bak", config->backup_dir, backup_date);

    // Verificar que los archivos existen
    ifstream test_users(backup_users);
    ifstream test_clients(backup_clients);
    ifstream test_rooms(backup_rooms);
    ifstream test_reservations(backup_reservations);

    if (!test_users || !test_clients || !test_rooms || !test_reservations) {
        cout << "Archivos de copia de seguridad no encontrados para la fecha: " << backup_date << endl;
        return false;
    }

    test_users.close();
    test_clients.close();
    test_rooms.close();
    test_reservations.close();

    // Restaurar archivos
    ifstream src_users(backup_users, ios::binary);
    ofstream dst_users(config->db_users_file, ios::binary);
    if (src_users && dst_users) {
        dst_users << src_users.rdbuf();
    } else {
        cout << "Error al restaurar usuarios" << endl;
        return false;
    }

    ifstream src_clients(backup_clients, ios::binary);
    ofstream dst_clients(config->db_clients_file, ios::binary);
    if (src_clients && dst_clients) {
        dst_clients << src_clients.rdbuf();
    } else {
        cout << "Error al restaurar clientes" << endl;
        return false;
    }

    ifstream src_rooms(backup_rooms, ios::binary);
    ofstream dst_rooms(config->db_rooms_file, ios::binary);
    if (src_rooms && dst_rooms) {
        dst_rooms << src_rooms.rdbuf();
    } else {
        cout << "Error al restaurar habitaciones" << endl;
        return false;
    }

    ifstream src_reservations(backup_reservations, ios::binary);
    ofstream dst_reservations(config->db_reservations_file, ios::binary);
    if (src_reservations && dst_reservations) {
        dst_reservations << src_reservations.rdbuf();
    } else {
        cout << "Error al restaurar reservas" << endl;
        return false;
    }

    cout << "Restauración desde copia de seguridad completada: " << backup_date << endl;
    return true;
}
