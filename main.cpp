#include <iostream>            
#include <vector>              
#include <string>             
#include <algorithm>           // Para usar remove_if en eliminar ventas

#include "Venta.h"             // Struct con los datos de cada venta
#include "HashMapList.h"       // Estructura de HashMap provista por la cátedra
#include "procesarEstadisticas.cpp"  // Funciones estadísticas
#include "CSVReader.cpp"       // Función leerVentasDesdeCSV
using namespace std;           

// Función para reprocesar 
void reprocesarEstadisticas(
    const vector<Venta>& ventas,
    HashMapList<string, double>& ventasPorCiudad,
    HashMapList<string, double>& ventasPorProductoPais,
    HashMapList<string, pair<double, int>>& promedioCategoriaPais,
    HashMapList<string, int>& enviosPorPais,
    HashMapList<string, int>& enviosPorCategoria,
    HashMapList<string, double>& ventasPorFecha,
    HashMapList<string, int>& estadoEnvioPorPais,
    HashMapList<string, int>& cantidadPorProducto
) {
    ventasPorCiudad.vaciar();
    ventasPorProductoPais.vaciar();
    promedioCategoriaPais.vaciar();
    enviosPorPais.vaciar();
    enviosPorCategoria.vaciar();
    ventasPorFecha.vaciar();
    estadoEnvioPorPais.vaciar();
    cantidadPorProducto.vaciar();

    procesarEstadisticas(ventas, ventasPorCiudad, ventasPorProductoPais, promedioCategoriaPais,
                         enviosPorPais, enviosPorCategoria, ventasPorFecha,
                         estadoEnvioPorPais, cantidadPorProducto);
}

// Agregar venta
void agregarVenta(vector<Venta>& ventas) {
    Venta nueva;

    cout << "Ingrese ID: ";
    cin >> nueva.id;
    cin.ignore();

    cout << "Fecha (YYYY-MM-DD): ";
    getline(cin, nueva.fecha);

    cout << "País: ";
    getline(cin, nueva.pais);

    cout << "Ciudad: ";
    getline(cin, nueva.ciudad);

    cout << "Cliente: ";
    getline(cin, nueva.cliente);

    cout << "Producto: ";
    getline(cin, nueva.producto);

    cout << "Categoría: ";
    getline(cin, nueva.categoria);

    cout << "Cantidad: ";
    cin >> nueva.cantidad;
    cin.ignore();

    cout << "Precio unitario: ";
    cin >> nueva.precioUnitario;
    cin.ignore();

    nueva.montoTotal = nueva.cantidad * nueva.precioUnitario;

    cout << "Medio de envío: ";
    getline(cin, nueva.medioEnvio);

    cout << "Estado de envío: ";
    getline(cin, nueva.estadoEnvio);

    ventas.push_back(nueva);
    cout << "✅ Venta agregada correctamente." << endl;
}

// Eliminar ventas por filtro
void eliminarVentasPorFiltro(vector<Venta>& ventas) {
    string filtro;
    cout << "Ingrese país o ciudad: ";
    cin.ignore();
    getline(cin, filtro);

    int antes = ventas.size();

    ventas.erase(remove_if(ventas.begin(), ventas.end(), [&](Venta v) {
        return v.pais == filtro || v.ciudad == filtro;
    }), ventas.end());

    int despues = ventas.size();

    cout << "Se eliminaron " << (antes - despues) << " ventas." << endl;
}

// Modificar venta por ID
void modificarVentaPorID(vector<Venta>& ventas) {
    int id;
    cout << "Ingrese ID de la venta: ";
    cin >> id;
    cin.ignore();

    bool encontrada = false;

    for (Venta& v : ventas) {
        if (v.id == id) {
            encontrada = true;

            cout << "Nueva fecha: ";
            getline(cin, v.fecha);

            cout << "Nuevo país: ";
            getline(cin, v.pais);

            cout << "Nueva ciudad: ";
            getline(cin, v.ciudad);

            cout << "Nuevo cliente: ";
            getline(cin, v.cliente);

            cout << "Nuevo producto: ";
            getline(cin, v.producto);

            cout << "Nueva categoría: ";
            getline(cin, v.categoria);

            cout << "Nueva cantidad: ";
            cin >> v.cantidad;
            cin.ignore();

            cout << "Nuevo precio unitario: ";
            cin >> v.precioUnitario;
            cin.ignore();

            v.montoTotal = v.cantidad * v.precioUnitario;

            cout << "Nuevo medio de envío: ";
            getline(cin, v.medioEnvio);

            cout << "Nuevo estado de envío: ";
            getline(cin, v.estadoEnvio);

            cout << "Venta modificada." << endl;
            break;
        }
    }

    if (!encontrada) {
        cout << "No se encontro venta con ese ID." << endl;
    }
}

