// TOTAL en HashMapList.h:
// IF explícitos: 6
// IF de estructuras complejas (HashMap, AVL, Lista, etc): 32
// TOTAL ESTIMADO DE IFs: 38

// Archivo: HashMapList.h
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.

#ifndef U05_HASH_HASHMAP_HASHMAPLIST_H_
#define U05_HASH_HASHMAP_HASHMAPLIST_H_

#include <iostream>
#include "HashEntry.h"  // Entrada clave-valor
#include "Lista.h"       // Lista enlazada usada como contenedor interno
using namespace std;

// ======================= CLASE HASHMAPLIST =======================

template <class K, class T>
class HashMapList {
private:
    Lista<HashEntry<K, T>>** tabla;          // Tabla hash con listas por casilla
    unsigned int tamanio;                    // Cantidad total de buckets
    static unsigned int hashFunc(K clave);   // Función hash por defecto
    unsigned int (*hashFuncP)(K clave);      // Puntero a función hash (puede personalizarse)

public:
    explicit HashMapList(unsigned int k);                                // Constructor con función hash por defecto
    HashMapList(unsigned int k, unsigned int (*hashFuncP)(K clave));     // Constructor con función hash personalizada
    ~HashMapList();                                                      // Destructor

    void put(K clave, T valor);       // Inserta clave-valor
    T get(K clave);                   // Devuelve valor según clave
    void remove(K clave);            // Elimina elemento por clave
    bool esVacio();                  // Verifica si está vacío
    void print();                    // Imprime el contenido del mapa
    void getList(K clave);           // Imprime valores del bucket de una clave

    void vaciar();                   // Borra toda la tabla
    Nodo<HashEntry<K, T>>* getInicio(int pos);  // Devuelve inicio de la lista en una posición
    Lista<HashEntry<K, T>>* getBucket(int i) const; // Devuelve lista en posición i
    unsigned int getCapacidad() const;            // Devuelve tamaño total de la tabla
};

// ======================= CONSTRUCTORES =======================

// Constructor por defecto con función hash propia
template <class K, class T>
HashMapList<K, T>::HashMapList(unsigned int k) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>>*[tamanio];
    for (int i = 0; i < tamanio; i++) {
        tabla[i] = new Lista<HashEntry<K, T>>();
    }
    hashFuncP = hashFunc;
}

// Constructor con función hash personalizada
template <class K, class T>
HashMapList<K, T>::HashMapList(unsigned int k, unsigned int (*fp)(K)) {
    tamanio = k;
    tabla = new Lista<HashEntry<K, T>>*[tamanio];
    for (int i = 0; i < tamanio; i++) {
        tabla[i] = new Lista<HashEntry<K, T>>();
    }
    hashFuncP = fp;
}

// ======================= DESTRUCTOR =======================

template <class K, class T>
HashMapList<K, T>::~HashMapList() {
    for (int i = 0; i < tamanio; i++) {
        delete tabla[i];
    }
    delete[] tabla;
}

// ======================= OPERACIONES BÁSICAS =======================

// Inserta un nuevo valor al final del bucket correspondiente
template <class K, class T>
void HashMapList<K, T>::put(K clave, T valor) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    tabla[pos]->insertarUltimo(HashEntry<K, T>(clave, valor));
}

// Busca un valor por clave, lanza excepción si no existe
template <class K, class T>
T HashMapList<K, T>::get(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    if (tabla[pos] == nullptr) throw 404;
    Nodo<HashEntry<K, T>>* nodo = tabla[pos]->getInicio();
    while (nodo != nullptr) {
        if (nodo->getDato().getClave() == clave) {
            return nodo->getDato().getValor();
        }
        nodo = nodo->getSiguiente();
    }
    throw 404;
}

// Elimina una entrada por clave, si no la encuentra lanza excepción
template <class K, class T>
void HashMapList<K, T>::remove(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    // se utilizan 2 cantidad de if
    if (tabla[pos] != nullptr) {
        Nodo<HashEntry<K, T>>* actual = tabla[pos]->getInicio();
        int index = 0;
        while (actual != nullptr) {
            if (actual->getDato().getClave() == clave) {
                tabla[pos]->remover(index);
                return;
            }
            actual = actual->getSiguiente();
            index++;
        }
        throw 404;
    } else {
        throw 409;
    }
}

// Verifica si el hashmap está completamente vacío
template <class K, class T>
bool HashMapList<K, T>::esVacio() {
    for (int i = 0; i < tamanio; i++) {
        // se utilizan 1 cantidad de if
        if (!tabla[i]->esVacia()) return false;
    }
    return true;
}

// ======================= OPERACIONES EXTRA =======================

// Imprime los valores en el bucket correspondiente a una clave
template <class K, class T>
void HashMapList<K, T>::getList(K clave) {
    unsigned int pos = hashFuncP(clave) % tamanio;
    Nodo<HashEntry<K, T>>* aux = tabla[pos]->getInicio();
    while (aux != nullptr) {
        cout << aux->getDato().getValor() << endl;
        aux = aux->getSiguiente();
    }
}

// Imprime toda la tabla hash con sus buckets
template <class K, class T>
void HashMapList<K, T>::print() {
    for (int i = 0; i < tamanio; i++) {
        cout << "Índice " << i << ": ";
        Nodo<HashEntry<K, T>>* aux = tabla[i]->getInicio();
        while (aux != nullptr) {
            cout << "(" << aux->getDato().getClave() << ", " << aux->getDato().getValor() << ") ";
            aux = aux->getSiguiente();
        }
        cout << std::endl;
    }
}

// ======================= ACCESO A LA ESTRUCTURA INTERNA =======================

template <class K, class T>
unsigned int HashMapList<K, T>::getCapacidad() const {
    return tamanio;
}

template <class K, class T>
Lista<HashEntry<K, T>>* HashMapList<K, T>::getBucket(int i) const {
    return tabla[i];
}

template <class K, class T>
Nodo<HashEntry<K, T>>* HashMapList<K, T>::getInicio(int pos) {
    return tabla[pos]->getInicio();
}

// Borra todos los elementos del mapa, reinicia listas
template <class K, class T>
void HashMapList<K, T>::vaciar() {
    for (unsigned int i = 0; i < tamanio; i++) {
        // se utilizan 1 cantidad de if
        if (tabla[i] != nullptr) {
            tabla[i]->vaciar();
            delete tabla[i];
            tabla[i] = new Lista<HashEntry<K, T>>();
        }
    }
}

// ======================= FUNCIÓN HASH POR DEFECTO =======================

// Genera un hash para claves tipo string usando multiplicación polinómica
template <class K, class T>
unsigned int HashMapList<K, T>::hashFunc(K clave) {
    unsigned int hash = 0;
    for (char c : clave) {
        hash = hash * 31 + c;
    }
    return hash;
}

#endif // U05_HASH_HASHMAP_HASHMAPLIST_H_
