# Beast Brawl
Beast Brawl es el nombre del juego que está siendo desarrollado por Clover Games Studio, un grupo formado por 6 personas de 4º curso del ininerario de Creación y Entretenimiento Digital del Grado en Ingeniería Multimedia:
* José Francisco Valdés
* Antonio José Martínez
* Judith Mula
* Carlos de la Fuente
* Rubén Rubio
* Jesús Mas

## Compilación

Para compilar nuestro proyecto debes situarte en el directorio raíz (NO en el subdirectorio `src`). Ahí habrá un archivo Makefile para compilar en terminal. Según qué comandos utilices, se trabajará con el proyecto del juego, el motor gráfico o los dos.

### Juego
* `make` para compilar el código y crear el archivo ejecutable.
* `make clean`para borrar los archivos compilados y el ejecutable.
* `make exe` para ejecutar.

### Motor gráfico
* `make gl` para compilar el código y crear el archivo ejecutable.
* `make clean_gl` para borrar los archivos compilados y el ejecutable.
* `make exe_gl`para ejecutar.

### Juego y motor gráfico
* `make all` borra todos los archivos compilados,borra los ejecutables y compila todo; pero solo ejecuta el juego.


## Controles de juego
Actualmente los contoles de nuestro juego funcionan mediante teclado.
 * `I` para acelerar.
 * `O` para descelerar.
 * `A` y `D` para moverse a izquierda y derecha.
 * `SPACE` para lanzar power-up.
 * Mantener presionada `Q` para cambiar a la cámara trasera.
 * `E` para cambiar a la cámara totem.
 * `F1` para comenzar partida desde el menú.
 * `F2` para poner el juego en pausa a mitad de partida.
 * `F3` para reanudar partida en pausa.
 * `F4` para volver al menú principal desde el manú pausa.
 * `ESC` para salir.
 
 ## Controles de debug
 * `F3` muestra el grafo y las colisiones.
 * `LShift + F3` activa el debug de navmeshes, posición destino, el path de dijkstra e información sobre la IA seleccionada (por defecto está seleccionada la IA 1).
     * `LShift + 1/2/3` cambia la selección de la IA.
     * `LShift + 0` muestra información de todas las IA.
