// TOTAL en main.cpp:
// IF explícitos: 2
// IF de estructuras complejas (HashMap, AVL, Lista, etc): 17
// TOTAL ESTIMADO DE IFs: 19
// Archivo: main.cpp
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.

#include <iostream>     
#include <vector>      
#include <string>       
#include <algorithm>                               

#include "Venta.h"                     // Definición de estructura Venta
#include "HashMapList.h"              // Mapa hash con listas
#include "csv_reader.cpp"             // Función para leer el archivo CSV
#include "procesarEstadisticas.cpp"   // Procesamiento estadístico de las ventas

using namespace std;

// ======================= FUNCIONES AUXILIARES =======================

// Vacía los hashmaps y vuelve a procesar todas las estadísticas
void reprocesarEstadisticas(
    const vector<Venta>& ventas,
    HashMapList<string, double>& ventasPorCiudad,
    HashMapList<string, double>& ventasPorProductoPais,
    HashMapList<string, pair<double, int>>& promedioCategoriaPais,
    HashMapList<string, int>& enviosPorPais,
    HashMapList<string, int>& enviosPorCategoria,
    HashMapList<string, double>& ventasPorFecha,
    HashMapList<string, int>& estadoEnvioPorPais,
    HashMapList<string, int>& cantidadPorProducto)
{
    ventasPorCiudad.vaciar();
    ventasPorProductoPais.vaciar();
    promedioCategoriaPais.vaciar();
    enviosPorPais.vaciar();
    enviosPorCategoria.vaciar();
    ventasPorFecha.vaciar();
    estadoEnvioPorPais.vaciar();
    cantidadPorProducto.vaciar();

    // Vuelve a calcular todas las estadísticas
    procesarEstadisticas(
        ventas, ventasPorCiudad, ventasPorProductoPais, promedioCategoriaPais,
        enviosPorPais, enviosPorCategoria, ventasPorFecha,
        estadoEnvioPorPais, cantidadPorProducto);
}

// Permite al usuario cargar una nueva venta manualmente
void agregarVenta(vector<Venta>& ventas) {
    Venta nueva;
    cout << "Ingrese ID: "; cin >> nueva.id; cin.ignore();
    cout << "Fecha (YYYY-MM-DD): "; getline(cin, nueva.fecha);
    cout << "Pais: "; getline(cin, nueva.pais);
    cout << "Ciudad: "; getline(cin, nueva.ciudad);
    cout << "Cliente: "; getline(cin, nueva.cliente);
    cout << "Producto: "; getline(cin, nueva.producto);
    cout << "Categoria: "; getline(cin, nueva.categoria);
    cout << "Cantidad: "; cin >> nueva.cantidad; cin.ignore();
    cout << "Precio unitario: "; cin >> nueva.precioUnitario; cin.ignore();
    nueva.montoTotal = nueva.cantidad * nueva.precioUnitario;
    cout << "Medio de envio: "; getline(cin, nueva.medioEnvio);
    cout << "Estado de envio: "; getline(cin, nueva.estadoEnvio);
    ventas.push_back(nueva); // Agrega al vector
    cout << " Venta agregada correctamente." << endl;
}

// Elimina ventas que coincidan con un país o ciudad ingresado
void eliminarVentasPorFiltro(vector<Venta>& ventas) {
    string filtro;
    // se utilizan 1 cantidad de if
    cout << "Ingrese pais o ciudad: ";
    cin.ignore();
    getline(cin, filtro);

    int antes = ventas.size();
    // Elimina todas las ventas cuyo país o ciudad coincidan
    ventas.erase(remove_if(ventas.begin(), ventas.end(), [&](Venta v) {
        return v.pais == filtro || v.ciudad == filtro;
    }), ventas.end());

    cout << "Se eliminaron " << (antes - ventas.size()) << " ventas." << endl;
}

