#include "gestorUsuarios.h"

void eliminarUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff){
	// Recibir el nombre de usuario a eliminar
	memset(recvBuff, 0, 512);
	int bytes = recv(comm_socket, recvBuff, 512, 0);
	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		// Verificar si el usuario existe
		if (!comprobarUsuario(recvBuff)) {
			strcpy(sendBuff, "ERROR: El usuario no existe en la base de datos");
			printf("Usuario %s no existe en la BD\n", recvBuff);
		} else {
			// Eliminar usuario de la BD
			if (eliminarUsuarioBD(recvBuff) != 1) {
				strcpy(sendBuff, "Usuario no eliminado correctamente");
				printf("Error al eliminar el usuario %s de la BD\n", recvBuff);
			} else {
				strcpy(sendBuff, "Usuario eliminado correctamente");
				printf("Usuario %s eliminado de la BD\n", recvBuff);
			}
		}

		send(comm_socket, sendBuff, strlen(sendBuff), 0);
	}
}

void iniciarSesion(SOCKET comm_socket, char *recvBuff, char *sendBuff){
	char usu[20] = {0}, con[20] = {0};
	int resul;

	// Get username
	memset(recvBuff, 0, 512);
	int bytes_username = recv(comm_socket, recvBuff, 512, 0);
	if (bytes_username <= 0) {
		strcpy(sendBuff, "Error al recibir el nombre de usuario");
		send(comm_socket, sendBuff, strlen(sendBuff), 0);
		return;
	}

	strncpy(usu, recvBuff, sizeof(usu)-1);
	usu[sizeof(usu)-1] = '\0';

	// Get password
	memset(recvBuff, 0, 512);
	int bytes_password = recv(comm_socket, recvBuff, 512, 0);
	if (bytes_password <= 0) {
		strcpy(sendBuff, "Error al recibir la contraseña");
		send(comm_socket, sendBuff, strlen(sendBuff), 0);
		return;
	}

	strncpy(con, recvBuff, sizeof(con)-1);
	con[sizeof(con)-1] = '\0';

	// Send confirmation
	sprintf(sendBuff, "Servidor: Recibido %s %s", usu, con);
	send(comm_socket, sendBuff, strlen(sendBuff), 0);

	// Properly initialize and use the Usuario structure
	Usuario *u = (Usuario*) malloc(sizeof(Usuario));
	memset(u, 0, sizeof(Usuario)); // Initialize with zeros

	if (recuperarUsuarioBD(usu, u) != 0) {
		if (strcmp(con, u->password) == 0) {
			resul = 0;

			// Preparar para enviar los datos del usuario al cliente después del resultado
			memset(sendBuff, 0, 512);
			sprintf(sendBuff, "%d", resul);
			send(comm_socket, sendBuff, strlen(sendBuff), 0);

			// Enviar los datos del usuario como una cadena delimitada por '|'
			memset(sendBuff, 0, 512);
			sprintf(sendBuff, "%s|%s|%s|%s|%s|%d",
				u->nombre, u->rol, u->usuario, u->password, u->turno, u->salario);
			send(comm_socket, sendBuff, strlen(sendBuff), 0);
		} else {
			resul = 1;
			// Solo enviar el resultado (error de contraseña)
			memset(sendBuff, 0, 512);
			sprintf(sendBuff, "%d", resul);
			send(comm_socket, sendBuff, strlen(sendBuff), 0);
		}
	} else {
		resul = 2;
		// Solo enviar el resultado (usuario no encontrado)
		memset(sendBuff, 0, 512);
		sprintf(sendBuff, "%d", resul);
		send(comm_socket, sendBuff, strlen(sendBuff), 0);
	}

	free(u);
	printf("Verificación de credenciales completada: %d\n", resul);
}

void listarUsuarios(SOCKET comm_socket, char *recvBuff, char *sendBuff){
	// Obtener la lista de usuarios desde la BD
	char* listaDeUsuarios = listaUsuarios();

	// Asegurarse de que no exceda el tamaño del buffer
	strncpy(sendBuff, listaDeUsuarios, 511);
	sendBuff[511] = '\0'; // Garantizar terminación con NULL

	// Enviar la respuesta al cliente
	send(comm_socket, sendBuff, strlen(sendBuff), 0);

	printf("Enviada lista de usuarios al cliente\n");
}

void crearUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
	// Procesar registro de nuevo usuario
	memset(recvBuff, 0, 512);
	int bytes = recv(comm_socket, recvBuff, 512, 0);
	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		printf("Datos de usuario recibidos: %s\n", recvBuff);

		// Extraer datos del usuario
		Usuario *nuevoUsuario = (Usuario*) malloc(sizeof(Usuario));
		memset(nuevoUsuario, 0, sizeof(Usuario));

		// Parsear los datos separados por '|'
		char *token;
		char *rest = recvBuff;

		// Nombre
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoUsuario->nombre, token);

		// Rol
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoUsuario->rol, token);

		// Usuario
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoUsuario->usuario, token);

		// Password
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoUsuario->password, token);

		// Turno
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoUsuario->turno, token);

		// Salario
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) nuevoUsuario->salario = atoi(token);

		printf("Datos extraídos - Nombre: %s, Rol: %s, Usuario: %s, Password: %s, Turno: %s, Salario: %d\n",
			  nuevoUsuario->nombre, nuevoUsuario->rol, nuevoUsuario->usuario,
			  nuevoUsuario->password, nuevoUsuario->turno, nuevoUsuario->salario);

		// Verificar si el usuario ya existe
		if (comprobarUsuario(nuevoUsuario->usuario)) {
			strcpy(sendBuff, "ERROR: El nombre de usuario ya existe");
			printf("Usuario %s ya existe en la BD\n", nuevoUsuario->usuario);
		} else {
			// Crear usuario en la BD
			if (crearUsuarioBD(nuevoUsuario) != 1) {
				strcpy(sendBuff, "Usuario no registrado correctamente");
				printf("Usuario %s no a quedado registrado en la BD\n", nuevoUsuario->usuario);
			} else {
				strcpy(sendBuff, "Usuario registrado correctamente");
				printf("Usuario %s registrado en la BD\n", nuevoUsuario->usuario);

			}
		}

		free(nuevoUsuario);
		send(comm_socket, sendBuff, strlen(sendBuff), 0);
	}
}

void modificarUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
	// Solicitar el nombre de usuario a modificar
	memset(recvBuff, 0, 512);
	int bytes = recv(comm_socket, recvBuff, 512, 0);
	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		char nombreUsuario[20];
		strncpy(nombreUsuario, recvBuff, sizeof(nombreUsuario)-1);
		nombreUsuario[sizeof(nombreUsuario)-1] = '\0';

		// Verificar si el usuario existe
		Usuario *u = (Usuario*) malloc(sizeof(Usuario));
		memset(u, 0, sizeof(Usuario));

		if (recuperarUsuarioBD(nombreUsuario, u) == 0) {
			strcpy(sendBuff, "ERROR: El usuario no existe en la base de datos");
			printf("Usuario %s no existe en la BD\n", nombreUsuario);
			free(u);
		} else {
			// Enviar los datos actuales del usuario al cliente
			memset(sendBuff, 0, 512);
			sprintf(sendBuff, "%s|%s|%s|%s|%s|%d",
				u->nombre, u->rol, u->usuario, u->password, u->turno, u->salario);
			send(comm_socket, sendBuff, strlen(sendBuff), 0);

			// Recibir los datos actualizados del usuario
			memset(recvBuff, 0, 512);
			bytes = recv(comm_socket, recvBuff, 512, 0);
			if (bytes > 0) {
				recvBuff[bytes] = '\0'; // Asegurar terminación
				printf("Datos actualizados del usuario recibidos: %s\n", recvBuff);

				// Parsear los datos separados por '|'
				char *token;
				char *rest = recvBuff;

				// Nombre
				token = strtok_s(rest, "|", &rest);
				if (token != NULL) strcpy(u->nombre, token);

				// Rol
				token = strtok_s(rest, "|", &rest);
				if (token != NULL) strcpy(u->rol, token);

				// Usuario (no cambia, es la clave primaria)

				// Password
				token = strtok_s(rest, "|", &rest);
				if (token != NULL) strcpy(u->password, token);

				// Turno
				token = strtok_s(rest, "|", &rest);
				if (token != NULL) strcpy(u->turno, token);

				// Salario
				token = strtok_s(rest, "|", &rest);
				if (token != NULL) u->salario = atoi(token);

				// Actualizar el usuario en la BD
				if (modificarUsuarioBD(u) != 1) {
					strcpy(sendBuff, "Usuario no modificado correctamente");
					printf("Error al modificar el usuario %s en la BD\n", u->usuario);
				} else {
					strcpy(sendBuff, "Usuario modificado correctamente");
					printf("Usuario %s modificado en la BD\n", u->usuario);
				}
			}
			free(u);
		}

		send(comm_socket, sendBuff, strlen(sendBuff), 0);
	}
}

void buscarUsuario(SOCKET comm_socket, char *recvBuff, char *sendBuff){
	// Recibir el criterio de búsqueda
	memset(recvBuff, 0, 512);
	int bytes = recv(comm_socket, recvBuff, 512, 0);
	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		printf("Búsqueda de usuario por criterio: %s\n", recvBuff);

		// Realizar la búsqueda en la base de datos
		char* resultadoBusqueda = buscarUsuarioBD(recvBuff);

		// Enviar resultados al cliente
		memset(sendBuff, 0, 512);
		strncpy(sendBuff, resultadoBusqueda, 511);
		sendBuff[511] = '\0'; // Asegurar terminación

		send(comm_socket, sendBuff, strlen(sendBuff), 0);
		printf("Resultados de búsqueda enviados al cliente\n");
	}
}

