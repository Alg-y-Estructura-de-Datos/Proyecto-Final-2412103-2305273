#include <iostream>        // Para usar cout, cin, endl
#include <vector>          // Para usar vector<Venta> y almacenar las ventas
#include <string>          // Para trabajar con texto
#include <limits>          // Para obtener el mínimo y máximo valor numérico
#include <algorithm>       // Para ordenar estructuras con sort()
#include <map>             // Para agrupar ciudades por país temporalmente
#include "Venta.h"         // Incluye la estructura Venta definida por el usuario
#include "HashMapList.h"   // HashMap con listas enlazadas provisto por la cátedra

using namespace std;       // Evita el uso de std:: en cada línea

// Función que genera una clave combinada "Pais-Ciudad"
string clavePaisCiudad(const string& pais, const string& ciudad) {
    return pais + "-" + ciudad;
}

// Función que genera una clave combinada "Producto-Pais"
string claveProductoPais(const string& producto, const string& pais) {
    return producto + "-" + pais;
}

// Función que genera una clave combinada "Categoria-Pais"
string claveCategoriaPais(const string& categoria, const string& pais) {
    return categoria + "-" + pais;
}

// Función que devuelve directamente la categoría como clave
string claveCategoria(const string& categoria) {
    return categoria;
}

// Procesa las estadísticas iniciales cargando todos los datos en sus respectivos HashMapList
void procesarEstadisticas(
    const vector<Venta>& ventas,                             // Vector con todas las ventas
    HashMapList<string, double>& ventasPorCiudad,            // Monto total por ciudad y país
    HashMapList<string, double>& ventasPorProductoPais,      // Monto total por producto y país
    HashMapList<string, pair<double, int>>& acumuladorCategoriaPais, // Monto y cantidad por categoría
    HashMapList<string, int>& enviosPorPais,                 // Cantidad de envíos por país
    HashMapList<string, int>& enviosPorCategoria,            // Cantidad de envíos por categoría
    HashMapList<string, double>& ventasPorFecha,             // Monto total vendido por fecha
    HashMapList<string, int>& estadosEnvioPorPais,           // Estados de envío por país
    HashMapList<string, int>& cantidadVendidaPorProducto     // Cantidad total de productos vendidos
) {
    for (const Venta& v : ventas) { // Recorremos cada venta

        // Sumamos el monto total por ciudad
        string ciudadKey = clavePaisCiudad(v.pais, v.ciudad);
        ventasPorCiudad.put(ciudadKey, ventasPorCiudad.get(ciudadKey) + v.montoTotal);

        // Sumamos el monto por producto y país
        string prodPaisKey = claveProductoPais(v.producto, v.pais);
        ventasPorProductoPais.put(prodPaisKey, ventasPorProductoPais.get(prodPaisKey) + v.montoTotal);

        // Acumulamos monto y cantidad para calcular promedio de categoría
        string catPaisKey = claveCategoriaPais(v.categoria, v.pais);
        pair<double, int> actual = acumuladorCategoriaPais.get(catPaisKey);
        actual.first += v.montoTotal; // monto total acumulado
        actual.second += 1;           // cantidad de ventas
        acumuladorCategoriaPais.put(catPaisKey, actual); // actualizamos

        // Contamos el medio de envío más usado por país
        string envioPaisKey = clavePaisCiudad(v.pais, v.medioEnvio);
        enviosPorPais.put(envioPaisKey, enviosPorPais.get(envioPaisKey) + 1);

        // Contamos el medio de envío por categoría
        string envioCatKey = claveCategoria(v.categoria + "-" + v.medioEnvio);
        enviosPorCategoria.put(envioCatKey, enviosPorCategoria.get(envioCatKey) + 1);

        // Sumamos ventas por fecha
        ventasPorFecha.put(v.fecha, ventasPorFecha.get(v.fecha) + v.montoTotal);

        // Contamos estados de envío por país
        string estadoPaisKey = clavePaisCiudad(v.pais, v.estadoEnvio);
        estadosEnvioPorPais.put(estadoPaisKey, estadosEnvioPorPais.get(estadoPaisKey) + 1);

        // Sumamos unidades vendidas por producto
        cantidadVendidaPorProducto.put(v.producto, cantidadVendidaPorProducto.get(v.producto) + v.cantidad);
    }
}
// Muestra el producto con más y menos unidades vendidas
void mostrarProductoMasYMenosVendido(HashMapList<string, int>& cantidades) {
    int maxCant = numeric_limits<int>::min();   // Inicializamos el máximo con el valor más bajo posible
    int minCant = numeric_limits<int>::max();   // Inicializamos el mínimo con el valor más alto posible
    string productoMax, productoMin;            // Variables para guardar los nombres de los productos

    // Recorremos cada bucket del HashMapList
    for (int i = 0; i < cantidades.capacidad(); i++) {
        Lista<HashEntry<string, int>>* lista = cantidades.getBucket(i); // Obtenemos la lista de ese bucket
        for (int j = 0; j < lista->getTamanio(); j++) {
            HashEntry<string, int> entry = lista->getDato(j); // Accedemos a la entrada clave-valor

            // Verificamos si es el nuevo máximo
            if (entry.getValor() > maxCant) {
                maxCant = entry.getValor();
                productoMax = entry.getClave();
            }

            // Verificamos si es el nuevo mínimo
            if (entry.getValor() < minCant) {
                minCant = entry.getValor();
                productoMin = entry.getClave();
            }
        }
    }

    // Mostramos los resultados por pantalla
    cout << "Producto más vendido: " << productoMax << " (" << maxCant << " unidades)" << endl;
    cout << "Producto menos vendido: " << productoMin << " (" << minCant << " unidades)" << endl;
}

