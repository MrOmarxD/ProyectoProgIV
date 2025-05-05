#include "gestorClientes.h"

void crearCliente(Cliente *client) {
    printf("Ingrese DNI: ");
    fflush(stdout);

    while (getchar() != '\n');

    fgets(client->dni, 50, stdin);
    client->dni[strcspn(client->dni, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese nombre completo: ");
    fflush(stdout);


    fgets(client->nombre, 50, stdin);
    client->nombre[strcspn(client->nombre, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese apellido: ");
    fflush(stdout);

    fgets(client->apellido, 50, stdin);
    client->apellido[strcspn(client->apellido, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese telefono: ");
    fflush(stdout);


    fgets(client->telefono, 50, stdin);
    client->telefono[strcspn(client->telefono, "\n")] = '\0'; // Eliminar el salto de línea

    do {
		printf("Ingrese email: ");
		fflush(stdout);

		//while (getchar() != '\n');

		fgets(client->email, 50, stdin);
		client->email[strcspn(client->email, "\n")] = '\0'; // Eliminar el salto de línea
		if (comprobarUsuario(client->email)) {
			printf("El nombre de usuario ya existe. Por favor, elija otro.\n\n");
			fflush(stdout);
		} else {
			break;
		}
	} while (1);


    crearClienteBD(client);
}

void modificarCliente(char *dni_recibido, int socket_cliente) {
    Cliente client;
    char buffer[1024];
    int bytes_recibidos;

    // Eliminar posible salto de línea en el DNI
    dni_recibido[strcspn(dni_recibido, "\n")] = '\0';

    // Verificar si el cliente existe en la base de datos
    if (!recuperarClienteBD(dni_recibido, &client)) {
        // Informar al cliente que no se encontró
        char respuesta[] = "CLIENTE_NO_ENCONTRADO";
        send(socket_cliente, respuesta, strlen(respuesta), 0);
        printf("Cliente con DNI %s no encontrado.\n", dni_recibido);
        fflush(stdout);
        return;
    }

    // Enviar los datos actuales al cliente
    sprintf(buffer, "CLIENTE_ENCONTRADO:%s:%s:%s:%s:%s",
            client.dni, client.nombre, client.apellido, client.telefono, client.email);
    send(socket_cliente, buffer, strlen(buffer), 0);

    printf("Cliente con DNI %s encontrado. Esperando modificaciones.\n", dni_recibido);
    fflush(stdout);

    // Esperar a recibir los datos modificados
    memset(buffer, 0, sizeof(buffer));
    bytes_recibidos = recv(socket_cliente, buffer, sizeof(buffer), 0);
    if (bytes_recibidos <= 0) {
        printf("Error recibiendo datos modificados.\n");
        fflush(stdout);
        return;
    }
    buffer[bytes_recibidos] = '\0';

    // Verificar que es una modificación
    if (strncmp(buffer, "DATOS_MODIFICADOS:", 17) != 0) {
        printf("Formato incorrecto de datos recibidos.\n");
        fflush(stdout);
        return;
    }

    // Parsear los datos recibidos
    char datos[1024];
    strcpy(datos, buffer + 17); // Saltar el prefijo "DATOS_MODIFICADOS:"

    // Formato: dni:nombre:apellido:telefono:email
    sscanf(datos, "%[^:]:%[^:]:%[^:]:%[^:]:%[^:]",
           client.dni, client.nombre, client.apellido, client.telefono, client.email);

    printf("Datos recibidos para modificación:\n");
    printf("DNI: %s\n", client.dni);
    printf("Nombre: %s\n", client.nombre);
    printf("Apellido: %s\n", client.apellido);
    printf("Teléfono: %s\n", client.telefono);
    printf("Email: %s\n", client.email);
    fflush(stdout);

    // Modificar en la base de datos
    modificarClienteBD(&client);

    // Enviar confirmación de modificación exitosa
    char respuesta[] = "MODIFICACION_EXITOSA";
    send(socket_cliente, respuesta, strlen(respuesta), 0);

    printf("Cliente con DNI %s modificado correctamente.\n", client.dni);
    fflush(stdout);
}

void buscarCliente(Cliente *client){
	char dni[20];

	printf("\n--- BUSCAR CLIENTE ---\n");
	printf("Ingrese el dni de cliente que quiera buscar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(dni, 20, stdin);
	dni[strcspn(dni, "\n")] = '\0';

	buscarClientesBD(dni);
}


