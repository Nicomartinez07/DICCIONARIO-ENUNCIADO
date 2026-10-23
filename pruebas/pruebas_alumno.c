#include "pa2m.h"
#include <stdio.h>

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP DICCIONARIO ===============");

	pa2m_nuevo_grupo("");
	pa2m_nuevo_grupo("==== PRUEBAS BUSCAR ====");
	pruebas_buscar();

	pa2m_nuevo_grupo("");
	pa2m_nuevo_grupo("==== PRUEBAS ELIMINAR ====");
	pruebas_eliminar();

	pa2m_nuevo_grupo("");
	pa2m_nuevo_grupo("==== PRUEBAS INSERTAR ====");
	pruebas_insertar();

	pa2m_nuevo_grupo("");
	pa2m_nuevo_grupo("==== PRUEBAS OBTENER ====");
	pruebas_obtener();

	pa2m_nuevo_grupo("");
	pa2m_nuevo_grupo("==== PRUEBAS PROGRAMA ====");
	pruebas_programa();

	return pa2m_mostrar_reporte();
}
