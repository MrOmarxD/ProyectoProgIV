#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <direct.h>

// Estructura para almacenar configuración
typedef struct {
    char server_ip[20];          // IP del servidor
    int server_port;             // Puerto del servidor
    char db_users_file[100];     // Ruta al archivo de usuarios
    char db_clients_file[100];   // Ruta al archivo de clientes
    char db_rooms_file[100];     // Ruta al archivo de habitaciones
    char db_reservations_file[100]; // Ruta al archivo de reservas
    char log_file[100];          // Ruta al archivo de registro de actividades
    int backup_interval;         // Intervalo de copia de seguridad (en días)
    char backup_dir[100];        // Directorio para copias de seguridad
} ConfigData;

// Funciones para gestionar configuración
bool loadConfig(const char* config_file, ConfigData* config);
bool saveConfig(const char* config_file, ConfigData* config);
void setDefaultConfig(ConfigData* config);
void printConfig(const ConfigData* config);
bool updateConfigValue(const char* config_file, const char* key, const char* value);
bool createBackup(const ConfigData* config);
bool restoreFromBackup(const ConfigData* config, const char* backup_file);

#endif /* CONFIG_H */
