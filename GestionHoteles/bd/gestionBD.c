#include "gestionBD.h"

sqlite3 *db;
sqlite3_stmt *stmt;
int result;

void abrirBd(){

	result = sqlite3_open("hotelesBD.sqlite", &db);
    if (result != SQLITE_OK) {
        printf("Error al abrir la BBDD\n");
        fflush(stdout);
        return;
    }
    printf("BBDD abierta\n");
    fflush(stdout);
}

void cerrarBd(){
	result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error al cerrar la BBDD\n");
        printf("%s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return;
    }

    printf("BBDD cerrada\n");
    fflush(stdout);
}

// FUNCIONALIDAD PARA USUARIO ---------------------------------------------------------------------------------------------------------------------------------------

char* listaUsuarios() {
    static char resultBuffer[4096]; // Buffer estático para almacenar resultados
    resultBuffer[0] = '\0'; // Inicializar el buffer vacío
    sqlite3_stmt *stmt;
    int result;

    const char *sql = "SELECT nombre_usuario, nombre, rol, turno FROM usuarios";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sprintf(resultBuffer, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return resultBuffer;
    }

    strcat(resultBuffer, "Lista de usuarios:\n");
    strcat(resultBuffer, "Usuario Nombre Rol Turno\n");
    strcat(resultBuffer, "----------------------------------------\n");

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        char temp[512];
        sprintf(temp, "%s, %s, %s, %s\n",
            (const char*)sqlite3_column_text(stmt, 0),  // usuario
            (const char*)sqlite3_column_text(stmt, 1),  // nombre
            (const char*)sqlite3_column_text(stmt, 2),  // rol
            (const char*)sqlite3_column_text(stmt, 3)); // turno

        strcat(resultBuffer, temp);
    }

    if (result != SQLITE_DONE) {
        char temp[256];
        sprintf(temp, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        strcat(resultBuffer, temp);
    }

    sqlite3_finalize(stmt);
    return resultBuffer;
}

int eliminarUsuarioBD(char* nombreUsuario) {
    sqlite3_stmt *stmt;
    int result;

    // Query SQL para eliminar el usuario
    char sql[100] = "DELETE FROM usuarios WHERE nombre_usuario = ?";

    // Preparar la declaración SQL
    if (sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // Vincular el parámetro (nombre de usuario)
    sqlite3_bind_text(stmt, 1, nombreUsuario, strlen(nombreUsuario), SQLITE_STATIC);

    // Ejecutar la consulta
    result = sqlite3_step(stmt);

    // Finalizar la declaración
    sqlite3_finalize(stmt);

    // Verificar el resultado
    if (result != SQLITE_DONE) {
        printf("Error al eliminar el usuario: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // Si llegamos aquí, el usuario se eliminó correctamente
    return 1;
}

int comprobarUsuario(const char *usuario){
	char sql2[] = "select count(*) from usuarios where nombre_usuario = ?";

		sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;
		sqlite3_bind_text(stmt, 1, usuario, strlen(usuario), SQLITE_STATIC);

		result = sqlite3_step(stmt);
		    int count = 0;
		    if (result == SQLITE_ROW) {
		        count = sqlite3_column_int(stmt, 0);
		    }

		    sqlite3_finalize(stmt);
		    return count > 0;
}



int crearUsuarioBD(Usuario *user) {
    char sql1[] = "INSERT INTO usuarios (id, nombre, rol, nombre_usuario, contraseña, turno, salario) VALUES (NULL, ?, ?, ?, ?, ?, ?)";

    // Verificar primero si el usuario ya existe
    if (comprobarUsuario(user->usuario)) {
        printf("Error: El usuario %s ya existe en la base de datos\n", user->usuario);
        fflush(stdout);
        return 0;
    }

    if (sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, user->nombre, strlen(user->nombre), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user->rol, strlen(user->rol), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user->usuario, strlen(user->usuario), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user->password, strlen(user->password), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, user->turno, strlen(user->turno), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, user->salario);

    // Iniciar una transacción
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando el usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL); // Hacer rollback en caso de error
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    } else {
        printf("Usuario %s insertado con éxito\n", user->nombre);
        // Ejecutar COMMIT para asegurar que los cambios se guarden
        sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 1;
    }
}

int modificarUsuarioBD(Usuario *user) {
    char sql[] = "UPDATE usuarios SET nombre = ?, rol = ?, contraseña = ?, turno = ?, salario = ? WHERE nombre_usuario = ?";
    sqlite3_stmt *stmt;
    int result;

    char nombre[50];
    char rol[20];
    char usuario[20];
    char password[20];
    char turno[20];
    int salario = user->salario;

    strcpy(nombre, user->nombre);
    strcpy(rol, user->rol);
    strcpy(usuario, user->usuario);
    strcpy(password, user->password);
    strcpy(turno, user->turno);

    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, rol, strlen(rol), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password, strlen(password), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, turno, strlen(turno), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, salario);
    sqlite3_bind_text(stmt, 6, usuario, strlen(usuario), SQLITE_STATIC); // Nombre de usuario es la clave primaria

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error al modificar el usuario: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    } else {
        printf("Usuario '%s' modificado correctamente\n", user->usuario);
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 1;
    }
}

