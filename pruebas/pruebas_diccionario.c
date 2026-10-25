#include "../src/diccionario.h"
#include "pa2m.h"

bool contar_elementos(char *clave, void *valor, void *aux)
{
	size_t *contador = aux;
	(*contador)++;

	return true;
}

void prueba_operaciones_diccionario_vacio()
{
	unsigned int capacidad = 3;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0,
		     "Al crearse el diccionario contiene 0 elementos.\n");

	pa2m_afirmar(!diccionario_buscar(d, "arbol"),
		     "No se puede buscar elementos en un diccionario vacio.");
	pa2m_afirmar(!diccionario_obtener(d, "arbol"),
		     "No se puede obtener elementos en un diccionario vacio");
	pa2m_afirmar(
		!diccionario_eliminar(d, "arbol"),
		"No se puede obtener elementos en un diccionario vacio,'.\n");

	diccionario_destruir(d);
}

void prueba_operaciones_diccionario_invalido()
{
	diccionario_t *d = NULL;

	int dato = 1;
	pa2m_afirmar(!diccionario_insertar(d, "arbol", &dato, NULL),
		     "No se puede insertar elementos con un diccionario NULL.");
	pa2m_afirmar(!diccionario_buscar(d, "arbol"),
		     "No se puede buscar elementos con un diccionario NULL.");
	pa2m_afirmar(!diccionario_obtener(d, "arbol"),
		     "No se puede obtener elementos con un diccionario NULL.");
	pa2m_afirmar(
		!diccionario_eliminar(d, "arbol"),
		"No se puede obtener elementos con un diccionario NULL.\n");

	diccionario_destruir(d);
}

void prueba_operaciones_clave_nula()
{
	unsigned int capacidad = 3;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0,
		     "Al crearse el diccionario contiene 0 elementos.\n");

	int dato = 1;
	pa2m_afirmar(!diccionario_insertar(d, NULL, &dato, NULL),
		     "No se puede insertar elementos con clave NULL.");
	pa2m_afirmar(!diccionario_buscar(d, NULL),
		     "No se puede buscar elementos con clave NULL.");
	pa2m_afirmar(!diccionario_obtener(d, NULL),
		     "No se puede obtener elementos con clave NULL.");
	pa2m_afirmar(!diccionario_eliminar(d, NULL),
		     "No se puede obtener elementos con clave NULL..\n");

	diccionario_destruir(d);
}

void prueba_operar_un_elemento()
{
	unsigned int capacidad = 5;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0,
		     "Al crearse el diccionario contiene 0 elementos.\n");

	int dato = 1;
	pa2m_afirmar(diccionario_insertar(d, "arbol", &dato, NULL),
		     "Se inserto un elemento correctamente.");
	pa2m_afirmar(
		diccionario_cantidad(d) == 1,
		"Al insertar un dato en el diccionario, este contiene 1 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "arbol"),
		     "En el diccionario hay un elemento con clave 'arbol'.");

	pa2m_afirmar(
		diccionario_obtener(d, "arbol") == &dato,
		"Al obtener el valor asociado a la clave insertada, coincide con el valor ingresado.\n");

	pa2m_afirmar(
		diccionario_eliminar(d, "arbol") == &dato,
		"Al eliminar el valor asociado a la clave insertada previamente, se obtiene el valor ingresado");

	pa2m_afirmar(
		diccionario_cantidad(d) == 0,
		"Al eliminar la unica clave del diccionario, este pasa a contiener 0 elementos.\n");

	diccionario_destruir(d);
}

