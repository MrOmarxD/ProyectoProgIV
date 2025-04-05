#include "gestionBD.h"

sqlite3 *db;
sqlite3_stmt *stmt;
int result;

int abrirBd(){

	result = sqlite3_open("hotelesBD.sqlite", &db);
    if (result != SQLITE_OK) {
        printf("Error al abrir la BBDD\n");
        fflush(stdout);
        return result;
    }
    printf("BBDD abierta\n");
    fflush(stdout);
}

int cerrarBd(){
	result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error al cerrar la BBDD\n");
        printf("%s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return result;
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
		main();
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
    main();
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

void crearUsuarioBD(Usuario *user){
	char sql1[] = "insert into usuarios (id, nombre, rol, nombre_usuario, contraseña) values (NULL, ?, ?, ?, ?)";
		char nombre[50];
		char rol[20];
		char usuario[20];
		char password[20];

		strcpy(nombre, user->nombre);
		strcpy(rol, user->rol);
		strcpy(usuario, user->usuario);
		strcpy(password, user->password);

		sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL) ;
		sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, rol, strlen(rol), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, usuario, strlen(usuario), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 4, password, strlen(password), SQLITE_STATIC);

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			printf("Error insertando el usuario\n");
			fflush(stdout);
		}else{
			printf("Usuario %s insertado\n", nombre);
			fflush(stdout);
		}

		sqlite3_finalize(stmt);
		main();
}

void modificarUsuarioBD(Usuario *user){
	    char sql[] = "UPDATE usuarios SET nombre = ?, rol = ?, contraseña = ? WHERE nombre_usuario = ?";
	    sqlite3_stmt *stmt;

	    char nombre[50];
		char rol[20];
		char usuario[20];
		char password[20];

		strcpy(nombre, user->nombre);
		strcpy(rol, user->rol);
		strcpy(usuario, user->usuario);
		strcpy(password, user->password);

		sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
		sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, rol, strlen(rol), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 4, usuario, strlen(usuario), SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, password, strlen(password), SQLITE_STATIC);

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			printf("Error al modificar el usuario: %s\n", sqlite3_errmsg(db));
			fflush(stdout);
		} else {
			printf("Usuario '%s' modificado correctamente\n", user->usuario);
			fflush(stdout);
		}

		sqlite3_finalize(stmt);
	    main();
}

int recuperarUsuarioBD(const char *nombreUsuario, Usuario *user) {
    char sql[] = "SELECT nombre, rol, nombre_usuario, contraseña FROM usuarios WHERE nombre_usuario = ?";
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
        sqlite3_finalize(stmt);
        return 1;
    } else {
        printf("Usuario no encontrado.\n");
        fflush(stdout);
        sqlite3_finalize(stmt);
        return 0;
    }
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
		main();
}

