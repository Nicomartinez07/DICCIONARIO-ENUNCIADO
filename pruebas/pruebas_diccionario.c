#include "../src/diccionario.h"
#include "pa2m.h"

void prueba_operaciones_diccionario_vacio() {
	unsigned int capacidad = 3;
	diccionario_t *d = diccionario_crear(capacidad);
	if(!d) return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0, "Al crearse el diccionario contiene 0 elementos.\n");

	pa2m_afirmar(!diccionario_buscar(d, "arbol"), "No se puede buscar elementos en un diccionario vacio.");
	pa2m_afirmar(!diccionario_obtener(d, "arbol"), "No se puede obtener elementos en un diccionario vacio");
	pa2m_afirmar(!diccionario_eliminar(d, "arbol"), "No se puede obtener elementos en un diccionario vacio,'.\n");

	diccionario_destruir(d);
}

void prueba_operaciones_diccionario_invalido() {
	diccionario_t *d = NULL;

	int dato = 1;
	pa2m_afirmar(!diccionario_insertar(d, "arbol", &dato, NULL), "No se puede insertar elementos con un diccionario NULL.");
	pa2m_afirmar(!diccionario_buscar(d, "arbol"), "No se puede buscar elementos con un diccionario NULL.");
	pa2m_afirmar(!diccionario_obtener(d, "arbol"), "No se puede obtener elementos con un diccionario NULL.");
	pa2m_afirmar(!diccionario_eliminar(d, "arbol"), "No se puede obtener elementos con un diccionario NULL.\n");

	diccionario_destruir(d);
}

void prueba_operaciones_clave_nula() {
	unsigned int capacidad = 3;
	diccionario_t *d = diccionario_crear(capacidad);
	if(!d) return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0, "Al crearse el diccionario contiene 0 elementos.\n");

	int dato = 1;
	pa2m_afirmar(!diccionario_insertar(d, NULL, &dato, NULL), "No se puede insertar elementos con clave NULL.");
	pa2m_afirmar(!diccionario_buscar(d, NULL), "No se puede buscar elementos con clave NULL.");
	pa2m_afirmar(!diccionario_obtener(d, NULL), "No se puede obtener elementos con clave NULL.");
	pa2m_afirmar(!diccionario_eliminar(d, NULL), "No se puede obtener elementos con clave NULL..\n");

	diccionario_destruir(d);
}

void prueba_operar_un_elemento(){
	unsigned int capacidad = 5;
	diccionario_t *d = diccionario_crear(capacidad);
	if(!d) return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0, "Al crearse el diccionario contiene 0 elementos.\n");

 	int dato = 1;
	pa2m_afirmar(diccionario_insertar(d, "arbol", &dato, NULL), "Se inserto un elemento correctamente.");
	pa2m_afirmar(diccionario_cantidad(d) == 1, "Al insertar un dato en el diccionario, este contiene 1 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "arbol"), "En el diccionario hay un elemento con clave 'arbol'.");

	void *dato_obtenido = diccionario_obtener(d, "arbol");
	pa2m_afirmar(*(int *)dato_obtenido == dato, "Al obtener el valor asociado a la clave insertada, coincide con el valor ingresado.\n");


	void *dato_eliminado = diccionario_eliminar(d, "arbol");
	pa2m_afirmar(*(int *)dato_eliminado == dato, "Al eliminar el valor asociado a la clave insertada previamente, se obtiene el valor ingresado");

	pa2m_afirmar(diccionario_cantidad(d) == 0, "Al eliminar la unica clave del diccionario, este pasa a contiener 0 elementos.\n");

	diccionario_destruir(d);
}

void prueba_reemplazar_un_elemento(){
	unsigned int capacidad = 5;
	diccionario_t *d = diccionario_crear(capacidad);
	if(!d) return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0, "Al crearse el diccionario contiene 0 elementos.\n");

 	int dato = 1;
	pa2m_afirmar(diccionario_insertar(d, "arbol", &dato, NULL), "Se inserto un elemento con clave 'arbol' correctamente.");
	pa2m_afirmar(diccionario_cantidad(d) == 1, "Al insertar un dato en el diccionario, este contiene 1 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "arbol"), "En el diccionario hay un elemento con clave 'arbol'.");

	void *dato_obtenido = diccionario_obtener(d, "arbol");
	pa2m_afirmar(*(int *)dato_obtenido == dato, "Al obtener el valor asociado a la clave insertada, coincide con el valor ingresado.\n");

	void *anterior;
	int dato2 = 2;
	pa2m_afirmar(diccionario_insertar(d, "arbol", &dato2, &anterior), "Se inserto otro elemento con clave 'arbol' correctamente.");
	pa2m_afirmar(diccionario_cantidad(d) == 1, "Al insertar dos claves iguales, la cantidad sigue siendo 1.");
	void *dato_obtenido_al_reemplazar = diccionario_obtener(d, "arbol");
	pa2m_afirmar(*(int *)dato_obtenido_al_reemplazar == dato2, "Al obtener el valor asociado a la clave cuyo valor fue reemplazado, coincide con el ultimo valor insertado.");
	pa2m_afirmar(*(int *)anterior == dato, "Al reemplazar un elemento, y obtener el anterior, los elementos coinciden.\n");

	diccionario_destruir(d);
}

