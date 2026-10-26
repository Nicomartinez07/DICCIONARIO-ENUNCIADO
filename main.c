#include "src/diccionario.h"
#include "src/leer_linea.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LECTURA "r"

const int ERROR = -1;
const int EXITO = 0;
const unsigned int CAPACIDAD_INICIAL = 3;
const int CANTIDAD_ARG_NECESARIA = 3;
const char CARACTER_CERO_LITERAL = '\0';
const char CARACTER_VACIO = ' ';
const char CARACTER_SALTO_LINEA = '\n';
const char CARACTER_ESCAPE = '\r';
const char CARACTER_COMA = ',';
const int POSICION_ARCHIVO = 1;
const int POSICION_TEXTO = 2;

/*
 * Copia el texto en una cadena nueva para no modificar el texto original.
 */
char *copiar_texto(const char *texto)
{
	if (!texto)
		return NULL;

	size_t longitud = strlen(texto);
	char *copia = malloc(sizeof(char) * (longitud + 1));
	if (!copia)
		return NULL;

	memcpy(copia, texto, longitud + 1);
	return copia;
}

/*
 * Dado una palabra, la busca como clave en el diccionario e imprimer el valor asociado en el caso de que se encuentre en el diccionario.
 * En el caso de no estar en el diccionario, imprime la misma palabra.
 */
void imprimir_palabra(diccionario_t *diccionario, char* palabra) {
	void *reemplazo = diccionario_obtener(diccionario, palabra);

	if (reemplazo) {
		printf("%s", (char *)reemplazo);
	} else {
		printf("%s", palabra);
	}
}

/*
 * Parsea un texto recorriendo por palabras y lo imprime a medida que lo va recorriendo.
 */
void imprimir_texto(diccionario_t *diccionario, const char *texto)
{
	if (!diccionario || !texto)
		return;

	char *copia_del_texto = copiar_texto(texto);
	if (!copia_del_texto)
		return;

	char *cursor = copia_del_texto;
	while (*cursor != CARACTER_CERO_LITERAL) {
		while (*cursor != CARACTER_CERO_LITERAL && (*cursor == CARACTER_VACIO || *cursor == CARACTER_SALTO_LINEA)) {
			putchar(*cursor);
			cursor++;
		}

		if (*cursor != CARACTER_CERO_LITERAL) {
			char *comienzo_de_palabra = cursor;

			while (*cursor != CARACTER_CERO_LITERAL &&
			       *cursor != CARACTER_VACIO &&
			       *cursor != CARACTER_SALTO_LINEA) {
				cursor++;
			}

			char ultimo_caracter_de_palabra = *cursor;
			*cursor = CARACTER_CERO_LITERAL;

			imprimir_palabra(diccionario, comienzo_de_palabra);

			*cursor = ultimo_caracter_de_palabra;	
		}
	}

	printf("\n");
	free(copia_del_texto);
}

/*
 * Parsea el archivo ingresado.
 * Por cada linea del archivo, busca la coma separadora, 
 * e inserta en el diccionario a la primera palabra como una clave y a la segunda como un valor.
 * Devuelve true si pudo realizar la operacion con exito, false en caso contrario.
 */
bool parsear_archivo(diccionario_t *diccionario, const char *nombre_archivo)
{
	if (!diccionario || !nombre_archivo)
		return false;

	FILE *archivo = fopen(nombre_archivo, LECTURA);
	if (!archivo)
		return false;

	bool resultado_guardar = true;
	char *linea = NULL;

	while ((linea = leer_linea(archivo)) != NULL && resultado_guardar) {
		size_t longitud = strlen(linea);
		if ((linea[longitud - 1] == CARACTER_SALTO_LINEA ||
			linea[longitud - 1] == CARACTER_ESCAPE)) {
			linea[longitud - 1] = CARACTER_CERO_LITERAL;
		}

		char *division = strchr(linea, CARACTER_COMA);
		if (!division) {
			resultado_guardar = false;
		} else {
			*division = CARACTER_CERO_LITERAL;
			char *reemplazo = copiar_texto(division + 1);

			if (!reemplazo) {
				resultado_guardar = false;
			} else {
				void *anterior = NULL;

				if (!diccionario_insertar(diccionario, linea, reemplazo,
							&anterior)) {
					free(reemplazo);
					resultado_guardar = false;
				}
				free(anterior);
			}
		}
		free(linea);
	}
	fclose(archivo);

	if (resultado_guardar)
		return true;
	return false;
}


int main(int argc, char const *argv[])
{
	if (argc != CANTIDAD_ARG_NECESARIA) {
		return ERROR;
	}

	diccionario_t *d = diccionario_crear(CAPACIDAD_INICIAL);
	if (!d)
		return ERROR;

	bool resultado_parsear = parsear_archivo(d, argv[POSICION_ARCHIVO]);
	if (!resultado_parsear) {
		diccionario_destruir_todo(d, free);
		return ERROR;
	}

	imprimir_texto(d, (char *)argv[POSICION_TEXTO]);

	diccionario_destruir_todo(d, free);
	return EXITO;
}
