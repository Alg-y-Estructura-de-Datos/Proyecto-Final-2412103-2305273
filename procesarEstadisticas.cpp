// TOTAL en procesarEstadisticas.cpp:
// IF explícitos: 6
// IF de estructuras complejas (HashMap, AVL, Lista, etc): 16
// TOTAL ESTIMADO DE IFs: 22

// Archivo: procesarEstadisticas.cpp
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.

#include <iostream>
#include <vector>
#include <string>
#include <limits> //Incluye utilidades relacionadas con los límites de los tipos de datos numéricos, como std::numeric_limits<int>::max(), que da el valor máximo posible para un int.
#include <algorithm>//Incluye funciones algorítmicas estándar, como sort(), max_element(), find(), etc. Se usan para manipular contenedores (como vectores).
#include <map>// Incluye la clase std::map, que es una estructura de datos tipo diccionario (clave-valor) ordenada automáticamente por clave.
#include "Venta.h"
#include "HashMapList.h"

using namespace std;

// ================= FUNCIONES AUXILIARES =================

// Esta función recibe dos cadenas por referencia constante: pais y ciudad.
string clavePaisCiudad(const string& pais, const string& ciudad) {
    return pais + "-" + ciudad; 
}

// Similar a la anterior, esta función crea una clave única a partir del nombre de un producto y un país
string claveProductoPais(const string& producto, const string& pais) {
    return producto + "-" + pais;
}

// Normaliza categoría-pais eliminando espacios y creando una clave única
string claveCategoriaPais(const string& categoria, const string& pais) {
    string clave = categoria + "-" + pais;
    clave.erase(remove(clave.begin(), clave.end(), ' '), clave.end()); //Usa el patrón erase-remove para borrar todos los ' ' (espacios) del string clave.
    return clave;
}

// Devuelve solo la categoría como clave simple
string claveCategoria(const string& categoria) {
    return categoria;
}

// Función segura para obtener claves que aún no existen
template <typename K, typename T>
T getSeguro(HashMapList<K, T>& mapa, K clave, T valorPorDefecto) {
    try {
        return mapa.get(clave);
    } catch (...) {
        return valorPorDefecto;
    }
}

// ================= PROCESAMIENTO PRINCIPAL =================

// Procesamiento principal de estadísticas, creamos hashmaps
void procesarEstadisticas(
    const vector<Venta>& ventas,
    HashMapList<string, double>& ventasPorCiudad,
    HashMapList<string, double>& ventasPorProductoPais,
    HashMapList<string, pair<double, int>>& acumuladorCategoriaPais,
    HashMapList<string, int>& enviosPorPais,
    HashMapList<string, int>& enviosPorCategoria,
    HashMapList<string, double>& ventasPorFecha,
    HashMapList<string, int>& estadosEnvioPorPais,
    HashMapList<string, int>& cantidadVendidaPorProducto
) {
    for (const Venta& v : ventas) { // Creamos claves para cada ""CATEGORIA"" que nos va a servir para mas adelante
        string ciudadKey = clavePaisCiudad(v.pais, v.ciudad);
        string prodPaisKey = claveProductoPais(v.producto, v.pais);
        string catPaisKey = claveCategoriaPais(v.categoria, v.pais);
        string envioPaisKey = clavePaisCiudad(v.pais, v.medioEnvio);
        string envioCatKey = v.categoria + "-" + v.medioEnvio;
        string estadoPaisKey = clavePaisCiudad(v.pais, v.estadoEnvio);

        ventasPorCiudad.put(ciudadKey, getSeguro(ventasPorCiudad, ciudadKey, 0.0) + v.montoTotal);//Acumulación de estadísticas
        ventasPorProductoPais.put(prodPaisKey, getSeguro(ventasPorProductoPais, prodPaisKey, 0.0) + v.montoTotal);

        pair<double, int> actual = getSeguro(acumuladorCategoriaPais, catPaisKey, {0.0, 0}); 
        actual.first += v.montoTotal;
        actual.second += 1;
        acumuladorCategoriaPais.put(catPaisKey, actual);

        enviosPorPais.put(envioPaisKey, getSeguro(enviosPorPais, envioPaisKey, 0) + 1);
        enviosPorCategoria.put(envioCatKey, getSeguro(enviosPorCategoria, envioCatKey, 0) + 1);
        ventasPorFecha.put(v.fecha, getSeguro(ventasPorFecha, v.fecha, 0.0) + v.montoTotal);
        estadosEnvioPorPais.put(estadoPaisKey, getSeguro(estadosEnvioPorPais, estadoPaisKey, 0) + 1);
        cantidadVendidaPorProducto.put(v.producto, getSeguro(cantidadVendidaPorProducto, v.producto, 0) + v.cantidad);
    }
}

// ================= FUNCIONES DE ANÁLISIS Y REPORTE =================

// Producto más y menos vendido
void mostrarProductoMasYMenosVendido(HashMapList<string, int>& cantidades) {
    int maxCant = numeric_limits<int>::min(); // Inicializa al valor más bajo
    int minCant = numeric_limits<int>::max(); // Inicializa al valor más alto
    string productoMax, productoMin;

    // Recorre todos los buckets del hashmap
    for (int i = 0; i < cantidades.getCapacidad(); i++) {
        auto lista = cantidades.getBucket(i);
        for (int j = 0; j < lista->getTamanio(); j++) {
            auto entry = lista->getDato(j);
            int valor = entry.getValor();
            if (valor > maxCant) {
                maxCant = valor;
                productoMax = entry.getClave();
            }
            if (valor < minCant) {
                minCant = valor;
                productoMin = entry.getClave();
            }
        }
    }

    cout << "Producto mas vendido: " << productoMax << " (" << maxCant << " unidades)" << endl;
    cout << "Producto menos vendido: " << productoMin << " (" << minCant << " unidades)" << endl;
}