int recuperarUsuarioBD(const char *nombreUsuario, Usuario *user) {
    char sql[] = "SELECT nombre, rol, nombre_usuario, contraseña, turno, salario FROM usuarios WHERE nombre_usuario = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, nombreUsuario, strlen(nombreUsuario), SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        strcpy(user->nombre, (const char *)sqlite3_column_text(stmt, 0));
        strcpy(user->rol, (const char *)sqlite3_column_text(stmt, 1));
        strcpy(user->usuario, (const char *)sqlite3_column_text(stmt, 2));
        strcpy(user->password, (const char *)sqlite3_column_text(stmt, 3));
        strcpy(user->turno, (const char *)sqlite3_column_text(stmt, 4));
        user->salario = sqlite3_column_int(stmt, 5);

        sqlite3_finalize(stmt);
        return 1;
    } else {
        printf("Usuario no encontrado.\n");
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    }
}

char* buscarUsuarioBD(const char *nombreUsuario) {
    static char resultBuffer[1024]; // Buffer estático para almacenar resultados
    resultBuffer[0] = '\0'; // Inicializar el buffer vacío
    sqlite3_stmt *stmt;
    int result;

    const char *sql = "SELECT nombre, rol, nombre_usuario, contraseña, turno, salario FROM usuarios WHERE nombre_usuario LIKE ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sprintf(resultBuffer, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return resultBuffer;
    }


    char searchPattern[100];
    sprintf(searchPattern, "%s", nombreUsuario); // nombreUsuario

    if (sqlite3_bind_text(stmt, 1, searchPattern, strlen(searchPattern), SQLITE_STATIC) != SQLITE_OK) {
        sprintf(resultBuffer, "Error al enlazar parámetros: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return resultBuffer;
    }

    strcat(resultBuffer, "Resultados de la búsqueda:\n");
    strcat(resultBuffer, "Nombre Rol Usuario Contraseña Turno Salario\n");
    strcat(resultBuffer, "-----------------------------------------------------------\n");

    bool foundResult = false;

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        foundResult = true;
        char temp[512];
        sprintf(temp, "%s, %s, %s, %s, %s, %d\n",
            (const char*)sqlite3_column_text(stmt, 0),  // nombre
            (const char*)sqlite3_column_text(stmt, 1),  // rol
            (const char*)sqlite3_column_text(stmt, 2),  // usuario
            (const char*)sqlite3_column_text(stmt, 3),  // password
            (const char*)sqlite3_column_text(stmt, 4),  // turno
            sqlite3_column_int(stmt, 5));               // salario

        strcat(resultBuffer, temp);
    }

    if (!foundResult) {
        strcat(resultBuffer, "No se encontraron usuarios que coincidan con el criterio de búsqueda.\n");
    }

    if (result != SQLITE_DONE && result != SQLITE_ROW) {
        char temp[256];
        sprintf(temp, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        strcat(resultBuffer, temp);
    }

    sqlite3_finalize(stmt);
    return resultBuffer;
}