void prueba_reemplazar_un_elemento()
{
	unsigned int capacidad = 5;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0,
		     "Al crearse el diccionario contiene 0 elementos.\n");

	int dato = 1;
	pa2m_afirmar(diccionario_insertar(d, "arbol", &dato, NULL),
		     "Se inserto un elemento con clave 'arbol' correctamente.");
	pa2m_afirmar(
		diccionario_cantidad(d) == 1,
		"Al insertar un dato en el diccionario, este contiene 1 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "arbol"),
		     "En el diccionario hay un elemento con clave 'arbol'.");

	pa2m_afirmar(
		diccionario_obtener(d, "arbol") == &dato,
		"Al obtener el valor asociado a la clave insertada, coincide con el valor ingresado.\n");

	void *anterior;
	int dato2 = 2;
	pa2m_afirmar(
		diccionario_insertar(d, "arbol", &dato2, &anterior),
		"Se inserto otro elemento con clave 'arbol' correctamente.");
	pa2m_afirmar(
		diccionario_cantidad(d) == 1,
		"Al insertar dos claves iguales, la cantidad sigue siendo 1.");
	pa2m_afirmar(
		diccionario_obtener(d, "arbol") == &dato2,
		"Al obtener el valor asociado a la clave cuyo valor fue reemplazado, coincide con el ultimo valor insertado.");
	pa2m_afirmar(
		*(int *)anterior == dato,
		"Al reemplazar un elemento, y obtener el anterior, los elementos coinciden.\n");

	diccionario_destruir(d);
}