// Modifica una venta existente buscando por ID
void modificarVentaPorID(vector<Venta>& ventas) {
    int id;
    // se utilizan 2 cantidad de if
    cout << "Ingrese ID de la venta: ";
    cin >> id;
    cin.ignore();
    bool encontrada = false;

    for (Venta& v : ventas) {
        if (v.id == id) {
            encontrada = true;
            cout << "Nueva fecha: "; getline(cin, v.fecha);
            cout << "Nuevo pais: "; getline(cin, v.pais);
            cout << "Nueva ciudad: "; getline(cin, v.ciudad);
            cout << "Nuevo cliente: "; getline(cin, v.cliente);
            cout << "Nuevo producto: "; getline(cin, v.producto);
            cout << "Nueva categoria: "; getline(cin, v.categoria);
            cout << "Nueva cantidad: "; cin >> v.cantidad; cin.ignore();
            cout << "Nuevo precio unitario: "; cin >> v.precioUnitario; cin.ignore();
            v.montoTotal = v.cantidad * v.precioUnitario;
            cout << "Nuevo medio de envio: "; getline(cin, v.medioEnvio);
            cout << "Nuevo estado de envio: "; getline(cin, v.estadoEnvio);
            cout << " Venta modificada correctamente." << endl;
            break;
        }
    }

    if (!encontrada) {
        cout << " No se encontro ninguna venta con ese ID." << endl;
    }
}

// ======================= FUNCIÓN PRINCIPAL =======================

int main() {
    try {
        // ============ DEBUG: MOSTRAR DATOS CARGADOS ================
        vector<Venta> ventas = leerVentasDesdeCSV("ventas_sudamerica.csv");
        cout << "\nVentas cargadas desde el archivo CSV:\n";
        for (const Venta& v : ventas) {
            cout << "ID: " << v.id
                 << " | Fecha: " << v.fecha
                 << " | Pais: " << v.pais
                 << " | Ciudad: " << v.ciudad
                 << " | Cliente: " << v.cliente
                 << " | Producto: " << v.producto
                 << " | Categoria: " << v.categoria
                 << " | Cantidad: " << v.cantidad
                 << " | Precio: " << v.precioUnitario
                 << " | Total: " << v.montoTotal
                 << " | Medio: " << v.medioEnvio
                 << " | Estado: " << v.estadoEnvio << endl;
        }
        cout << "======================================================" << endl;
        // ==============================================================

        // Inicialización de estructuras de estadísticas
        HashMapList<string, double> ventasPorCiudad(50);
        HashMapList<string, double> ventasPorProductoPais(50);
        HashMapList<string, pair<double, int>> promedioCategoriaPais(50);
        HashMapList<string, int> enviosPorPais(50);
        HashMapList<string, int> enviosPorCategoria(50);
        HashMapList<string, double> ventasPorFecha(50);
        HashMapList<string, int> estadoEnvioPorPais(50);
        HashMapList<string, int> cantidadPorProducto(50);

        // Procesamiento inicial de estadísticas
        procesarEstadisticas(ventas, ventasPorCiudad, ventasPorProductoPais,
                             promedioCategoriaPais, enviosPorPais, enviosPorCategoria,
                             ventasPorFecha, estadoEnvioPorPais, cantidadPorProducto);

        int opcion;

        // ======================= MENÚ INTERACTIVO =======================
        do {
            cout << "\n========= MENU =========" << endl;
            cout << "1. Producto mas y menos vendido" << endl;
            cout << "2. Dia con mayor monto de ventas" << endl;
            cout << "3. Promedios por categoria y pais" << endl;
            cout << "4. Top 5 ciudades por pais" << endl;
            cout << "5. Medio de envio mas usado por pais" << endl;
            cout << "6. Estado de envio mas frecuente por pais" << endl;
            cout << "7. Agregar nueva venta" << endl;
            cout << "8. Eliminar ventas por pais o ciudad" << endl;
            cout << "9. Modificar venta por ID" << endl;
            cout << "0. Salir" << endl;
            cout << "Seleccione una opcion ";
            cin >> opcion;
            cout << endl;

            // Menú de opciones con llamadas a funciones correspondientes
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
                    cout << "Hasta luego!" << endl;
                    break;
                default:
                    cout << "Opcion invalida. Intente otra vez." << endl;
            }

        } while (opcion != 0); // Repite hasta que el usuario elija salir

    } catch (int e) {
        cout << "Se lanzo una excepcion con codigo: " << e << endl;
    }

    return 0;
}