// FUNCIONALIDAD PARA CLIENTE ----------------------------------------------------------------------------------------------------------------------------------------
int crearClienteBD(Cliente *cliente) {
    char sql[] = "INSERT INTO clientes (dni, nombre, apellido, telefono, email) VALUES (?, ?, ?, ?, ?)";

    // Verificar primero si el cliente ya existe
    if (comprobarCliente(cliente->dni)) {
        printf("Error: El cliente con DNI %s ya existe en la base de datos\n", cliente->dni);
        fflush(stdout);
        return 0;
    }

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, cliente->dni, strlen(cliente->dni), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cliente->nombre, strlen(cliente->nombre), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cliente->apellido, strlen(cliente->apellido), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, cliente->telefono, strlen(cliente->telefono), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, cliente->email, strlen(cliente->email), SQLITE_STATIC);

    // Iniciar una transacción
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando el cliente: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL); // Hacer rollback en caso de error
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    } else {
        printf("Cliente %s %s insertado con éxito\n", cliente->nombre, cliente->apellido);
        // Ejecutar COMMIT para asegurar que los cambios se guarden
        sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 1;
    }
}

int comprobarCliente(const char *cliente){
	char sql2[] = "select count(*) from clientes where email = ?";

		sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;
		sqlite3_bind_text(stmt, 1, cliente, strlen(cliente), SQLITE_STATIC);

		result = sqlite3_step(stmt);
		int count = 0;
		if (result == SQLITE_ROW) {
			count = sqlite3_column_int(stmt, 0);
		}

		sqlite3_finalize(stmt);
		return count > 0;
}

int modificarClienteBD(Cliente *cliente) {
    char sql[] = "UPDATE clientes SET nombre = ?, apellido = ?, telefono = ?, email = ? WHERE dni = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, cliente->nombre, strlen(cliente->nombre), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cliente->apellido, strlen(cliente->apellido), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cliente->telefono, strlen(cliente->telefono), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, cliente->email, strlen(cliente->email), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, cliente->dni, strlen(cliente->dni), SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error al actualizar el cliente: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    } else {
        int changes = sqlite3_changes(db);
        if (changes == 0) {
            printf("No se encontró cliente con DNI '%s' para actualizar o no hubo cambios\n", cliente->dni);
            fflush(stdout);
            sqlite3_finalize(stmt);
            return 0;
        } else {
            printf("Cliente con DNI '%s' actualizado correctamente\n", cliente->dni);
            fflush(stdout);
            sqlite3_finalize(stmt);
            return 1;
        }
    }
}

int recuperarClienteBD(const char *dniCliente, Cliente *client) {
    char sql[] = "SELECT dni, nombre, apellido, telefono, email FROM clientes WHERE dni = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, dniCliente, strlen(dniCliente), SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        strcpy(client->dni, (const char *)sqlite3_column_text(stmt, 0));
        strcpy(client->nombre, (const char *)sqlite3_column_text(stmt, 1));
        strcpy(client->apellido, (const char *)sqlite3_column_text(stmt, 2));
        strcpy(client->telefono, (const char *)sqlite3_column_text(stmt, 3));
        strcpy(client->email, (const char *)sqlite3_column_text(stmt, 4));
        sqlite3_finalize(stmt);
        return 1;
    } else {
        printf("Cliente no encontrado.\n");
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    }
}

char* listarClientes(){
	static char resultBuffer[4096]; // Buffer estático para almacenar resultados
	    resultBuffer[0] = '\0'; // Inicializar el buffer vacío

	    char sql2[] = "select dni, nombre from clientes";
	    sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL);

	    strcat(resultBuffer, "Lista de clientes:\n");

	    do {
	        result = sqlite3_step(stmt);
	        if (result == SQLITE_ROW) {
	            char temp[256];
	            sprintf(temp, "%s, %s\n", (char*)sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1));
	            strcat(resultBuffer, temp);
	        }
	    } while (result == SQLITE_ROW);

	    sqlite3_finalize(stmt);
	    return resultBuffer;
}


