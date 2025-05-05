#include "gestorHabitaciones.h"
//#include "gestorRegistros.h"

#include <stdio.h>
#include "gestorMenus.h"
using namespace std;

void gestionHabitaciones(SOCKET s) {
	Habitacion h;
    int opcion;

    cout<<"\n--- GESTIÓN DE HABITACIONES ---\n";
    cout<<"1. Añadir nueva habitación\n";
    cout<<"2. Modificar habitación\n";
    cout<<"3. Cambiar estado de habitación\n";
    cout<<"4. Listar habitaciones\n";
    cout<<"5. Buscar habitaciones\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: "<<endl;
    cin >> opcion;

    switch (opcion) {
                case 1:
//                	crearHabitacion(&h);
                    break;
                case 2:
//                	modificarHabitacion(&h);
                    break;
                case 3:
//                	establecerEstadoHabitacion(&h);
                    break;
                case 4:
//                	listarHabitaciones();
                    break;
                case 5:
//					buscarHabitacion(&h);
					break;
                case 0:
                	mostrarMenuPrincipal(s);
                	break;
                default:
                    cout<<"Opción no válida. Intente nuevamente.\n" <<endl;
            }

    //registrarActividad(usuario_actual, "Acceso a gestión de habitaciones", LOG_FILE);
}
