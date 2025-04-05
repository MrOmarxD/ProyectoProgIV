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

void listaUsuarios(){
	char sql2[] = "select nombre from usuarios";

		sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;

		printf("\n");
		do {
			result = sqlite3_step(stmt);
			if (result == SQLITE_ROW) {
				printf("%s\n", (char*) sqlite3_column_text(stmt, 0));
			}
		} while (result == SQLITE_ROW);
		printf("\n");

		sqlite3_finalize(stmt);
}

void eliminarUsuarioBD() {
    char nombreUsuario[50];

    printf("\n--- ELIMINAR USUARIO ---\n");
    printf("Ingrese el nombre de usuario a eliminar: ");
    fflush(stdout);
    scanf("%49s", nombreUsuario);  // Limitamos la entrada a 49 caracteres + el terminador nulo

    if (!comprobarUsuario(nombreUsuario)) {
        printf("El usuario '%s' no existe en la base de datos.\n", nombreUsuario);
        fflush(stdout);
        return;
    }

    char sql[] = "DELETE FROM usuarios WHERE nombre_usuario = ?";

    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

    sqlite3_bind_text(stmt, 1, nombreUsuario, strlen(nombreUsuario), SQLITE_STATIC);
    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error al eliminar el usuario: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
    } else {
        printf("Usuario '%s' eliminado correctamente\n", nombreUsuario);
        fflush(stdout);
    }

    sqlite3_finalize(stmt);
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


void crearUsuarioBD(Usuario *user) {
    char sql1[] = "INSERT INTO usuarios (id, nombre, rol, nombre_usuario, contraseña, turno, salario) VALUES (NULL, ?, ?, ?, ?, ?, ?)";
    sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user->nombre, strlen(user->nombre), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user->rol, strlen(user->rol), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user->usuario, strlen(user->usuario), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user->password, strlen(user->password), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, user->turno, strlen(user->turno), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, user->salario);

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando el usuario: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
    } else {
        printf("Usuario %s insertado\n", user->nombre);
        fflush(stdout);
    }

    sqlite3_finalize(stmt);
}

void modificarUsuarioBD(Usuario *user){
	    char sql[] = "UPDATE usuarios SET nombre = ?, rol = ?, contraseña = ?, turno = ?, salario = ? WHERE nombre_usuario = ?";
	    sqlite3_stmt *stmt;

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

		sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
		sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, rol, strlen(rol), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 6, usuario, strlen(usuario), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, password, strlen(password), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 4, turno, strlen(turno), SQLITE_STATIC);
		sqlite3_bind_int(stmt, 5, salario);

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			printf("Error al modificar el usuario: %s\n", sqlite3_errmsg(db));
			fflush(stdout);
		} else {
			printf("Usuario '%s' modificado correctamente\n", user->usuario);
			fflush(stdout);
		}

		sqlite3_finalize(stmt);
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

void buscarUsuarioBD(const char *nombreUsuario){
	char sql2[] = "SELECT nombre, rol, nombre_usuario, contraseña, turno, salario FROM usuarios WHERE nombre_usuario = ?";

		sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;

		sqlite3_bind_text(stmt, 1, nombreUsuario, strlen(nombreUsuario), SQLITE_STATIC);

		printf("\n");
		do {
			result = sqlite3_step(stmt);
			if (result == SQLITE_ROW) {
				printf("%s, %s, %s, %s, %s, %d\n", (char*) 	sqlite3_column_text(stmt, 0),
															sqlite3_column_text(stmt, 1),
															sqlite3_column_text(stmt, 2),
															sqlite3_column_text(stmt, 3),
															sqlite3_column_text(stmt, 4),
															sqlite3_column_int(stmt, 5));
			}
		} while (result == SQLITE_ROW);
		printf("\n");

		sqlite3_finalize(stmt);
}

// FUNCIONALIDAD PARA CLIENTE ----------------------------------------------------------------------------------------------------------------------------------------

void crearClienteBD(Cliente *client){
	char sql1[] = "insert into clientes (dni, nombre, apellido, telefono, email, fecha_registro) values (?, ?, ?, ?, ?, ?)";
		char dni[10];
	    char nombre[50];
	    char apellido[50];
	    char telefono[15];
	    char email[50];

		strcpy(dni, client->dni);
		strcpy(nombre, client->nombre);
		strcpy(apellido, client->apellido);
		strcpy(telefono, client->telefono);
		strcpy(email, client->email);

		// Obtener la fecha actual
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		// Formatear la fecha como texto
		char fecha[20];
		snprintf(fecha, sizeof(fecha), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

		sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) ;
		sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, nombre, strlen(nombre), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, apellido, strlen(apellido), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 4, telefono, strlen(telefono), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 5, email, strlen(email), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 6, fecha, strlen(fecha), SQLITE_STATIC);

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			printf("Error insertando el cliente\n");
			fflush(stdout);
		}else{
			printf("Cliente %s insertado\n", dni);
			fflush(stdout);
		}

		sqlite3_finalize(stmt);
		mostrarMenuPrincipal();
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

