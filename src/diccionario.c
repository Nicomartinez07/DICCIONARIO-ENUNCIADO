#include "diccionario.h"
#include <string.h>

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
 * Devuelve el valor de hash calculado para la cadena
 */
unsigned long funcion_hash(char *cadena)
{
	unsigned long hash = 0;
	unsigned char c;

	while ((c = (unsigned char)*cadena++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

/*
 * Busca si la clave ya existia en el diccionario. Si existia, devuelve true y en posicion_encontrada devuelve el indice en el cual se la encontro.
 * En caso contrario, devuelve false.
 */
bool buscar_posicion_clave(diccionario_t *d, char *clave,
			   size_t *posicion_encontrada)
{
	if (!d || !clave || d->capacidad == 0)
		return false;

	bool encontrado = false;
	bool seguir_buscando = true;

	size_t intento = 0;
	unsigned long hash = funcion_hash(clave);
	size_t inicio = hash % d->capacidad;

	while (intento < d->capacidad && seguir_buscando && !encontrado) {
		size_t posicion = (inicio + intento) % d->capacidad;
		;
		entrada_t actual = d->tabla[posicion];

		if (actual.estado == VACIA) {
			seguir_buscando = false;
		} else if (actual.estado == OCUPADA &&
			   strcmp(actual.clave, clave) == 0) {
			encontrado = true;
			*posicion_encontrada = posicion;
		}

		intento++;
	}

	return encontrado;
}

/*
 * Devuevle en posicion_encontrada el indice del primer espacio disponible para insertar. Si en esa posicion habia un elemento y se elimino, devuelve true. False en caso contrario.
 */
bool buscar_posicion_disponible(diccionario_t *d, char *clave,
				size_t *posicion_encontrada,
				bool *posicion_estaba_eliminada)
{
	if (!d || !clave || d->capacidad == 0)
		return false;

	bool encontro_posicion_disponible = false;
	bool seguir_buscando = true;

	size_t intento = 0;
	unsigned long hash = funcion_hash(clave);
	size_t inicio = hash % d->capacidad;

	while (intento < d->capacidad && seguir_buscando) {
		size_t posicion = (inicio + intento) % d->capacidad;
		;
		entrada_t actual = d->tabla[posicion];

		if (actual.estado != OCUPADA) {
			seguir_buscando = false;
			encontro_posicion_disponible = true;
			if (actual.estado == ELIMINADA)
				*posicion_estaba_eliminada = true;
			*posicion_encontrada = posicion;
		}

		intento++;
	}

	return encontro_posicion_disponible;
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

bool insertar_en_tabla_nueva(entrada_t *tabla, size_t capacidad,
			     entrada_t entrada)
{
	size_t inicio = funcion_hash(entrada.clave) % capacidad;

	for (size_t intento = 0; intento < capacidad; intento++) {
		size_t posicion = (inicio + intento) % capacidad;

		if (tabla[posicion].estado != OCUPADA) {
			tabla[posicion] = entrada;
			return true;
		}
	}

	return false;
}

bool rehash(diccionario_t *d)
{
	size_t nueva_capacidad = d->capacidad + d->capacidad / 2;

	entrada_t *nueva_tabla = calloc(nueva_capacidad, sizeof(entrada_t));
	if (!nueva_tabla)
		return false;

	size_t i = 0;

	while (i < d->capacidad) {
		if (d->tabla[i].estado == OCUPADA) {
			if (!insertar_en_tabla_nueva(nueva_tabla,
						     nueva_capacidad,
						     d->tabla[i])) {
				free(nueva_tabla);
				return false;
			}
		}
		i++;
	}
	entrada_t *tabla_vieja = d->tabla;

	d->eliminadas = 0;
	d->capacidad = nueva_capacidad;
	d->tabla = nueva_tabla;

	free(tabla_vieja);
	return true;
}

/*
 * Crea un diccionario con una capacidad inicial. Si la capacidad inicial es <= 3, se utiliza 3.
 */
diccionario_t *diccionario_crear(unsigned capacidad_inicial)
{
	diccionario_t *diccionario = malloc(sizeof(diccionario_t));
	if (!diccionario) {
		return NULL;
	}

	if (capacidad_inicial < 3) {
		capacidad_inicial = 3;
	}

	entrada_t *tabla = calloc(capacidad_inicial, sizeof(entrada_t));
	if (!tabla) {
		free(diccionario);
		return NULL;
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
			  void **anterior)
{
	if (!d || !clave || d->capacidad == 0)
		return false;

	if (anterior)
		*anterior = NULL;

	size_t posicion = 0;
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

	//factor de carga superado
	while ((d->cantidad * 10) >= (d->capacidad * 7)) {
		if (!rehash(d)) {
			free(copia);
			return false;
		}
	}

	//Funcion q busca primera posicion libre y guarda en una variable la primera posicion eliminada
	//Guardar alguna de las 2 para ver en cual inserto el dato
	bool estaba_eliminada = false;
	bool encontro_posicion_disponible = buscar_posicion_disponible(
		d, clave, &posicion, &estaba_eliminada);
	if (!encontro_posicion_disponible) {
		free(copia);
		return false;
	}

	d->tabla[posicion].clave = copia;
	d->tabla[posicion].valor = valor;
	d->tabla[posicion].estado = OCUPADA;

	if (!esta_en_tabla)
		d->cantidad++;
	if (estaba_eliminada)
		d->eliminadas--;
	return true;
}

/*
 * Busca una clave en el diccionario y devuelve true si existe.
 */
bool diccionario_buscar(diccionario_t *d, char *clave)
{
	if (!d || !clave || d->capacidad == 0)
		return false;

	size_t posicion = 0;
	return buscar_posicion_clave(d, clave, &posicion);
}

/*
 * Busca una clave en el diccionario y devuelve el valor asociado.
 */
void *diccionario_obtener(diccionario_t *d, char *clave)
{
	if (!d || !clave || d->capacidad == 0)
		return NULL;

	size_t posicion = 0;
	if (!buscar_posicion_clave(d, clave, &posicion))
		return NULL;

	return d->tabla[posicion].valor;
}

/*
 * Elimina una clave de la tabla y devuelve el valor asociado a esa clave o NULL si no existía.
 */
void *diccionario_eliminar(diccionario_t *d, char *clave)
{
	if (!d || !clave || d->capacidad == 0)
		return NULL;

	size_t posicion = 0;

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
	if (!d || d->capacidad == 0)
		return 0;
	return d->cantidad;
}

/*
 * Recorre el diccionario y aplica la función f a cada valor del diccionario.
   Devuelve la cantidad de veces que se invoca la función f.
 */
size_t diccionario_iterar(diccionario_t *d, bool (*f)(char *, void *, void *),
			  void *extra)
{
	if (!d || d->capacidad == 0 || !f)
		return 0;

	size_t contador = 0;
	size_t invocaciones = 0;
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

	for (size_t i = 0; i < d->capacidad; i++) {
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

	for (size_t i = 0; i < d->capacidad; i++) {
		if (d->tabla[i].estado == OCUPADA) {
			if (destructor) {
				destructor(d->tabla[i].valor);
			}
			free(d->tabla[i].clave);
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
	it->indice = 0;

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
	if (!it)
		return;

	free(it);
}