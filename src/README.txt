NOTAS IMPORTANTES SOBRE EL PROYECTO:

Este proyecto cumple con los requisitos de funcionalidad y documentación de la iteración 1, pero
existe mucho código no implementado o no utilizado, que se usará en posteriores iteraciones del
proyecto. Por ejemplo, collector no se emplea en ninguno de los otros módulos, pero posiblemente
se use para la interfaz gráfica, utils.h tampoco se usa por el momento y el inventario y los
objetos no están implementados al completo, sino que parcialmente para cumplir los requisitos.

Para gestionar errores se ha empleado el modulo debug_printing. Este implementa un log llamado
debug.log donde se almacena información relacionada con la ejecución del programa, funciones de
alto nivel y demás, se ha utilizado bastante para localizar errores. debug.log sólo se genera
después de ejecutar el juego y salir. En caso de que el programa pare su ejecución de forma
anormal no tendra contenido.

En la representación gráfica están implementados los links, que permiten un enlace entre espacioes
más dinámico, está completamente implementado.

Player cumple los requisitos de funcionalidad, pero porque almacena otra estructura de tipo entity
la cual contiene datos básicos para cualquier player, aliado, enemigo... Es la base para futuras
iteraciones.