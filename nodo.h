// TOTAL en nodo.h:
// IF de estructuras complejas (HashMap, AVL, Lista, etc): 5
// TOTAL ESTIMADO DE IFs: 5

// Archivo: nodo.h
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.

#ifndef NODO_H
#define NODO_H

template<class T>
class Nodo {
private:
    T dato;
    Nodo<T> *siguiente;

public:
    Nodo(T d) : dato(d), siguiente(nullptr) {} // Constructor que evita error por falta de constructor por defecto

    T getDato() { return dato; }
    void setDato(T d) { dato = d; }

    Nodo<T> *getSiguiente() { return siguiente; }
    void setSiguiente(Nodo<T> *sig) { siguiente = sig; }
};

#endif // NODO_H
