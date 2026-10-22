#include "diccionario.h"

typedef enum {
    VACIA,
    OCUPADA,
    ELIMINADA
} estado_t;

typedef struct {
    char *clave;
    void *valor;
    estado_t estado;
} entrada_t;

struct diccionario {
    entrada_t *tabla;
    size_t capacidad;
    size_t cantidad;
    size_t eliminadas;
};

/*
 *
 */
unsigned long funcion_hash(char *cadena) {
    unsigned long hash = 0;
    int c;

    while ((c = (unsigned char)*cadena++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

/*
 * Crea un diccionario con una capacidad inicial. Si la capacidad inicial es <= 3, se utiliza 3.
 */
diccionario_t *diccionario_crear(unsigned capacidad_inicial) {
    diccionario_t *diccionario = malloc(sizeof(diccionario_t));
    if(diccionario) {
        return NULL;
    }

    if(capacidad_inicial < 3) {
        capacidad_inicial = 3;
    }

    entrada_t *tabla = malloc(sizeof(entrada_t * capacidad_inicial));
    if(!tabla) {
        return NULL;
        free(diccionario);
    }

    diccionario->tabla = tabla;
    diccionario->capacidad = capacidad_inicial;
    diccionario->cantidad = 0;
    diccionario->eliminadas = 0;

    return diccionario;

}

/*
 * Inserta clave/valor, devuelve true si pudo o false si no pudo. Si ya existía
 * la clave apunta anterior al valor asociado anteriormente.
 *
 * Si se necesita agrandar la tabla no está permitido hacerla crecer a mas del doble del tamaño.
 *
 *  No es válido insertar claves NULL.
 */
bool diccionario_insertar(diccionario_t *d, char *clave, void *valor,
			  void **anterior) {

              }


/*
    Buscar: calculás la posición inicial con hash(clave) % capacidad y seguís probando posiciones. Si encontrás OCUPADA con la misma clave, la encontraste.
                                                                                                   Si encontrás ELIMINADA, seguís buscando. 
                                                                                                   Si encontrás VACIA, la clave no está.

    Calcular indice -> Buscar en el indice indicado -> si estado ==  OCUPADO y son iguales las claves -> retornar el valor
                                                        Estado == Eliminado -> avanzar al siguiente indice 
                                                        Estado == Vacia -> Retornar null y dejar de iterar
    */

/*
 * Busca una clave en el diccionario y devuelve true si existe.
 */
bool diccionario_buscar(diccionario_t *d, char *clave) {
    if(!d || !clave) {
        return false;
    }

    bool encontrado = false;
    size_t posicion_inicial =
        (size_t)(funcion_hash(clave) % d->capacidad);

    if(d->tabla[posicion].estado == OCUPADA && d->tabla[posicion].clave == clave) {
        encontrado = true;
    }   
    size_t intento = 0;

    while(!encontrado && d->tabla[posicion].estado != VACIA && intento < d->capacidad) {
        posicion = (posicion + intento) % d->capacidad;

        
        if(d->tabla[posicion].estado == OCUPADA && d->tabla[posicion].clave == clave) {
            encontrado = true;
        }
        intento++;
    }

    return encontrado;
}

/*
 * Busca una clave en el diccionario y devuelve el valor asociado.
 */
void *diccionario_obtener(diccionario_t *d, char *clave){

}

/*
 * Elimina una clave de la tabla y devuelve el valor asociado a esa clave o NULL si no existía.
 */
void *diccionario_eliminar(diccionario_t *d, char *clave) {

}

/*
 * Devuelve la cantidad de claves almacenadas en el diccionario.
 */
size_t diccionario_cantidad(diccionario_t *d) {

}

/*
 * Recorre el diccionario y aplica la función f a cada valor del diccionario.
   Devuelve la cantidad de veces que se invoca la función f.
 */
size_t diccionario_iterar(diccionario_t *d, bool (*f)(char *, void *, void *),
			  void *extra) {

              }

/*
 * Destruye el diccionario y la memoria asociada.
 */
void diccionario_destruir(diccionario_t *d) {

}

void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *)) {

}