char* buscarClientesBD(const char *dniCliente) {
    static char resultadoBusqueda[512]; // Buffer estático para devolver resultado
    Cliente cliente;
    memset(&cliente, 0, sizeof(Cliente)); // Inicializar el objeto
    memset(resultadoBusqueda, 0, sizeof(resultadoBusqueda)); // Inicializar buffer resultado

    char sql[] = "SELECT dni, nombre, apellido, telefono, email, fecha_registro FROM clientes WHERE dni = ?";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    if (rc != SQLITE_OK) {
        sprintf(resultadoBusqueda, "ERROR|Error en la consulta: %s", sqlite3_errmsg(db));
        return resultadoBusqueda;
    }

    sqlite3_bind_text(stmt, 1, dniCliente, strlen(dniCliente), SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        // Asignar los valores obtenidos de la base de datos al objeto Cliente
        strcpy(cliente.dni, (const char *)sqlite3_column_text(stmt, 0));
        strcpy(cliente.nombre, (const char *)sqlite3_column_text(stmt, 1));
        strcpy(cliente.apellido, (const char *)sqlite3_column_text(stmt, 2));
        strcpy(cliente.telefono, (const char *)sqlite3_column_text(stmt, 3));
        strcpy(cliente.email, (const char *)sqlite3_column_text(stmt, 4));

        // Formatear el resultado
        snprintf(resultadoBusqueda, sizeof(resultadoBusqueda), "CLIENTE|%s|%s|%s|%s|%s",
                cliente.dni, cliente.nombre, cliente.apellido,
                cliente.telefono, cliente.email);
    } else {
        strcpy(resultadoBusqueda, "ERROR|Cliente no encontrado");
    }

    sqlite3_finalize(stmt);
    return resultadoBusqueda;
}




// FUNCIONALIDAD PARA HABITACIONES ----------------------------------------------------------------------------------------------------------------------------------------

// Función para comprobar si una habitación existe en la base de datos
int comprobarHabitacion(const char *numeroHabitacion) {
    char sql[] = "SELECT count(*) FROM habitaciones WHERE numero = ?";
    sqlite3_stmt *stmt;
    int result;

    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_text(stmt, 1, numeroHabitacion, strlen(numeroHabitacion), SQLITE_STATIC);

    result = sqlite3_step(stmt);
    int count = 0;
    if (result == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}

// Función para crear una habitación en la base de datos
void crearHabitacionBD(Habitacion *habitacion) {
    char sql[] = "INSERT INTO habitaciones (numero, tipo, precio, estado, capacidad, descripcion) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    int result;

    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, habitacion->numero, strlen(habitacion->numero), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, habitacion->tipo, strlen(habitacion->tipo), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, habitacion->precio);
    sqlite3_bind_text(stmt, 4, habitacion->estado, strlen(habitacion->estado), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, habitacion->capacidad);
    sqlite3_bind_text(stmt, 6, habitacion->descripcion, strlen(habitacion->descripcion), SQLITE_STATIC);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando la habitacion: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
    } else {
        printf("Habitacion %s insertada\n", habitacion->numero);
        fflush(stdout);
    }

    sqlite3_finalize(stmt);
}

// Función para modificar una habitación en la base de datos
void modificarHabitacionBD(Habitacion *habitacion) {
    char sql[] = "UPDATE habitaciones SET tipo = ?, precio = ?, estado = ?, capacidad = ?, descripcion = ? WHERE numero = ?";
    sqlite3_stmt *stmt;
    int result;

    char numero[10];
    char tipo[20];
    int precio = habitacion->precio;
    char estado[15]; // Disponible, Ocupada, Mantenimiento
    int capacidad = habitacion->capacidad;
    char descripcion[100];

    strcpy(numero, habitacion->numero);
    strcpy(tipo, habitacion->tipo);
    strcpy(estado, habitacion->estado);
    strcpy(descripcion, habitacion->descripcion);

    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, tipo, strlen(tipo), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, precio);
    sqlite3_bind_text(stmt, 3, estado, strlen(estado), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, capacidad);
    sqlite3_bind_text(stmt, 5, descripcion, strlen(descripcion), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, numero, strlen(numero), SQLITE_STATIC);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error al modificar la habitacion: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
    } else {
        printf("Habitacion '%s' modificada correctamente\n", habitacion->numero);
        fflush(stdout);
    }

    sqlite3_finalize(stmt);
}

