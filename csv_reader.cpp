#include <vector>      // Para usar vectores dinámicos (vector<Venta>)
#include <string>      // Para usar strings (texto)
#include <fstream>     // Para abrir y leer archivos
#include <sstream>     // Para separar texto por partes (split por coma)
#include <iostream>    // Para mostrar mensajes por consola
#include "Venta.h"     // Incluye la definición del struct Venta

using namespace std;   // Nos permite escribir vector, string, etc. sin std::

// Función que lee el archivo CSV y devuelve un vector con todas las ventas cargadas
vector<Venta> leerVentasDesdeCSV(const string& nombreArchivo) {
    vector<Venta> ventas;               // Creamos un vector vacío para guardar las ventas
    ifstream archivo(nombreArchivo);    // Abrimos el archivo con el nombre recibido

    // Si el archivo no se puede abrir, mostramos un mensaje de error
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
        return ventas;  // Devolvemos un vector vacío
    }

    string linea;
    getline(archivo, linea);  // Leemos la primera línea (los títulos de columna) y la descartamos

    // Leemos línea por línea hasta llegar al final del archivo
    while (getline(archivo, linea)) {
        stringstream ss(linea);     // Creamos un stream con la línea completa
        string campo;               // Para guardar cada valor separado por coma
        vector<string> campos;      // Vector temporal para guardar los 12 campos

        // Separamos la línea por comas y guardamos cada parte en el vector campos
        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }

        // Verificamos que la línea tenga exactamente 12 campos
        if (campos.size() != 12) {
            cerr << "Línea inválida con " << campos.size() << " campos" << endl;
            continue;  // Saltamos esta línea si está incompleta o mal
        }

        // Creamos un objeto Venta y cargamos sus atributos desde los campos del CSV
        Venta v;
        v.id = stoi(campos[0]);              // Convertimos el ID a entero
        v.fecha = campos[1];                 // Fecha en formato texto
        v.pais = campos[2];                  // País
        v.ciudad = campos[3];                // Ciudad
        v.cliente = campos[4];               // Nombre del cliente
        v.producto = campos[5];              // Producto vendido
        v.categoria = campos[6];             // Categoría del producto
        v.cantidad = stoi(campos[7]);        // Cantidad vendida (entero)
        v.precioUnitario = stod(campos[8]);  // Precio por unidad (double)
        v.montoTotal = stod(campos[9]);      // Monto total (double)
        v.medioEnvio = campos[10];           // Medio de envío
        v.estadoEnvio = campos[11];          // Estado del envío

        ventas.push_back(v);  // Agregamos la venta al vector
    }

    archivo.close();  // Cerramos el archivo cuando terminamos
    return ventas;    // Devolvemos el vector con todas las ventas cargadas
}