void prueba_rehash()
{
	unsigned int capacidad = 10;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");

	char *claves[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	pa2m_afirmar(diccionario_insertar(d, claves[0], &valores[0], NULL),
		     "Inserta la primera clave");
	pa2m_afirmar(
		diccionario_obtener(d, claves[0]) == &valores[0],
		"Al obtener la clave recien insertada, el valor coincide.\n");

	for (size_t i = 1; i < capacidad; i++) {
		pa2m_afirmar(diccionario_insertar(d, claves[i], &valores[i],
						  NULL),
			     "Inserta otra clave");
		pa2m_afirmar(
			diccionario_obtener(d, claves[i]) == &valores[i],
			"Al obtener la clave recien insertada, el valor coincide.\n");
	}

	pa2m_afirmar(
		diccionario_cantidad(d) == capacidad,
		"La cantidad refleja la cantidad de elementos en el diccionario (10)");

	diccionario_destruir(d);
}

void prueba_eliminar_diferentes_posiciones()
{
	unsigned int capacidad = 10;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente\n");

	char *claves[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	for (size_t i = 0; i < capacidad; i++) {
		pa2m_afirmar(diccionario_insertar(d, claves[i], &valores[i],
						  NULL),
			     "Inserta una clave");
	}

	for (size_t j = 1; j < capacidad; j += 2) {
		pa2m_afirmar(
			diccionario_eliminar(d, claves[j]) == &valores[j],
			"Se elimina una clave impar y conciden los valores.");
		pa2m_afirmar(
			diccionario_obtener(d, claves[j]) == NULL,
			"Al querer obtener la clave impar recien eliminada, el valor es NULL.\n");
	}

	for (size_t k = 0; k < capacidad; k += 2) {
		pa2m_afirmar(
			diccionario_obtener(d, claves[k]) == &valores[k],
			"Se puede seguir obteniendo claves a pesar de que haya eliminadas.");
	}

	pa2m_afirmar(
		diccionario_cantidad(d) == 5,
		"La cantidad refleja la cantidad de elementos no eliminados en el diccionario\n");

	diccionario_destruir(d);
}

void prueba_colisionar()
{
	unsigned int capacidad = 10;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");

	int valores[4] = { 1, 2, 3, 4 };

	pa2m_afirmar(diccionario_insertar(d, "clave1", &valores[0], NULL),
		     "Inserta la primera clave");
	pa2m_afirmar(diccionario_insertar(d, "clave2", &valores[1], NULL),
		     "Inserta una clave que colisiona");
	pa2m_afirmar(diccionario_insertar(d, "clave3", &valores[2], NULL),
		     "Inserta otra clave que colisiona");
	pa2m_afirmar(diccionario_insertar(d, "clave4", &valores[3], NULL),
		     "Inserta otra clave que colisiona");

	pa2m_afirmar(diccionario_eliminar(d, "clave1") == &valores[0],
		     "Se elimina la primera clave y conciden los valores.");
	pa2m_afirmar(
		diccionario_obtener(d, "clave2") == &valores[1],
		"A pesar de haber eliminado un elemento se puede encontrar la siguiente clave");
	pa2m_afirmar(
		diccionario_obtener(d, "clave3") == &valores[2],
		"A pesar de haber eliminado un elemento se puede encontrar la siguiente clave");
	pa2m_afirmar(
		diccionario_obtener(d, "clave4") == &valores[3],
		"A pesar de haber eliminado un elemento, no se impide encontrar la ultima clave");
	pa2m_afirmar(
		diccionario_cantidad(d) == 3,
		"La cantidad refleja la cantidad de elementos en el diccionario (4 - eliminado)");

	diccionario_destruir(d);
}

void prueba_modificar_clave()
{
	unsigned int capacidad = 10;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");

	char *clave = "casa";
	int dato = 1;

	pa2m_afirmar(diccionario_insertar(d, clave, &dato, NULL),
		     "Inserta la clave 'casa'.\n");

	pa2m_afirmar(
		diccionario_obtener(d, clave) == &dato,
		"El valor de la clave buscada, coincide con el que se inserto previamente\n");

	clave = "carro";

	pa2m_afirmar(
		diccionario_obtener(d, "casa") == &dato,
		"A pesar de modificar la clave, en el diccionario sigue estando con la clave que se inserto. \n");

	pa2m_afirmar(
		!(diccionario_obtener(d, clave) == &dato),
		"A pesar de usar el mismo string, cuando la clave cambia, no se modifica en el diccionario\n");

	diccionario_destruir(d);
}

void prueba_iterar()
{
	unsigned int capacidad = 10;
	diccionario_t *d = diccionario_crear(capacidad);
	if (!d)
		return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");

	char *claves[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
	int valores[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	for (size_t i = 0; i < capacidad; i++) {
		pa2m_afirmar(diccionario_insertar(d, claves[i], &valores[i],
						  NULL),
			     "Inserta una clave");
		pa2m_afirmar(
			diccionario_obtener(d, claves[i]) == &valores[i],
			"Al obtener la clave recien insertada, el valor coincide.\n");
	}

	pa2m_afirmar(diccionario_eliminar(d, "i") == &valores[8],
		     "Se elimina la ante-ultima clave y conciden los valores.");

	size_t contador = 0;
	size_t cantidad = diccionario_iterar(d, contar_elementos, &contador);

	pa2m_afirmar(
		contador == 9,
		"El iterador recorre los elementos que no estan eliminados (9)");

	pa2m_afirmar(cantidad == 9,
		     "La funcion iterar devuelve 9 elementos recorridos");

	pa2m_afirmar(diccionario_cantidad(d) == 9,
		     "El diccionario contiene 9 elementos");

	diccionario_destruir(d);
}

void pruebas_diccionario()
{
	pa2m_nuevo_grupo("Prueba distintas operaciones con diccionario vacio");
	prueba_operaciones_diccionario_vacio();

	pa2m_nuevo_grupo("Prueba distintas operaciones con clave nula");
	prueba_operaciones_clave_nula();

	pa2m_nuevo_grupo("Prueba distintas operaciones con diccionario nulo");
	prueba_operaciones_diccionario_invalido();

	pa2m_nuevo_grupo("Prueba operar con un unico elemento");
	prueba_operar_un_elemento();

	pa2m_nuevo_grupo("Prueba reemplazar un elemento");
	prueba_reemplazar_un_elemento();

	pa2m_nuevo_grupo("Prueba redimensionamiento del diccionario");
	prueba_rehash();

	pa2m_nuevo_grupo(
		"Prueba colisiones, y ademas eliminar el primero y verificar que podes seguir obteniendo los demas");
	prueba_colisionar();

	pa2m_nuevo_grupo(
		"Prueba insersiones y eliminaciones. Demuestra que se puede seguir obteniendo elementos a pesar de haber eliminados");
	prueba_eliminar_diferentes_posiciones();

	pa2m_nuevo_grupo(
		"Prueba modificar una clave ya insertada, y que siga estando esa clave y no la modificada");
	prueba_modificar_clave();

	pa2m_nuevo_grupo(
		"Prueba buscar elementos con la funcion diccionario_iterar()");
	prueba_iterar();
}