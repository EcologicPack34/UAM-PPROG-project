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

---------------------------------------------------------------------------------------------------

NOTAS ITERACIÓN 2:
Darle al enter sin especificar un comando ejecuta el comando anterior.

En la descripcion solo se muestran los objetos del espacio actual, además, se puede ver en la descripcion
grafica de cada espacio algunos de los objetos que contiene. Esto se debe a que pensamos que ver donde se
encuentra cada objeto en todo momento le quita la gracia al juego, pues el proposito(que nosotros consideramos)
es que es necesario explorar el mapa para encontrar los objetos.

El combate está regulado por el módulo combat, sigue de forma similar los contenidos del guión, pero más adelante
se adaptará hacia un estilo de combate por turnos RPG, con implementación de habilidades, equipo, etc.

Existe una cierta probabilidad de que tras ejecutar ciertos comandos, si hay enemigos en el espacio, se entrará en
modo combate. Esto se puede desactivar cambiando la linea del evento responsable en anthill.dat

Vector2 es un modulo usado unicamente para asignar una posicion espacial a los espacios.

El mapeo de espacios utiliza un struct local a la funcion para guardar información durante el proceso de manera mas facil.
Esto se debe a que este struct unicamente es requerido en esta funcion.

El mapeo de espacios unicamente funciona entre el primer bloque de espacios adyacentes, es decir, si existe un grupo de espacios no adyacente
estos no se mostrarán bien en el graphic_engine.

Los enlaces entre espacios adyacentes estan marcadoso como un hueco en blanco.
Los enlaces bloqueados entre espacios adyacentes estan marcados como una linea.
Los espacios no adyacentes estan marcados como una 'o'.

Para ejecutar los test existen comandos en el makefile.

Los espacios antes de comandos y entre argumentos son ignorados, es decir, si hay mas de un espacio
estos se ignoraran. del mismo modo si el comando es correcto pero tiene espacios delante estos 
tambien serán ignorados.