// Función principal
int main() {
    // Cargar las ventas desde el CSV
    string archivo = "ventas_sudamerica.csv";
    vector<Venta> ventas = leerVentasDesdeCSV(archivo);

    // Crear estructuras para las estadísticas
    HashMapList<string, double> ventasPorCiudad(50);
    HashMapList<string, double> ventasPorProductoPais(50);
    HashMapList<string, pair<double, int>> promedioCategoriaPais(50);
    HashMapList<string, int> enviosPorPais(50);
    HashMapList<string, int> enviosPorCategoria(50);
    HashMapList<string, double> ventasPorFecha(50);
    HashMapList<string, int> estadoEnvioPorPais(50);
    HashMapList<string, int> cantidadPorProducto(50);

    // Procesar estadísticas iniciales
    procesarEstadisticas(ventas, ventasPorCiudad, ventasPorProductoPais, promedioCategoriaPais,
                         enviosPorPais, enviosPorCategoria, ventasPorFecha,
                         estadoEnvioPorPais, cantidadPorProducto);

    int opcion;

    // Menú interactivo
    do {
        cout << "\n========= MENÚ =========" << endl;
        cout << "1. Producto más y menos vendido" << endl;
        cout << "2. Día con mayor monto de ventas" << endl;
        cout << "3. Promedios por categoría y país" << endl;
        cout << "4. Top 5 ciudades por país" << endl;
        cout << "5. Medio de envío más usado por país" << endl;
        cout << "6. Estado de envío más frecuente por país" << endl;
        cout << "7. Agregar nueva venta" << endl;
        cout << "8. Eliminar ventas por país o ciudad" << endl;
        cout << "9. Modificar venta por ID" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        cout << endl;

        switch(opcion) {
            case 1:
                mostrarProductoMasYMenosVendido(cantidadPorProducto);
                break;
            case 2:
                mostrarDiaConMayorVenta(ventasPorFecha);
                break;
            case 3:
                mostrarPromedioVentasPorCategoriaYPais(promedioCategoriaPais);
                break;
            case 4:
                mostrarTop5CiudadesPorPais(ventasPorCiudad);
                break;
            case 5:
                mostrarMedioEnvioMasUsadoPorPais(enviosPorPais);
                break;
            case 6:
                mostrarEstadoEnvioMasFrecuentePorPais(estadoEnvioPorPais);
                break;
            case 7:
                agregarVenta(ventas);
                reprocesarEstadisticas(ventas, ventasPorCiudad, ventasPorProductoPais,
                                       promedioCategoriaPais, enviosPorPais, enviosPorCategoria,
                                       ventasPorFecha, estadoEnvioPorPais, cantidadPorProducto);
                break;
            case 8:
                eliminarVentasPorFiltro(ventas);
                reprocesarEstadisticas(ventas, ventasPorCiudad, ventasPorProductoPais,
                                       promedioCategoriaPais, enviosPorPais, enviosPorCategoria,
                                       ventasPorFecha, estadoEnvioPorPais, cantidadPorProducto);
                break;
            case 9:
                modificarVentaPorID(ventas);
                reprocesarEstadisticas(ventas, ventasPorCiudad, ventasPorProductoPais,
                                       promedioCategoriaPais, enviosPorPais, enviosPorCategoria,
                                       ventasPorFecha, estadoEnvioPorPais, cantidadPorProducto);
                break;
            case 0:
                cout << "👋 ¡Hasta luego!" << endl;
                break;
            default:
                cout << "❌ Opción inválida. Intente otra vez." << endl;
        }

    } while (opcion != 0);

    return 0;
}