void modificarClienteBD(Cliente *client){
	    char sql[] = "UPDATE clientes SET nombre = ?, apellido = ?, telefono = ?, email = ? WHERE dni = ?";
	    sqlite3_stmt *stmt;

	    char dni[10];
		char nombre[50];
		char apellido[50];
		char telefono[15];
		char email[50];

		strcpy(dni, client->dni);
		strcpy(nombre, client->nombre);
		strcpy(apellido, client->apellido);
		strcpy(telefono, client->telefono);
		strcpy(email, client->email);

		sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
		sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, apellido, strlen(apellido), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, telefono, strlen(telefono), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 4, email, strlen(email), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 5, dni, strlen(dni), SQLITE_STATIC);

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			printf("Error al modificar el cliente: %s\n", sqlite3_errmsg(db));
			fflush(stdout);
		} else {
			printf("Cliente '%s' modificado correctamente\n", client->dni);
			fflush(stdout);
		}

		sqlite3_finalize(stmt);
		mostrarMenuPrincipal();
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

void listarClientes(){
	char sql2[] = "select dni, nombre from clientes";

		sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;

		printf("\n");
		do {
			result = sqlite3_step(stmt);
			if (result == SQLITE_ROW) {
				printf("%s, %s\n", (char*) sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1));
			}
		} while (result == SQLITE_ROW);
		printf("\n");

		sqlite3_finalize(stmt);
}

void buscarClientesBD(const char *dniCliente){
	char sql2[] = "SELECT dni, nombre, apellido, telefono, email, fecha_registro FROM clientes WHERE dni = ?";

		sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;

		sqlite3_bind_text(stmt, 1, dniCliente, strlen(dniCliente), SQLITE_STATIC);

		printf("\n");
		do {
			result = sqlite3_step(stmt);
			if (result == SQLITE_ROW) {
				printf("%s, %s, %s, %s, %s, %s\n", (char*) 	sqlite3_column_text(stmt, 0),
															sqlite3_column_text(stmt, 1),
															sqlite3_column_text(stmt, 2),
															sqlite3_column_text(stmt, 3),
															sqlite3_column_text(stmt, 4),
															sqlite3_column_text(stmt, 5));
			}
		} while (result == SQLITE_ROW);
		printf("\n");

		sqlite3_finalize(stmt);
}


// FUNCIONALIDAD PARA HABITACIONES ----------------------------------------------------------------------------------------------------------------------------------------

void crearHabitacionBD(Habitacion *habitacion) {
    char sql[] = "INSERT INTO habitaciones (numero, tipo, precio, estado, capacidad, descripcion) VALUES (?, ?, ?, ?, ?, ?)";
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

void modificarHabitacionBD(Habitacion *habitacion){
	char sql[] = "UPDATE habitaciones SET tipo = ?, precio = ?, estado = ?, capacidad = ?, descripcion = ? WHERE numero = ?";
		    sqlite3_stmt *stmt;

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

			sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
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
        printf("Cliente no encontrado.\n");
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    }
}

void listarHabitaciones(){
	char sql2[] = "select numero from habitaciones";

			sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;

			printf("\n");
			do {
				result = sqlite3_step(stmt);
				if (result == SQLITE_ROW) {
					printf("%s\n", (char*) sqlite3_column_text(stmt, 0));
				}
			} while (result == SQLITE_ROW);
			printf("\n");

			sqlite3_finalize(stmt);
}

void buscarHabitacionBD(const char *numHabitacion){
	char sql2[] = "SELECT numero, tipo, precio, estado, capacidad, descripcion FROM habitaciones WHERE numero = ?";

			sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;

			sqlite3_bind_text(stmt, 1, numHabitacion, strlen(numHabitacion), SQLITE_STATIC);

			printf("\n");
			do {
				result = sqlite3_step(stmt);
				if (result == SQLITE_ROW) {
					printf("%s, %s, %d, %s, %d, %s\n", (char*) 	sqlite3_column_text(stmt, 0),
																sqlite3_column_text(stmt, 1),
																sqlite3_column_int(stmt, 2),
																sqlite3_column_text(stmt, 3),
																sqlite3_column_int(stmt, 4),
																sqlite3_column_text(stmt, 5));
				}
			} while (result == SQLITE_ROW);
			printf("\n");

			sqlite3_finalize(stmt);
}