// Función para recuperar una habitación de la base de datos
int recuperarHabitacionBD(const char *numHabitacion, Habitacion *habitacion) {
    char sql[] = "SELECT numero, tipo, precio, estado, capacidad, descripcion FROM habitaciones WHERE numero = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, numHabitacion, strlen(numHabitacion), SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        strcpy(habitacion->numero, (const char *)sqlite3_column_text(stmt, 0));
        strcpy(habitacion->tipo, (const char *)sqlite3_column_text(stmt, 1));
        habitacion->precio = sqlite3_column_int(stmt, 2);
        strcpy(habitacion->estado, (const char *)sqlite3_column_text(stmt, 3));
        habitacion->capacidad = sqlite3_column_int(stmt, 4);
        strcpy(habitacion->descripcion, (const char *)sqlite3_column_text(stmt, 5));
        sqlite3_finalize(stmt);
        return 1;
    } else {
        printf("Habitación no encontrada.\n");
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    }
}

// Función para listar todas las habitaciones
char* listarHabitaciones() {
    static char resultBuffer[4096]; // Buffer estático para almacenar resultados
    resultBuffer[0] = '\0'; // Inicializar el buffer vacío
    sqlite3_stmt *stmt;
    int result;

    char sql[] = "SELECT numero, tipo, precio, estado, capacidad FROM habitaciones";
    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

    strcat(resultBuffer, "Lista de Habitaciones:\n");
    strcat(resultBuffer, "Número | Tipo | Precio | Estado | Capacidad\n");
    strcat(resultBuffer, "----------------------------------------\n");

    do {
        result = sqlite3_step(stmt);
        if (result == SQLITE_ROW) {
            char temp[256];
            sprintf(temp, "%s | %s | %d € | %s | %d personas\n",
                (char*)sqlite3_column_text(stmt, 0),    // número
                (char*)sqlite3_column_text(stmt, 1),    // tipo
                sqlite3_column_int(stmt, 2),            // precio
                (char*)sqlite3_column_text(stmt, 3),    // estado
                sqlite3_column_int(stmt, 4));           // capacidad

            strcat(resultBuffer, temp);
        }
    } while (result == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return resultBuffer;
}

// Función para buscar una habitación por número
char* buscarHabitacionPorNumeroBD(const char *numHabitacion) {
    static char resultBuffer[1024]; // Buffer estático para almacenar resultados
    resultBuffer[0] = '\0'; // Inicializar el buffer vacío
    sqlite3_stmt *stmt;
    int result;

    const char *sql = "SELECT numero, tipo, precio, estado, capacidad, descripcion FROM habitaciones WHERE numero LIKE ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sprintf(resultBuffer, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return resultBuffer;
    }

    // Agregar comodines para búsqueda parcial
    char searchPattern[100];
    sprintf(searchPattern, "%s", numHabitacion); // %numHabitacion%

    if (sqlite3_bind_text(stmt, 1, searchPattern, strlen(searchPattern), SQLITE_STATIC) != SQLITE_OK) {
        sprintf(resultBuffer, "Error al enlazar parámetros: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return resultBuffer;
    }

    strcat(resultBuffer, "Resultados de la búsqueda:\n");
    strcat(resultBuffer, "Número | Tipo | Precio | Estado | Capacidad | Descripción\n");
    strcat(resultBuffer, "-----------------------------------------------------------\n");

    bool foundResult = false;

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        foundResult = true;
        char temp[512];
        sprintf(temp, "%s | %s | %d € | %s | %d | %s\n",
            (const char*)sqlite3_column_text(stmt, 0),  // número
            (const char*)sqlite3_column_text(stmt, 1),  // tipo
            sqlite3_column_int(stmt, 2),                // precio
            (const char*)sqlite3_column_text(stmt, 3),  // estado
            sqlite3_column_int(stmt, 4),                // capacidad
            (const char*)sqlite3_column_text(stmt, 5)); // descripción

        strcat(resultBuffer, temp);
    }

    if (!foundResult) {
        strcat(resultBuffer, "No se encontraron habitaciones que coincidan con el criterio de búsqueda.\n");
    }

    if (result != SQLITE_DONE && result != SQLITE_ROW) {
        char temp[256];
        sprintf(temp, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        strcat(resultBuffer, temp);
    }

    sqlite3_finalize(stmt);
    return resultBuffer;
}


// FUNCIONALIDAD PARA FACTURAS ------------------------------------------------------------------------------

void crearFacturaBD(Factura* factura) {
    char sql[] = "INSERT INTO facturacion (id_reserva, id_cliente, monto, metodo_pago, fecha, numero_factura, estado) VALUES (?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    
    sqlite3_bind_text(stmt, 1, factura->id_reserva, strlen(factura->id_reserva), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, factura->dni_cliente, strlen(factura->dni_cliente), SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, factura->monto);
    sqlite3_bind_text(stmt, 4, factura->metodo_pago, strlen(factura->metodo_pago), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, factura->fecha, strlen(factura->fecha), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, factura->numero_Factura, strlen(factura->numero_Factura), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, factura->estado, strlen(factura->estado), SQLITE_STATIC);
    
    int result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando la factura: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
    } else {
        printf("Factura #%s registrada en la base de datos.\n", factura->numero_Factura);
        fflush(stdout);
    }
    
    sqlite3_finalize(stmt);
}

