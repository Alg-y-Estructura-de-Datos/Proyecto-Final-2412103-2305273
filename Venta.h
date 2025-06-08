// Archivo: Venta.h
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.

#ifndef VENTA_H  // Si no se ha definido VENTA_H, entonces...
#define VENTA_H  // ...lo definimos. Esto evita incluir el archivo dos veces.

#include <string>  // Incluimos la librería para usar el tipo string

using namespace std;  // Nos permite usar string sin tener que escribir std::string

// Definimos un struct llamado Venta que representa un registro de venta
struct Venta {
    int id;                 // ID de la venta (número único)
    string fecha;           // Fecha de la venta (ej: "2025-03-12")
    string pais;            // País donde se hizo la venta
    string ciudad;          // Ciudad donde se hizo la venta
    string cliente;         // Nombre del cliente que compró
    string producto;        // Nombre del producto vendido
    string categoria;       // Categoría del producto (ej: "Electrónica")
    int cantidad;           // Cantidad de unidades vendidas
    double precioUnitario;  // Precio por unidad del producto
    double montoTotal;      // Monto total de la venta (cantidad * precio)
    string medioEnvio;      // Medio por el que se envió el producto
    string estadoEnvio;     // Estado del envío (ej: "Entregado", "Pendiente")
};

// Esta función devuelve un texto con los datos de una venta
string ventaToString(const Venta& v);

// Esta función sirve para comparar dos ventas por su ID (útil para ordenar)
bool ventaMenorPorID(const Venta& a, const Venta& b);

#endif  // Fin del bloque de inclusión
