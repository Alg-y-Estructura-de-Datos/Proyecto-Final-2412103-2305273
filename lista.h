// TOTAL en Lista.h:
// IF explícitos: 7
// IF de estructuras complejas (HashMap, AVL, Lista, etc): 31
// TOTAL ESTIMADO DE IFs: 38

// Archivo: Lista.h
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.


#ifndef U02_LISTAS_LISTA_LISTA_H_
#define U02_LISTAS_LISTA_LISTA_H_
#include "nodo.h"
#include <iostream>

template <class T> class Lista {
private:
  Nodo<T> *inicio;

public:
  Lista();
  Lista(const Lista<T> &li);
  ~Lista();

  bool esVacia();
  int getTamanio();
  void insertar(int pos, T dato);
  void insertarPrimero(T dato);
  void insertarUltimo(T dato);
  void remover(int pos);
  T getDato(int pos);
  void reemplazar(int pos, T dato);
  void vaciar();
  void print();
  Nodo<T>* getInicio(); // Método necesario para que HashMapList funcione
};

template <class T> Lista<T>::Lista() { inicio = nullptr; }
template <class T> Lista<T>::Lista(const Lista<T> &li) { inicio = li.inicio; }
template <class T> Lista<T>::~Lista() { vaciar(); }
template <class T> bool Lista<T>::esVacia() { return inicio == nullptr; }

template <class T> int Lista<T>::getTamanio() {
  Nodo<T> *aux = inicio;
  int size = 0;
  while (aux != nullptr) {
    aux = aux->getSiguiente();
    size++;
  }
  return size;
}

template <class T> void Lista<T>::insertar(int pos, T dato) {

  int posActual = 0;
    // se utilizan 1 cantidad de if
  Nodo<T> *aux = inicio, *nuevo;
  nuevo = new Nodo<T>(dato);  // Se pasa dato al constructor
  if (pos == 0) {
    nuevo->setSiguiente(inicio);
    inicio = nuevo;
    return;
  }

  while (aux != nullptr && posActual < pos - 1) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) throw 404;

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

template <class T> void Lista<T>::insertarPrimero(T dato) { insertar(0, dato); }

template <class T> void Lista<T>::insertarUltimo(T dato) {

  Nodo<T> *aux = inicio, *nuevo;
    // se utilizan 1 cantidad de if
  nuevo = new Nodo<T>(dato);  //  Se pasa dato al constructor
  if (aux == nullptr) {
    nuevo->setSiguiente(inicio);
    inicio = nuevo;
    return;
  }

  while (aux->getSiguiente() != nullptr) {
    aux = aux->getSiguiente();
  }

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

template <class T> void Lista<T>::remover(int pos) {

  Nodo<T> *aux = inicio, *aBorrar;
    // se utilizan 1 cantidad de if
  int posActual = 0;
  if (pos == 0) {
    inicio = inicio->getSiguiente();
    delete aux;
    return;
  }

  while (aux != nullptr && posActual < pos - 1) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) throw 404;

  aBorrar = aux->getSiguiente();
  aux->setSiguiente(aBorrar->getSiguiente());
  delete aBorrar;
}

template <class T> T Lista<T>::getDato(int pos) {

  Nodo<T> *aux = inicio;
  int posActual = 0;
  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) throw 404;

  return aux->getDato();
}

template <class T> void Lista<T>::reemplazar(int pos, T dato) {

  Nodo<T> *aux = inicio;
  int posActual = 0;
  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) throw 404;
  aux->setDato(dato);
}

template <class T> void Lista<T>::vaciar() {
  Nodo<T> *aux = inicio, *aBorrar;
  while (aux != nullptr) {
    aBorrar = aux;
    aux = aux->getSiguiente();
    delete aBorrar;
  }
  inicio = nullptr;
}

template <class T> void Lista<T>::print() {
  Nodo<T> *aux = inicio;
  while (aux != nullptr) {
    std::cout << aux->getDato() << " -> ";
    aux = aux->getSiguiente();
  }
  std::cout << "NULL" << std::endl;
}

template <class T> Nodo<T>* Lista<T>::getInicio() {
  return inicio;
}

#endif // U02_LISTAS_LISTA_LISTA_H_

// TOTAL en Lista.h:
// IF explícitos: 7
// IF implícitos (for, while, switch): 8
// IF de estructuras complejas (HashMap, AVL, Lista, etc): 31
// TOTAL ESTIMADO DE IFs: 46