int buscarFacturaBD(const char *numero_factura, Factura *factura) {
    char sql[] = "SELECT id, id_reserva, id_cliente, monto, metodo_pago, fecha, numero_factura, estado FROM facturacion WHERE numero_factura = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, numero_factura, strlen(numero_factura), SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        factura->id = sqlite3_column_int(stmt, 0);
        strcpy(factura->id_reserva, (const char *)sqlite3_column_text(stmt, 2));
        strcpy(factura->dni_cliente, (const char *)sqlite3_column_text(stmt, 2));
        factura->monto = sqlite3_column_double(stmt, 3);
        strcpy(factura->metodo_pago, (const char *)sqlite3_column_text(stmt, 4));
        strcpy(factura->fecha, (const char *)sqlite3_column_text(stmt, 5));
        strcpy(factura->numero_Factura, (const char *)sqlite3_column_text(stmt, 6));
        strcpy(factura->estado, (const char *)sqlite3_column_text(stmt, 7));

        sqlite3_finalize(stmt);
        return 1;
    } else {
        sqlite3_finalize(stmt);
        return 0;
    }
}


// FUNCIONALIDAD PARA RESERVAS ---------------------------------------------------------------------------------------------------------------------------------------
int crearReservaBD(Reserva *reserva){
	char sql1[] = "INSERT INTO reservas (id, id_cliente, id_habitacion, fecha_entrada, fecha_salida, estado, monto, observaciones) VALUES (NULL, ?, ?, ?, ?, ?, ?, ?)";

	if (sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) != SQLITE_OK) {
		   printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
		   fflush(stdout);
		   return 0;
	}

    sqlite3_bind_text(stmt, 1, reserva->dni_cliente, strlen(reserva->dni_cliente), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, reserva->id_habitacion);
    sqlite3_bind_text(stmt, 3, reserva->fecha_entrada, strlen(reserva->fecha_entrada), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, reserva->fecha_salida, strlen(reserva->fecha_salida), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, reserva->estado, strlen(reserva->estado), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, reserva->monto);
    sqlite3_bind_text(stmt, 7, reserva->observaciones, strlen(reserva->observaciones), SQLITE_STATIC);

    // Iniciar una transacción
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando la reserva: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL); // Hacer rollback en caso de error
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    } else {
        printf("Resereva del cliente: %s insertada con éxito\n", reserva->dni_cliente);
        // Ejecutar COMMIT para asegurar que los cambios se guarden
        sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 1;
    }
}
int modificarReservaBD(Reserva *r) {
    char sql[] = "UPDATE reservas SET id_cliente = ?, id_habitacion = ?, fecha_entrada = ?, fecha_salida = ?, estado = ?, monto = ?, observaciones = ? WHERE id = ?";
    sqlite3_stmt *stmt;
    int result;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    // Vinculamos los parámetros a la consulta SQL
    sqlite3_bind_text(stmt, 1, r->dni_cliente, strlen(r->dni_cliente), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, r->id_habitacion);
    sqlite3_bind_text(stmt, 3, r->fecha_entrada, strlen(r->fecha_entrada), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, r->fecha_salida, strlen(r->fecha_salida), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, r->estado, strlen(r->estado), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, r->monto);
    sqlite3_bind_text(stmt, 7, r->observaciones, strlen(r->observaciones), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, r->id);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        printf("Error al modificar la reserva: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    } else {
        printf("Reserva con ID %d modificada correctamente\n", r->id);
        fflush(stdout);
        return 1;
    }
}