// Día con mayor monto vendido
void mostrarDiaConMayorVenta(HashMapList<string, double>& ventasPorFecha) {
    double maxVenta = -1;
    string mejorDia;

    // Busca el día con el monto más alto
    for (int i = 0; i < ventasPorFecha.getCapacidad(); i++) {
        auto lista = ventasPorFecha.getBucket(i);
        for (int j = 0; j < lista->getTamanio(); j++) {
            auto entry = lista->getDato(j);
            if (entry.getValor() > maxVenta) {
                maxVenta = entry.getValor();
                mejorDia = entry.getClave();
            }
        }
    }

    cout << "Dia con mayor monto de ventas: " << mejorDia << " ($" << maxVenta << ")" << endl;
}

// Promedios por categoría y país
void mostrarPromedioVentasPorCategoriaYPais(HashMapList<string, pair<double, int>>& acumulador) {
    map<string, pair<double, int>> promedios; // mapa auxiliar

    // Recorre todo el hash y acumula montos y cantidades
    for (int i = 0; i < acumulador.getCapacidad(); i++) {
        auto lista = acumulador.getBucket(i);
        for (int j = 0; j < lista->getTamanio(); j++) {
            auto entry = lista->getDato(j);
            string clave = entry.getClave();
            double total = entry.getValor().first;
            int cantidad = entry.getValor().second;
            if (cantidad > 0) {
                promedios[clave].first += total;
                promedios[clave].second += cantidad;
            }
        }
    }

    // Muestra una línea por cada categoría-país
    cout << "Promedios de ventas por categoria y pais:" << endl;
    for (const auto& par : promedios) {
        string clave = par.first;
        double total = par.second.first;
        int cantidad = par.second.second;

        size_t separador = clave.find('-'); // separa categoría y país
        string categoria = clave.substr(0, separador);
        string pais = clave.substr(separador + 1);

        double promedio = total / cantidad;
        cout << " - " << categoria << " en " << pais << ": $" << promedio << endl;
    }
}

// Top 5 ciudades por país
void mostrarTop5CiudadesPorPais(HashMapList<string, double>& ventasPorCiudad) {
    map<string, vector<pair<string, double>>> porPais;

    // Separa ciudad y país, y agrupa ventas
    for (int i = 0; i < ventasPorCiudad.getCapacidad(); i++) {
        auto lista = ventasPorCiudad.getBucket(i);
        for (int j = 0; j < lista->getTamanio(); j++) {
            auto entry = lista->getDato(j);
            string clave = entry.getClave();
            double monto = entry.getValor();
            size_t guion = clave.find('-');
            string pais = clave.substr(0, guion);
            string ciudad = clave.substr(guion + 1);
            porPais[pais].push_back({ ciudad, monto });
        }
    }

    // Ordena y muestra las 5 ciudades con más ventas por país
    for (auto& par : porPais) {
        cout << "Top 5 ciudades de " << par.first << ":" << endl;
        auto& ciudades = par.second;
        sort(ciudades.begin(), ciudades.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });

        for (int i = 0; i < min(5, (int)ciudades.size()); i++) {
            cout << " - " << ciudades[i].first << ": $" << ciudades[i].second << endl;
        }
    }
}

// Medio de envío más usado por país
void mostrarMedioEnvioMasUsadoPorPais(HashMapList<string, int>& enviosPorPais) {
    map<string, pair<string, int>> maximos;

    // Encuentra el medio de envío más frecuente por país
    for (int i = 0; i < enviosPorPais.getCapacidad(); i++) {
        auto lista = enviosPorPais.getBucket(i);
        for (int j = 0; j < lista->getTamanio(); j++) {
            auto entry = lista->getDato(j);
            string clave = entry.getClave();
            int cantidad = entry.getValor();
            size_t guion = clave.find('-');
            string pais = clave.substr(0, guion);
            string envio = clave.substr(guion + 1);
            if (cantidad > maximos[pais].second) {
                maximos[pais] = { envio, cantidad };
            }
        }
    }

    cout << "Medio de envio mas usado por pais:" << endl;
    for (auto& par : maximos) {
        cout << " - " << par.first << ": " << par.second.first << " (" << par.second.second << " envios)" << endl;
    }
}

// Estado de envío más frecuente por país
void mostrarEstadoEnvioMasFrecuentePorPais(HashMapList<string, int>& estadosPorPais) {
    map<string, pair<string, int>> maximos;

    // Encuentra el estado más frecuente por país (ej: "Entregado", "Pendiente")
    for (int i = 0; i < estadosPorPais.getCapacidad(); i++) {
        auto lista = estadosPorPais.getBucket(i);
        for (int j = 0; j < lista->getTamanio(); j++) {
            auto entry = lista->getDato(j);
            string clave = entry.getClave();
            int cantidad = entry.getValor();
            size_t guion = clave.find('-');
            string pais = clave.substr(0, guion);
            string estado = clave.substr(guion + 1);
            if (cantidad > maximos[pais].second) {
                maximos[pais] = { estado, cantidad };
            }
        }
    }

    cout << "Estado de envio más frecuente por pais:" << endl;
    for (auto& par : maximos) {
        cout << " - " << par.first << ": " << par.second.first << " (" << par.second.second << " veces)" << endl;
    }
}
