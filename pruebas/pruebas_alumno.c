#include "pa2m.h"
#include <stdio.h>
#include "pruebas.h"

int main()
{
	pa2m_nuevo_grupo(
		"============== PRUEBAS DEL TP DICCIONARIO ===============");

	pa2m_nuevo_grupo("");
	pa2m_nuevo_grupo("==== PRUEBAS DICCIONARIO ====");
	pruebas_diccionario();

	return pa2m_mostrar_reporte();
}