int recuperarReservaBD(int idReserva, Reserva *r) {
    char sql[] = "SELECT * FROM reservas WHERE id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    // Vincular el ID de la reserva a la consulta
    sqlite3_bind_int(stmt, 1, idReserva);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Recuperamos los datos de la reserva
        r->id = sqlite3_column_int(stmt, 0);
        strcpy(r->dni_cliente, (const char*)sqlite3_column_text(stmt, 1));
        r->id_habitacion = sqlite3_column_int(stmt, 2);
        strcpy(r->fecha_entrada, (const char*)sqlite3_column_text(stmt, 3));
        strcpy(r->fecha_salida, (const char*)sqlite3_column_text(stmt, 4));
        strcpy(r->estado, (const char*)sqlite3_column_text(stmt, 5));
        r->monto = sqlite3_column_int(stmt, 6);
        strcpy(r->observaciones, (const char*)sqlite3_column_text(stmt, 7));

        sqlite3_finalize(stmt);
        return 1;
    } else {
        sqlite3_finalize(stmt);
        return 0;
    }
}

void eliminarReservaBD(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Recibir el ID de la reserva a eliminar
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        int id_reserva = atoi(recvBuff);

        Reserva *r = (Reserva*) malloc(sizeof(Reserva));
		memset(r, 0, sizeof(Reserva));

        // Verificar si la reserva existe
        if (recuperarReservaBD(id_reserva, r) != 0) {
            strcpy(sendBuff, "ERROR: No existe reserva con ese ID");
            printf("Reserva con ID %d no existe en la BD\n", id_reserva);
        } else {
            // Eliminar la reserva de la BD
            char sql[] = "DELETE FROM reservas WHERE id = ?";
            sqlite3_stmt *stmt;

            if (sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) != SQLITE_OK) {
                strcpy(sendBuff, "ERROR: Error al preparar la consulta de eliminación");
                printf("Error al preparar la consulta de eliminación: %s\n", sqlite3_errmsg(db));
            } else {
                sqlite3_bind_int(stmt, 1, id_reserva);
                int result = sqlite3_step(stmt);

                if (result != SQLITE_DONE) {
                    strcpy(sendBuff, "Reserva no eliminada correctamente");
                    printf("Error al eliminar la reserva: %s\n", sqlite3_errmsg(db));
                } else {
                    strcpy(sendBuff, "Reserva eliminada correctamente");
                    printf("Reserva con ID %d eliminada de la BD\n", id_reserva);
                }

                sqlite3_finalize(stmt);
            }
        }

        send(comm_socket, sendBuff, strlen(sendBuff), 0);
        free(r);
    }

}

