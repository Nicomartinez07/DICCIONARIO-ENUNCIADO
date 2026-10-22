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

bool buscar_posicion(diccionario_t *d, char *clave, size_t *posicion_encontrada) {
    if(!d || !clave || d->capacidad == 0) 
        return false;

    bool encontrado = false;
    bool seguir_buscando = true;

    size_t intento = 0;
    unsigned long hash = funcion_hash(clave);
    size_t inicio = hash % d->capacidad;

    while(intento < d->capacidad && seguir_buscando && !encontrado) {
        size_t posicion = (inicio + intento) % d->capacidad;;
        entrada_t actual = d->tabla[posicion];
        
        if (actual.estado == VACIA) {
            seguir_buscando = false;
        } else if(actual.estado == OCUPADA && strcmp(actual.clave, clave) == 0) {
            encontrado = true;
            *posicion_encontrada = posicion;
        }

        intento++;
    }

    return encontrado;
}

void rehash () {

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

    entrada_t *tabla = malloc(sizeof(entrada_t) * capacidad_inicial);
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
    if(!d || !clave || d->capacidad == 0) return false;

    char* copia = clave;
    size_t posicion = 0;
    bool esta_en_tabla = buscar_posicion(d, clave, &posicion);
    if(esta_en_tabla) {
        d->tabla[posicion].valor = valor;
    } else {
        if(factor de carga superado) {
            rehash()
        }

        //calcular posicion hash
        //buscar posicion libre
    }

    d->tabla[posicion].clave = copia;
    d->tabla[posicion].valor = valor;
    d->tabla[posicion].estado = OCUPADA;
    d->cantidad++;
    return true;
}




/*
 * Busca una clave en el diccionario y devuelve true si existe.
 */
bool diccionario_buscar(diccionario_t *d, char *clave) {
    size_t posicion = 0;
    return buscar_posicion(d, clave, &posicion);
}

/*
 * Busca una clave en el diccionario y devuelve el valor asociado.
 */
void *diccionario_obtener(diccionario_t *d, char *clave){
    size_t posicion = 0;
    if(!buscar_posicion(d, clave, &posicion))
        return NULL;

    return d->tabla[posicion].valor;

}

/*
 * Elimina una clave de la tabla y devuelve el valor asociado a esa clave o NULL si no existía.
 */
void *diccionario_eliminar(diccionario_t *d, char *clave) {
    size_t posicion = 0;

    if(!buscar_posicion(d, clave, &posicion))
        return NULL;

    void *valor = d->tabla[posicion].valor;

    free(d->tabla[posicion].clave);
    d->tabla[posicion].clave = NULL;
    d->tabla[posicion].valor = NULL;
    d->tabla[posicion].estado = ELIMINADA;

    d->cantidad--;
    d->eliminadas++;

    return valor;
}

/*
 * Devuelve la cantidad de claves almacenadas en el diccionario.
 */
size_t diccionario_cantidad(diccionario_t *d) {
    return d->cantidad;
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
    if(!d) return;

    for(size_t i = 0; i < d->cantidad; i++) {
        if(d->tabla[i].estado == OCUPADA) {
            free(d->tabla[i].clave);
        }
    }
    free(d->tabla);
    free(d);
}

void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *)) {
    if(!d) return;

    for(size_t i = 0; i < d->cantidad; i++) {
        if(d->tabla[i].estado == OCUPADA) {
            if(destructor) {
                destructor(d->tabla[i].valor);
            }
            free(d->tabla[i].clave);
        }
    }
    free(d->tabla);
    free(d);
}

