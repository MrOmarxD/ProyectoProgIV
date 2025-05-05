#include "gestorReservas.h"

#include <stdio.h>
#include "gestorMenus.h"
using namespace std;

void gestionReservas(SOCKET s) {
    int opcion;
    Reserva r;

    cout<<"\n--- GESTIÓN DE RESERVAS ---\n";
    cout<<"1. Crear nueva reserva\n";
    cout<<"2. Modificar reserva\n";
    cout<<"3. Borrar reserva\n";
    cout<<"4. Buscar reservas por cliente\n";
    cout<<"5. Listar reservas activas\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: " <<endl;
	cin >> opcion;

    switch (opcion) {
                case 1:
                	crearReserva(s);
					break;
                case 2:
                	//modificarReserva(&r);
                    break;
                case 3:
                	//eliminarReservaBD();
                    break;
                case 4:
                	//buscarReservas(&r);
                    break;
                case 5:
                	//listarReserva();
                    break;
                case 0:
                	mostrarMenuPrincipal(s);
                	break;
                default:
                    cout<<"Opción no válida. Intente nuevamente.\n" <<endl;
            }

    //registrarActividad(usuario_actual, "Acceso a gestión de reservas", LOG_FILE);

}