int comprobarReserva(const char *idCliente) {
    char sql[] = "SELECT count(*) FROM reservas WHERE id_cliente = ?";
    sqlite3_stmt *stmt;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, idCliente, strlen(idCliente), SQLITE_STATIC);
    int result = sqlite3_step(stmt);

    if (result == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count > 0;
}

char* buscarReservaBD(int id_reserva) {
	 static char resultBuffer[1024]; // Buffer estático para almacenar resultados
	resultBuffer[0] = '\0'; // Inicializar el buffer vacío
	sqlite3_stmt *stmt;
	int result;
    const char *sql = "SELECT id, id_cliente, id_habitacion, fecha_entrada, fecha_salida, estado, monto, observaciones FROM reservas WHERE id = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
	  sprintf(resultBuffer, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
	  return resultBuffer;
	}



	if (sqlite3_bind_int(stmt, 1, id_reserva) != SQLITE_OK) {
	  sprintf(resultBuffer, "Error al enlazar parámetros: %s\n", sqlite3_errmsg(db));
	  sqlite3_finalize(stmt);
	  return resultBuffer;
	}

	strcat(resultBuffer, "Resultados de la búsqueda:\n");
	strcat(resultBuffer, "ID  Cliente  Habitación  Entrada      Salida       Estado      Monto    Observaciones\n");
	strcat(resultBuffer, "-----------------------------------------------------------\n");

	bool foundResult = false;

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
	  foundResult = true;
	  char temp[512];
	  snprintf(temp, sizeof(temp), "%d|%s|%d|%s|%s|%s|%f|%s",
				  sqlite3_column_int(stmt, 0),
				  sqlite3_column_text(stmt, 1),
				  sqlite3_column_int(stmt, 2),
				  sqlite3_column_text(stmt, 3),
				  sqlite3_column_text(stmt, 4),
				  sqlite3_column_text(stmt, 5),
				  sqlite3_column_double(stmt, 6),
				  sqlite3_column_text(stmt, 7));

	  strcat(resultBuffer, temp);
	}

	if (!foundResult) {
		strcat(resultBuffer, "No se encontraron reservas que coincidan con el criterio de búsqueda.\n");
	}

	if (result != SQLITE_DONE && result != SQLITE_ROW) {
		char temp[256];
		sprintf(temp, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
		strcat(resultBuffer, temp);
	}

	sqlite3_finalize(stmt);
	return resultBuffer;
}

char* listaReservasBD() {
    static char resultBuffer[8192]; // Buffer estático para almacenar resultados
    resultBuffer[0] = '\0'; // Inicializar el buffer vacío
    sqlite3_stmt *stmt;
    int result;

    const char *sql = "SELECT id, id_cliente, id_habitacion, fecha_entrada, fecha_salida, estado, monto, observaciones FROM reservas ORDER BY id";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sprintf(resultBuffer, "Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return resultBuffer;
    }

    strcat(resultBuffer, "Lista de reservas:\n");
    strcat(resultBuffer, "ID  Cliente  Habitación  Entrada      Salida       Estado      Monto    Observaciones\n");
    strcat(resultBuffer, "---------------------------------------------------------------------------------\n");

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
    	char temp[8192];
		sprintf(temp, "%d, %s, %d, %s, %s, %s, %f, %s\n",
				sqlite3_column_int(stmt, 0),
				(const char*)sqlite3_column_text(stmt, 1),
				sqlite3_column_int(stmt, 2),
				(const char*)sqlite3_column_text(stmt, 3),
				(const char*)sqlite3_column_text(stmt, 4),
				(const char*)sqlite3_column_text(stmt, 5),
				sqlite3_column_double(stmt, 6),
				(const char*)sqlite3_column_text(stmt, 7));

        strcat(resultBuffer, temp);
    }

        if (result != SQLITE_DONE) {
            char temp[256];
            sprintf(temp, "Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
            strcat(resultBuffer, temp);
        }

        sqlite3_finalize(stmt);
        return resultBuffer;
}
