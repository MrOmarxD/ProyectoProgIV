#ifndef GESTIONBD_H_
#define GESTIONBD_H_

#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include "../modulos/gestorUsuarios.h"
#include "../modulos/gestorClientes.h"
#include "../modulos/gestorHabitaciones.h"
#include "../modulos/gestorFacturas.h"
#include "../modulos/gestorReservas.h"

void abrirBd();
void cerrarBd();


// Funciones Usuarios
int comprobarUsuario(const char *usuario);
int crearUsuarioBD(Usuario *user);
int modificarUsuarioBD(Usuario *user);
char* listaUsuarios();
int eliminarUsuarioBD();
int comprobarUsuario(const char *usuario);
int recuperarUsuarioBD(const char *nombreUsuario, Usuario *user);
void buscarUsuarioBD(const char *nombreUsuario);

// Funciones Clientes
int crearClienteBD(Cliente *client);
int comprobarCliente(const char *cliente);
void modificarClienteBD(Cliente *client);
int recuperarClienteBD(const char *dniCliente, Cliente *client);
char* listaClientes();
void buscarClientesBD(const char *dniCliente, SOCKET comm_socket);

// Funciones Habitaciones
void crearHabitacionBD(Habitacion *habitacion);
void modificarHabitacionBD(Habitacion *habitacion);
int recuperarHabitacionBD(const char *numHabitacion, Habitacion *habitacion);
void cambiarEstadoHabitacionBD();
char* listarHabitaciones();
void buscarHabitacionBD(const char *numHabitacion);

// Funciones Facturas
void crearFacturaBD(Factura *factura);
int buscarFacturaBD(const char *numero_factura, Factura *factura);

// Funciones Reservas
int crearReservaBD(Reserva *r);
void modificarReservaBD(Reserva *r);
int recuperarReservaBD(const char *idUR, Reserva *r);
void eliminarReservaBD();
void buscarReservaBD(const char *reserva);
void listarReservaBD();

#endif /* GESTIONBD_H_ */
