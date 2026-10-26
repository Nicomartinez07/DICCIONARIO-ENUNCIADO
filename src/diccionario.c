#include "diccionario.h"
#include <string.h>

const int DOBLE = 2;
const int CERO = 0;
const int CAPACIDAD_MINIMA = 3;
const size_t FACTOR_CARGA_NUMERADOR = 7;
const size_t FACTOR_CARGA_DENOMINADOR = 10;

typedef enum { VACIA, OCUPADA, ELIMINADA } estado_t;

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

typedef struct iterador {
	entrada_t *t;
	size_t indice;
	size_t cantidad;
	size_t capacidad;
} diccionario_iterador_t;

/*
 * Devuelve el valor de hash calculado para la cadena ingresada.
 */
unsigned long funcion_hash(char *cadena)
{
	unsigned long hash = CERO;
	unsigned char c;

	while ((c = (unsigned char)*cadena++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

/*
 * Busca la posicion de una clave existente o una posicion disponible. En caso de encontrarlo devuelve true, false en caso contrario.
 */
bool buscar_posicion(diccionario_t *d, char *clave,
			    size_t *posicion_encontrada,
			    bool buscar_disponible,
			    bool *posicion_estaba_eliminada)
{
	if (!d || !clave || !posicion_encontrada || d->capacidad == CERO)
		return false;

	bool encontrado = false;
	bool seguir_buscando = true;

	size_t intento = CERO;
	unsigned long hash = funcion_hash(clave);
	size_t inicio = hash % d->capacidad;

	while (intento < d->capacidad && seguir_buscando && !encontrado) {
		size_t posicion = (inicio + intento) % d->capacidad;
		entrada_t actual = d->tabla[posicion];

		if (buscar_disponible) {
			if (actual.estado != OCUPADA) {
				encontrado = true;
				*posicion_encontrada = posicion;

				if (posicion_estaba_eliminada)
					*posicion_estaba_eliminada =
						actual.estado == ELIMINADA;
			}
		} else {
			if (actual.estado == VACIA) {
				seguir_buscando = false;
			} else if (actual.estado == OCUPADA &&
				   strcmp(actual.clave, clave) == CERO) {
				encontrado = true;
				*posicion_encontrada = posicion;
			}
		}

		intento++;
	}

	return encontrado;
}

/*
 * Busca si la clave ya existia en el diccionario. Si existia, devuelve true y en posicion_encontrada devuelve el indice en el cual se la encontro.
 * En caso contrario, devuelve false.
 */
bool buscar_posicion_clave(diccionario_t *d, char *clave,
			   size_t *posicion_encontrada)
{
	return buscar_posicion(d, clave, posicion_encontrada,
			       false, NULL);
}

/*
 * Devuevle true, si pudo encontrar una posicion disponible. 
 * Si encontro la posicion en posicion_encontrada devuelve el indice del primer espacio disponible para insertar. 
 * Si se pasa por parametro un puntero, indica si la posicion encontrada era una posicion eliminada.
 */
bool buscar_posicion_disponible(diccionario_t *d, char *clave,
				size_t *posicion_encontrada,
				bool *posicion_estaba_eliminada)
{
	if (!posicion_estaba_eliminada)
		return false;

	return buscar_posicion(d, clave, posicion_encontrada,
			       true, posicion_estaba_eliminada);
}

/*
 * Copia la clave recibida por parametros
 */
char *copiar_clave(char *clave)
{
	size_t longitud = strlen(clave);
	char *copia = malloc(sizeof(char) * longitud + 1);
	if (!copia)
		return NULL;

	strcpy(copia, clave);

	return copia;
}

/*
 * Devuelve true si puede insertar la entrada en una posicion de la tabla nueva, false en caso contrario
 */
bool insertar_en_tabla_nueva(entrada_t *tabla, size_t capacidad,
			     entrada_t entrada)
{
	size_t inicio = funcion_hash(entrada.clave) % capacidad;
	size_t intento = CERO;
	bool pudo_insertar = false;

	while(intento < capacidad && !pudo_insertar) {
		size_t posicion = (inicio + intento) % capacidad;

		if (tabla[posicion].estado != OCUPADA) {
			tabla[posicion] = entrada;
			pudo_insertar = true;
		}

		intento++;
	}

	return pudo_insertar;
}

/*
 * Redimensiona la tabla actual a una tabla 1.5 veces mas grande, vuelca todos los elementos de la tabla anterior a la nueva, cuyo estado sean OCUPADOS
 * Devuelve true si el proceso de rehash se realiza correctamente, false en caso contrario.
 */
bool rehash(diccionario_t *d)
{
	size_t nueva_capacidad = d->capacidad + d->capacidad / (long unsigned)DOBLE;

	entrada_t *nueva_tabla = calloc(nueva_capacidad, sizeof(entrada_t));
	if (!nueva_tabla)
		return false;

	size_t i = CERO;
	bool pudo_realizar_rehash = true;

	while (i < d->capacidad && pudo_realizar_rehash) {
		if (d->tabla[i].estado == OCUPADA) {
			if (!insertar_en_tabla_nueva(nueva_tabla,
						     nueva_capacidad,
						     d->tabla[i])) {
				free(nueva_tabla);
				pudo_realizar_rehash = false;
			}
		}
		i++;
	}
	
	entrada_t *tabla_vieja = d->tabla;

	d->eliminadas = 0;
	d->capacidad = nueva_capacidad;
	d->tabla = nueva_tabla;

	free(tabla_vieja);
	return pudo_realizar_rehash;
}

/*
 * Crea un diccionario con una capacidad inicial. Si la capacidad inicial es <= 3, se utiliza 3.
 */
diccionario_t *diccionario_crear(unsigned int capacidad_inicial)
{
	diccionario_t *diccionario = malloc(sizeof(diccionario_t));
	if (!diccionario) {
		return NULL;
	}

	if (capacidad_inicial < CAPACIDAD_MINIMA) {
		capacidad_inicial = CAPACIDAD_MINIMA;
	}

	entrada_t *tabla = calloc(capacidad_inicial, sizeof(entrada_t));
	if (!tabla) {
		free(diccionario);
		return NULL;
	}

	diccionario->tabla = tabla;
	diccionario->capacidad = capacidad_inicial;
	diccionario->cantidad = CERO;
	diccionario->eliminadas = CERO;

	return diccionario;
}

/*
 * Busca primera posicion libre e inserta el elemento en esa posicion del la tabla. Devuele true si fue exitoso
 */
bool asegurar_capacidad_correcta(diccionario_t *d) {
	bool salio_bien_rehash = true;

	while ((d->cantidad * FACTOR_CARGA_DENOMINADOR) >= (d->capacidad * FACTOR_CARGA_NUMERADOR) &&
	       salio_bien_rehash) {
		salio_bien_rehash = rehash(d);
	}

	return salio_bien_rehash;
}

/*
 * Busca primera posicion libre e inserta el elemento en esa posicion del la tabla. Devuele true si fue exitoso
 */
bool insertar_en_posicion_disponible(diccionario_t *d,
					    char *clave,
					    char *copia,
					    void *valor)
{
	size_t posicion = CERO;
	bool estaba_eliminada = false;

	bool hay_posicion_disponible = buscar_posicion_disponible(
		d, clave, &posicion, &estaba_eliminada);

	if (!hay_posicion_disponible)
		return false;

	d->tabla[posicion].clave = copia;
	d->tabla[posicion].valor = valor;
	d->tabla[posicion].estado = OCUPADA;

	d->cantidad++;
	if (estaba_eliminada)
		d->eliminadas--;

	return true;
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
			  void **anterior)
{
	if (!d || !clave || d->capacidad == CERO)
		return false;

	if (anterior)
		*anterior = NULL;

	size_t posicion = CERO;
	bool esta_en_tabla = buscar_posicion_clave(d, clave, &posicion);
	if (esta_en_tabla) {
		if (anterior)
			*anterior = d->tabla[posicion].valor;
		d->tabla[posicion].valor = valor;
		return true;
	}

	char *copia = copiar_clave(clave);
	if (!copia)
		return false;

	if (!asegurar_capacidad_correcta(d)) {
		free(copia);
		return false;
	}
	
	insertar_en_posicion_disponible(d, clave, copia, valor);

	return true;
}

/*
 * Busca una clave en el diccionario y devuelve true si existe.
 */
bool diccionario_buscar(diccionario_t *d, char *clave)
{
	if (!d || !clave || d->capacidad == CERO)
		return false;

	size_t posicion = CERO;
	return buscar_posicion_clave(d, clave, &posicion);
}

/*
 * Busca una clave en el diccionario y devuelve el valor asociado.
 */
void *diccionario_obtener(diccionario_t *d, char *clave)
{
	if (!d || !clave || d->capacidad == CERO)
		return NULL;

	size_t posicion = CERO;
	if (!buscar_posicion_clave(d, clave, &posicion))
		return NULL;

	return d->tabla[posicion].valor;
}

/*
 * Elimina una clave de la tabla y devuelve el valor asociado a esa clave o NULL si no existía.
 */
void *diccionario_eliminar(diccionario_t *d, char *clave)
{
	if (!d || !clave || d->capacidad == CERO)
		return NULL;

	size_t posicion = CERO;

	if (!buscar_posicion_clave(d, clave, &posicion))
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
size_t diccionario_cantidad(diccionario_t *d)
{
	if (!d || d->capacidad == CERO)
		return CERO;
	return d->cantidad;
}

/*
 * Recorre el diccionario y aplica la función f a cada valor del diccionario.
   Devuelve la cantidad de veces que se invoca la función f.
 */
size_t diccionario_iterar(diccionario_t *d, bool (*f)(char *, void *, void *),
			  void *extra)
{
	if (!d || d->capacidad == CERO || !f)
		return CERO;

	size_t contador = CERO;
	size_t invocaciones = CERO;
	bool seguir = true;

	while (contador < d->capacidad && seguir) {
		entrada_t actual = d->tabla[contador];

		if (actual.estado == OCUPADA) {
			seguir = f(actual.clave, actual.valor, extra);
			invocaciones++;
		}

		contador++;
	}

	return invocaciones;
}

/*
 * Destruye el diccionario y la memoria asociada.
 */
void diccionario_destruir(diccionario_t *d)
{
	if (!d)
		return;

	for (size_t i = CERO; i < d->capacidad; i++) {
		if (d->tabla[i].estado == OCUPADA) {
			free(d->tabla[i].clave);
		}
	}
	free(d->tabla);
	free(d);
}

/*
 * Destruye el diccionario y la memoria asociada, aplicandole la funcion destructor a cada valor del diccionario.
 */
void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *))
{
	if (!d)
		return;

	for (size_t i = CERO; i < d->capacidad; i++) {
		if (d->tabla[i].estado == OCUPADA) {
			free(d->tabla[i].clave);
			if (destructor) {
				destructor(d->tabla[i].valor);
			}
		}
	}
	free(d->tabla);
	free(d);
}

/*
 * Crea un iterador de diccionario
 */
diccionario_iterador_t *diccionario_iterador_crear(diccionario_t *d)
{
	if (!d)
		return NULL;

	diccionario_iterador_t *it = malloc(sizeof(diccionario_iterador_t));
	if (!it)
		return NULL;

	it->t = d->tabla;
	it->cantidad = d->cantidad;
	it->capacidad = d->capacidad;
	it->indice = CERO;

	while (it->indice < it->capacidad - 1 &&
	       it->t[it->capacidad - 1].estado != OCUPADA) {
		it->indice++;
	}
	return it;
}

/*
 * Devuelve true si hay mas entradas para iterar
 */
bool diccionario_iterador_se_puede_iterar(diccionario_iterador_t *it)
{
	if (!it)
		return false;
	return it->indice < it->capacidad;
}

/*
 * Avanza a la siguiente iteración
 */
void diccionario_iterador_siguiente(diccionario_iterador_t *it)
{
	if (!it || it->indice >= it->capacidad)
		return;

	it->indice++;

	while (it->indice < it->capacidad - 1 &&
	       it->t[it->indice].estado != OCUPADA) {
		it->indice++;
	}
}

/*
 * Devuelve el valor de la entrada actual iterada
 */
void *diccionario_iterador_obtener_valor(diccionario_iterador_t *it)
{
	if (!it || it->indice >= it->capacidad)
		return NULL;

	return it->t[it->indice].valor;
}

/*
 * Destruye el iterador
 */
void diccionario_iterador_destruir(diccionario_iterador_t *it)
{
	free(it);
}