void prueba_variar_capacidades() {
	unsigned int capacidad = 0;
	diccionario_t *d = diccionario_crear(capacidad);
	if(!d) return;

	pa2m_afirmar(d != NULL, "Se creo un diccionario correctamente");
	pa2m_afirmar(diccionario_cantidad(d) == 0, "Al crearse el diccionario contiene 0 elementos.\n");

 	int dato = 1;
	pa2m_afirmar(diccionario_insertar(d, "arbol", &dato, NULL), "Se inserto un elemento correctamente (clave: 'arbol').");
	pa2m_afirmar(diccionario_cantidad(d) == 1, "Al insertar un dato en el diccionario, este contiene 1 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "arbol"), "En el diccionario hay un elemento con clave 'arbol'.");
	void *dato_obtenido = diccionario_obtener(d, "arbol");
	pa2m_afirmar(*(int *)dato_obtenido == dato, "Al obtener el valor asociado a la clave 'arbol', coincide con el valor asociado a esa clave.\n");


	int dato2 = 2;
	pa2m_afirmar(diccionario_insertar(d, "arbul", &dato2, NULL), "Se inserto un segundo elemento correctamente (clave: 'arbul').");
	pa2m_afirmar(diccionario_cantidad(d) == 2, "Al insertar dos datos en el diccionario, este contiene 2 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "arbul"), "En el diccionario hay un elemento con clave 'arbul'.");
	void *dato_obtenido2 = diccionario_obtener(d, "arbul");
	pa2m_afirmar(*(int *)dato_obtenido2 == dato2, "Al obtener el valor asociado a la clave 'arbul', coincide con el valor asociado a esa clave.\n");


	int dato3 = 3;
	pa2m_afirmar(diccionario_insertar(d, "arbel", &dato3, NULL), "Se inserto un tercer correctamente (clave: 'arbel').");
	pa2m_afirmar(diccionario_cantidad(d) == 3, "Al insertar tres datos en el diccionario, este contiene 3 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "arbel"), "En el diccionario hay un elemento con clave 'arbel'.");
	void *dato_obtenido3 = diccionario_obtener(d, "arbel");
	pa2m_afirmar(*(int *)dato_obtenido3 == dato3, "Al obtener el valor asociado a la clave 'arbel', coincide con el valor asociado a esa clave.\n");

	
	pa2m_afirmar(diccionario_cantidad(d) == 3, "A pesar de que la capacidad inicial era '0', se pudo insertar elementos porque la capacidad minima es 3.\n");

	int dato4 = 4;
	pa2m_afirmar(diccionario_insertar(d, "casa", &dato4, NULL), "Se inserto un tercer correctamente (clave: 'casa').");
	pa2m_afirmar(diccionario_cantidad(d) == 4, "Al insertar cuatro datos en el diccionario, este contiene 4 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "casa"), "En el diccionario hay un elemento con clave 'casa'.");
	void *dato_obtenido4 = diccionario_obtener(d, "casa");
	pa2m_afirmar(*(int *)dato_obtenido4 == dato4, "Al obtener el valor asociado a la clave 'casa', coincide con el valor asociado a esa clave.\n");


	int dato5 = 5;
	pa2m_afirmar(diccionario_insertar(d, "edificio", &dato5, NULL), "Se inserto un tercer correctamente (clave: 'edificio').");
	pa2m_afirmar(diccionario_cantidad(d) == 5, "Al insertar tres datos en el diccionario, este contiene 5 elemento.");
	pa2m_afirmar(diccionario_buscar(d, "edificio"), "En el diccionario hay un elemento con clave 'edificio'.");
	void *dato_obtenido5 = diccionario_obtener(d, "edificio");
	pa2m_afirmar(*(int *)dato_obtenido5 == dato5, "Al obtener el valor asociado a la clave 'edificio', coincide con el valor asociado a esa clave.\n");


	pa2m_afirmar(diccionario_cantidad(d) == 5, "A pesar de que la capacidad minina es 3, se pudo insertar 5 elementos porque el diccionario se redimensiona en base a lo requerido.");

	diccionario_destruir(d);
}

void pruebas_diccionario()
{
	pa2m_nuevo_grupo("Prueba operar con diccionario vacio");
	prueba_operaciones_diccionario_vacio();

	pa2m_nuevo_grupo("Prueba operar con clave nula");
	prueba_operaciones_clave_nula();

	pa2m_nuevo_grupo("Prueba operar con diccionario nulo");
	prueba_operaciones_diccionario_invalido();

	pa2m_nuevo_grupo("Prueba operar con UN elemento");
	prueba_operar_un_elemento();

	pa2m_nuevo_grupo("Prueba reemplazar UN elemento");
	prueba_reemplazar_un_elemento();

	pa2m_nuevo_grupo("Prueba capacidades de un diccionario");
	prueba_variar_capacidades();
}