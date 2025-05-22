#include <iostream>      // Para imprimir por pantalla (cout)
#include <vector>        // Para usar vector<Venta>
#include <string>        // Para trabajar con texto (string)
#include <limits>        // Para obtener el valor mínimo y máximo de un tipo

#include "Venta.h"       // Archivo donde definimos el struct Venta
#include "HashMapList.h" // Estructura hash con listas, provista por tus profes

using namespace std;     // Para no escribir std:: en cada línea

// Función auxiliar para crear una clave del tipo "Perú-Lima"
string clavePaisCiudad(const string& pais, const string& ciudad) {
    return pais + "-" + ciudad;
}

// Crea una clave como "Laptop-Argentina" para identificar ventas por producto y país
string claveProductoPais(const string& producto, const string& pais) {
    return producto + "-" + pais;
}

// Crea una clave como "Electrónica-Argentina" para categoría y país
string claveCategoriaPais(const string& categoria, const string& pais) {
    return categoria + "-" + pais;
}

// Devuelve directamente la categoría (o categoría + envío si lo usás así)
string claveCategoria(const string& categoria) {
    return categoria;
}

// Esta función procesa TODAS las estadísticas iniciales al recorrer el vector de ventas
void procesarEstadisticas(
    const vector<Venta>& ventas,                          // Vector con todas las ventas cargadas
    HashMapList<string, double>& ventasPorCiudad,         // Monto total vendido por ciudad-pais
    HashMapList<string, double>& ventasPorProductoPais,   // Monto total vendido por producto-pais
    HashMapList<string, pair<double, int>>& acumuladorCategoriaPais, // Para calcular promedios por categoría
    HashMapList<string, int>& enviosPorPais,              // Contador de medios de envío por país
    HashMapList<string, int>& enviosPorCategoria,         // Contador de medios de envío por categoría
    HashMapList<string, double>& ventasPorFecha,          // Monto total por día
    HashMapList<string, int>& estadosEnvioPorPais,        // Estado más frecuente de envío por país
    HashMapList<string, int>& cantidadVendidaPorProducto  // Unidades vendidas por producto
) {
    // Recorremos cada venta y actualizamos todas las estructuras
    for (const Venta& v : ventas) {
        // Sumamos monto total vendido por ciudad y país
        string ciudadKey = clavePaisCiudad(v.pais, v.ciudad);
        ventasPorCiudad.put(ciudadKey, ventasPorCiudad.get(ciudadKey) + v.montoTotal);

        // Sumamos monto total vendido por producto y país
        string prodPaisKey = claveProductoPais(v.producto, v.pais);
        ventasPorProductoPais.put(prodPaisKey, ventasPorProductoPais.get(prodPaisKey) + v.montoTotal);

        // Acumulamos suma de montos y cantidad para luego calcular promedio por categoría y país
        string catPaisKey = claveCategoriaPais(v.categoria, v.pais);
        pair<double, int> actual = acumuladorCategoriaPais.get(catPaisKey);
        actual.first += v.montoTotal; // sumatoria de montos
        actual.second += 1;           // cantidad de ventas
        acumuladorCategoriaPais.put(catPaisKey, actual);

        // Contamos cuántas veces se usó cada medio de envío por país
        string envioPaisKey = clavePaisCiudad(v.pais, v.medioEnvio);
        enviosPorPais.put(envioPaisKey, enviosPorPais.get(envioPaisKey) + 1);

        // Contamos medios de envío por categoría (por ejemplo, "Electrónica-Estándar")
        string envioCatKey = claveCategoria(v.categoria + "-" + v.medioEnvio);
        enviosPorCategoria.put(envioCatKey, enviosPorCategoria.get(envioCatKey) + 1);

        // Sumamos monto total vendido por fecha
        ventasPorFecha.put(v.fecha, ventasPorFecha.get(v.fecha) + v.montoTotal);

        // Contamos estados de envío (ej: Entregado, Pendiente...) por país
        string estadoPaisKey = clavePaisCiudad(v.pais, v.estadoEnvio);
        estadosEnvioPorPais.put(estadoPaisKey, estadosEnvioPorPais.get(estadoPaisKey) + 1);

        // Sumamos unidades vendidas por producto (para saber cuál se vendió más y menos)
        cantidadVendidaPorProducto.put(v.producto, cantidadVendidaPorProducto.get(v.producto) + v.cantidad);
    }
}

// Esta función muestra cuál fue el producto más vendido y cuál el menos vendido (por cantidad de unidades)
void mostrarProductoMasYMenosVendido(HashMapList<string, int>& cantidades) {
    int maxCant = numeric_limits<int>::min();  // Empezamos con el valor más bajo posible
    int minCant = numeric_limits<int>::max();  // Empezamos con el valor más alto posible
    string productoMax, productoMin;

    // Recorremos todos los buckets de la tabla hash
    for (int i = 0; i < cantidades.capacidad(); i++) {
        Lista<HashEntry<string, int>>* lista = cantidades.getBucket(i);  // Lista de colisiones en esa posición
        for (int j = 0; j < lista->getTamanio(); j++) {
            HashEntry<string, int> entry = lista->getDato(j);  // Obtenemos cada clave y su valor
            if (entry.getValor() > maxCant) {                  // Si encontramos una cantidad mayor...
                maxCant = entry.getValor();
                productoMax = entry.getClave();
            }
            if (entry.getValor() < minCant) {                  // Si encontramos una cantidad menor...
                minCant = entry.getValor();
                productoMin = entry.getClave();
            }
        }
    }

    // Mostramos por pantalla los resultados
    cout << "Producto más vendido (unidades): " << productoMax << " (" << maxCant << " unidades)" << endl;
    cout << "Producto menos vendido (unidades): " << productoMin << " (" << minCant << " unidades)" << endl;
}