// Muestra el día con el mayor monto de ventas
void mostrarDiaConMayorVenta(HashMapList<string, double>& ventasPorFecha) {
    double maxVenta = -1;           // Inicializamos con un valor negativo
    string mejorDia;                // Guardará la fecha con más ventas

    // Recorremos los buckets del HashMapList
    for (int i = 0; i < ventasPorFecha.capacidad(); i++) {
        Lista<HashEntry<string, double>>* lista = ventasPorFecha.getBucket(i); // Lista del bucket
        for (int j = 0; j < lista->getTamanio(); j++) {
            HashEntry<string, double> entry = lista->getDato(j); // Obtenemos clave-valor

            // Comparamos y actualizamos si encontramos un monto mayor
            if (entry.getValor() > maxVenta) {
                maxVenta = entry.getValor();
                mejorDia = entry.getClave();
            }
        }
    }

    // Mostramos el día con mayor venta
    cout << "Día con mayor monto de ventas: " << mejorDia << " ($" << maxVenta << ")" << endl;
}

// Muestra los promedios de ventas por categoría y país
void mostrarPromedioVentasPorCategoriaYPais(HashMapList<string, pair<double, int>>& acumulador) {
    cout << "Promedios de ventas por categoría y país:" << endl;

    // Recorremos el HashMapList
    for (int i = 0; i < acumulador.capacidad(); i++) {
        Lista<HashEntry<string, pair<double, int>>>* lista = acumulador.getBucket(i); // Obtenemos lista del bucket

        for (int j = 0; j < lista->getTamanio(); j++) {
            HashEntry<string, pair<double, int>> entry = lista->getDato(j); // Clave y par (suma, cantidad)
            double total = entry.getValor().first;
            int cantidad = entry.getValor().second;

            if (cantidad > 0) {
                double promedio = total / cantidad; // Calculamos promedio
                cout << " - " << entry.getClave() << ": $" << promedio << endl;
            }
        }
    }
}

