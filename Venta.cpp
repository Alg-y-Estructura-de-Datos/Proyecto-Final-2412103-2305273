#include "Venta.h"  // Incluimos la definición de Venta

// Función que recibe una venta y arma un string con sus datos principales
string ventaToString(const Venta& v) {
    return "ID: " + to_string(v.id) +                       // Convierte el ID a string
           " | Fecha: " + v.fecha +                         // Agrega la fecha
           " | Producto: " + v.producto +                   // Nombre del producto
           " x" + to_string(v.cantidad) +                   // Cantidad vendida
           " ($" + to_string(v.precioUnitario) + ")"        // Precio por unidad
           " = $" + to_string(v.montoTotal) +               // Total de la venta
           " | Cliente: " + v.cliente +                     // Nombre del cliente
           " | Ciudad: " + v.ciudad + ", " + v.pais +       // Ciudad y país
           " | Envío: " + v.medioEnvio +                    // Medio de envío
           " [" + v.estadoEnvio + "]";                      // Estado del envío
}

// Esta función compara dos ventas por su ID
// Devuelve true si la primera tiene menor ID que la segunda
bool ventaMenorPorID(const Venta& a, const Venta& b) {
    return a.id < b.id;
}
