#include "gestorUsuarios.h"

using namespace std;

void gestionUsuarios(SOCKET s) {
    int opcion;
    cout<<"\n--- GESTIÓN DE USUARIOS ---\n";
    cout<<"1. Crear nuevo usuario\n";
    cout<<"2. Modificar usuario existente\n";
    cout<<"3. Eliminar usuario\n";
    cout<<"4. Listar usuarios\n";
    cout<<"5. Buscar usuario\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: " << endl;
    cin >> opcion;

    switch (opcion) {
                case 1:
                	cout<<"\n---CREAR NUEVO USUARIO---\n"<< endl;
                	//crearUsuario();
                    break;
                case 2:
                	//modificarUsuario(&usuario);
                    break;
                case 3:
                	//eliminarUsuarioBD();
                    break;
                case 4:
                	//listaUsuarios();
                    break;
                case 5:
                	//buscarUsuario(&usuario);
					break;
                case 0:
                	mostrarMenuPrincipal(s);
                	break;
                default:
                    cout<<"Opción no válida. Intente nuevamente.\n"<< endl;
            }


    //registrarActividad(usuario_actual, "Acceso a gestión de usuarios", LOG_FILE);
}
