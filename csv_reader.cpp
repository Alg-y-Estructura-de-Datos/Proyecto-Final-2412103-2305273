// TOTAL en csv_reader.cpp:
// IF explícitos: 2
// IF de estructuras complejas (HashMap, AVL, Lista, etc): 0
// TOTAL ESTIMADO DE IFs: 2

// Archivo: csv_reader.cpp
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.

#include <fstream>     // Para manejo de archivos
#include <sstream>     // Para procesar líneas y separar campos
#include <vector>      // Para almacenar las ventas
#include <string>      // Para manipular texto
#include "Venta.h"     // Estructura de datos que representa una venta

using namespace std;

// ======================= LIMPIEZA DE ESPACIOS =======================

// Función para limpiar espacios en blanco al inicio y final de un string
string limpiarEspacios(const string& str) {
    size_t inicio = str.find_first_not_of(" \t\r\n"); // Encuentra primer carácter no espacio
    size_t fin = str.find_last_not_of(" \t\r\n");     // Encuentra último carácter no espacio
    return (inicio == string::npos) ? "" : str.substr(inicio, fin - inicio + 1); // Si es todo espacio, devuelve vacío
}

// ======================= LECTURA DE ARCHIVO CSV =======================

// Función para leer el CSV
vector<Venta> leerVentasDesdeCSV(const string& nombreArchivo) {
    vector<Venta> ventas;              // Vector que almacenará las ventas leídas
    ifstream archivo(nombreArchivo);   // Abre archivo CSV en modo lectura

    if (!archivo.is_open()) {
        throw -1; // Error al abrir archivo
    }

    string linea;
    getline(archivo, linea); // Saltar encabezado (primera línea del CSV)

    // Leer cada línea restante del archivo
    while (getline(archivo, linea)) {
        stringstream ss(linea);  // Convierte línea a flujo de texto
        string campo;
        vector<string> campos;   // Vector para almacenar los valores de cada campo

        // Separar cada campo usando ',' como delimitador
        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }

        if (campos.size() < 12) continue; // Asegurarse que haya suficientes campos (evita errores)

        // Crear una venta a partir de los campos procesados
        Venta v;
        v.id = stoi(campos[0]);
        v.fecha = limpiarEspacios(campos[1]);
        v.pais = limpiarEspacios(campos[2]);
        v.ciudad = limpiarEspacios(campos[3]);
        v.cliente = limpiarEspacios(campos[4]);
        v.producto = limpiarEspacios(campos[5]);
        v.categoria = limpiarEspacios(campos[6]);
        v.cantidad = stoi(campos[7]);
        v.precioUnitario = stod(campos[8]);
        v.montoTotal = stod(campos[9]);
        v.medioEnvio = limpiarEspacios(campos[10]);
        v.estadoEnvio = limpiarEspacios(campos[11]);

        ventas.push_back(v); // Agrega la venta al vector
    }

    archivo.close(); // Cierra el archivo al finalizar
    return ventas;   // Devuelve todas las ventas leídas
}
