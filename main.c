#include "src/diccionario.h"
#include "src/leer_linea.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ESCRITURA "r"

const int ERROR = -1;
const int EXITO = 0;
const unsigned int CAPACIDAD_INICIAL = 3;
const int CANTIDAD_ARG_NECESARIA = 3;

/*
 * 
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

void imprimir_texto(diccionario_t *diccionario, const char *texto)
{
	if (!diccionario || !texto)
		return;

	char *copia_del_texto = copiar_texto(texto);
	if (!copia_del_texto)
		return;

	char *cursor = copia_del_texto;
	while (*cursor != '\0') {
		while (*cursor != '\0' && (*cursor == ' ' || *cursor == '\n')) {
			putchar(*cursor);
			cursor++;
		}

		if (*cursor == '\0')
			break;

		char *comienzo_de_palabra = cursor;
		while (*cursor != '\0' && *cursor != ' ' && *cursor != '\n') {
			cursor++;
		}

		char ultimo_caracter_de_palabra = *cursor;
		*cursor = '\0';

		void *reemplazo =
			diccionario_obtener(diccionario, comienzo_de_palabra);
		if (reemplazo) {
			printf("%s", (char *)reemplazo);
		} else {
			printf("%s", comienzo_de_palabra);
		}

		*cursor = ultimo_caracter_de_palabra;
	}

	free(copia_del_texto);
}

bool parsear_archivo(diccionario_t *diccionario, const char *nombre_archivo)
{
	if (!diccionario || !nombre_archivo)
		return false;

	FILE *archivo = fopen(nombre_archivo, ESCRITURA);
	if (!archivo)
		return false;

	bool resultado_guardar = true;
	char *linea = NULL;

	while ((linea = leer_linea(archivo)) != NULL && resultado_guardar) {
		size_t longitud = strlen(linea);
		if ((linea[longitud - 1] == '\n' ||
		     linea[longitud - 1] == '\r')) {
			linea[longitud - 1] = '\0';
		}

		char *division = strchr(linea, ',');
		if (!division) {
			free(linea);
			resultado_guardar = false;
			continue;
		}

		division = "\0";
		char *reemplazo = copiar_texto(division + 1);
		if (!reemplazo) {
			free(linea);
			resultado_guardar = false;
			continue;
		}

		void *anterior = NULL;
		if (!diccionario_insertar(diccionario, linea, reemplazo,
					  &anterior)) {
			free(reemplazo);
			free(linea);
			resultado_guardar = false;
		}

		free(anterior);
		free(linea);
	}

	fclose(archivo);

	if (resultado_guardar)
		return true;
	return false;
}

int main(int argc, char const *argv[])
{
	if (argc <= CANTIDAD_ARG_NECESARIA) {
		printf("Ejecucion incorrecta, uso: ./ejecutable <nombre_archivo> <texto>. \n");
		return ERROR;
	}

	diccionario_t *d = diccionario_crear(CAPACIDAD_INICIAL);
	if (!d)
		return ERROR;

	bool resultado_parsear = parsear_archivo(d, argv[1]);
	if (!resultado_parsear) {
		free(d);
		return ERROR;
	}

	imprimir_texto(d, (char *)argv[2]);

	diccionario_destruir_todo(d, free);
	return EXITO;
}
