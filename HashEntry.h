// Archivo: HashEntry.h
// Descripción: Este archivo forma parte del sistema de análisis de ventas y contiene definiciones relevantes para su funcionamiento.

#ifndef U05_HASH_HASHMAP_HASHENTRY_H_
#define U05_HASH_HASHMAP_HASHENTRY_H_

// ======================= CLASE HASHENTRY =======================
// Representa una entrada de la tabla hash (clave-valor)

template <class K, class T>
class HashEntry
{
private:
    K clave;   // Clave única asociada al valor
    T valor;   // Valor almacenado

public:
    // Constructor: inicializa clave y valor
    HashEntry(K c, T v){
        clave = c;
        valor = v;
    }

    // Devuelve la clave de la entrada
    K getClave(){
        return clave;
    }

    // Modifica la clave de la entrada
    void setClave(K c){
        clave = c;
    }

    // Devuelve el valor de la entrada
    T getValor(){
        return valor;
    }

    // Modifica el valor de la entrada
    void setValor(T v){
        valor = v;
    }
};

#endif // U05_HASH_HASHMAP_HASHENTRY_H_