// Muestra las 5 ciudades con mayor monto de ventas por país
void mostrarTop5CiudadesPorPais(HashMapList<string, double>& ventasPorCiudad) {
    map<string, vector<pair<string, double>>> porPais; // Agrupa ciudades por país

    // Recorremos el HashMapList
    for (int i = 0; i < ventasPorCiudad.capacidad(); i++) {
        Lista<HashEntry<string, double>>* lista = ventasPorCiudad.getBucket(i);

        for (int j = 0; j < lista->getTamanio(); j++) {
            HashEntry<string, double> entry = lista->getDato(j);
            string clave = entry.getClave(); // Ej: "Argentina-Córdoba"
            double monto = entry.getValor();

            size_t guion = clave.find('-');                      // Buscamos el guión separador
            string pais = clave.substr(0, guion);                // Extraemos el país
            string ciudad = clave.substr(guion + 1);             // Extraemos la ciudad

            porPais[pais].push_back(make_pair(ciudad, monto));   // Guardamos la ciudad y su monto
        }
    }

    // Mostramos el top 5 de cada país
    for (auto& par : porPais) {
        string pais = par.first;
        vector<pair<string, double>>& ciudades = par.second;

        // Ordenamos de mayor a menor por monto
        sort(ciudades.begin(), ciudades.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });

        cout << "Top 5 ciudades de " << pais << ":" << endl;

        // Mostramos máximo 5 ciudades
        for (int i = 0; i < min(5, (int)ciudades.size()); i++) {
            cout << " - " << ciudades[i].first << ": $" << ciudades[i].second << endl;
        }
    }
}

// Muestra el medio de envío más utilizado por cada país
void mostrarMedioEnvioMasUsadoPorPais(HashMapList<string, int>& enviosPorPais) {
    map<string, pair<string, int>> maximos; // país → (envío, cantidad)

    for (int i = 0; i < enviosPorPais.capacidad(); i++) {
        Lista<HashEntry<string, int>>* lista = enviosPorPais.getBucket(i);

        for (int j = 0; j < lista->getTamanio(); j++) {
            HashEntry<string, int> entry = lista->getDato(j);
            string clave = entry.getClave();     // Ej: "Argentina-Estándar"
            int cantidad = entry.getValor();

            size_t guion = clave.find('-');
            string pais = clave.substr(0, guion);     // Extraemos país
            string envio = clave.substr(guion + 1);   // Extraemos tipo de envío

            if (cantidad > maximos[pais].second) {
                maximos[pais] = make_pair(envio, cantidad); // Guardamos el más usado
            }
        }
    }

    // Mostramos los resultados
    cout << "Medio de envío más usado por país:" << endl;
    for (auto& par : maximos) {
        cout << " - " << par.first << ": " << par.second.first << " (" << par.second.second << " envíos)" << endl;
    }
}

// Muestra el estado de envío más frecuente por país
void mostrarEstadoEnvioMasFrecuentePorPais(HashMapList<string, int>& estadosPorPais) {
    map<string, pair<string, int>> maximos; // país → (estado, cantidad)

    for (int i = 0; i < estadosPorPais.capacidad(); i++) {
        Lista<HashEntry<string, int>>* lista = estadosPorPais.getBucket(i);

        for (int j = 0; j < lista->getTamanio(); j++) {
            HashEntry<string, int> entry = lista->getDato(j);
            string clave = entry.getClave();    // Ej: "Chile-Entregado"
            int cantidad = entry.getValor();

            size_t guion = clave.find('-');
            string pais = clave.substr(0, guion);     // País
            string estado = clave.substr(guion + 1);  // Estado

            if (cantidad > maximos[pais].second) {
                maximos[pais] = make_pair(estado, cantidad); // Guardamos el estado más frecuente
            }
        }
    }

    // Mostramos resultado por país
    cout << "Estado de envío más frecuente por país:" << endl;
    for (auto& par : maximos) {
        cout << " - " << par.first << ": " << par.second.first << " (" << par.second.second << " veces)" << endl;
    }
}
