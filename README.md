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

### **Juego**
#### Linux
* `make` para compilar el código y crear el archivo ejecutable.
* `make clean` para borrar los archivos compilados y el ejecutable.
* `make exe` para ejecutar la versión predeterminada.

#### Windows
* `make -f MakefileWin.mk` para compilar el código y crear el archivo ejecutable.
* `make clean` para borrar los archivos compilados y el ejecutable.
* `nombre del ejecutable` para ejecutar. Por ejemplo `./Beast_Brawl.exe`

### **Motor grafico**
#### Linux
* `make gl` para compilar el código y crear el archivo ejecutable.
* `make clean_gl` para borrar los archivos compilados y el ejecutable.
* `make exe_gl`para ejecutar.

#### Windows
* `make -f MakefileWin.mk` para compilar el código y crear el archivo ejecutable.
* `make clean` para borrar los archivos compilados y el ejecutable.



### En ambos casos del juego
* El ejecutable tiene dos parámetros para determinar si las animaciones se cargarán o no, y la url del servidor online.
* `./Beast_Brawl [anim] [url]`, por ejemplo: `./Beast_Brawl 0 miservidor.com` sería para lanzar el juego sin animaciones y con el servidor en 'miservidor.com'
* Por ejemplo: `./Beast_Brawl 1 localhost` sería para lanzar el juego con animaciones y con el servidor en 'localhost'
* En ambos casos, si simplementa lanzas el ejecutable sin parámetros, los valores por defecto son animaciones activadas, y localhost.


### Juego y motor gráfico
* `make all` borra todos los archivos compilados,borra los ejecutables y compila todo; pero solo ejecuta el juego.


## Controles de juego TECLADO
Actualmente los contoles de nuestro juego funcionan mediante teclado.
 * En carrera: 
    * `W` para acelerar.
    * `S` para descelerar e ir marcha atrás.
    * `A` y `D` para moverse a izquierda y derecha.
    * Mantener `U` para derrapar.
    * `SPACE` para lanzar power-up.
    * Mantener presionada `I` para cambiar a la cámara trasera.
    * Mantener presionada `O` para cambiar a la cámara del totem.
    * Sonar el claxon `P`.
    * Ocultar partes del HUD `R`.
    * `ESC` para poner el juego en pausa.
 * Menú intro:
    * `SPACE` para ir al menú principal.
 * En el menú principal:
    * Moverse entre opciones con las flechas.
    * `SPACE` para seleccionar.
    * `BACK` para atrás.
 * Desde el menú de pausa:
    * Moverse entre opciones con las flechas.
    * `SPACE` para seleccionar.
 * Desde la pantalla de final de partida:
    * Moverse entre opciones con las flechas en el segundo menú.
    * `SPACE` para volver al menú principal.

## Controles de juego MANDO
Actualmente los contoles de nuestro juego funcionan mediante teclado.
 * En carrera: 
    * `RT` para acelerar.
    * `LT` para descelerar.
    * `Joystick` para moverse a izquierda y derecha.
    * Mantener `X` para derrapar.
    * `A` para lanzar power-up.
    * Mantener presionada `LB` para cambiar a la cámara trasera.
    * Mantener presionada `Y` para cambiar a la cámara del totem.
    * Ocultar partes del HUD `B`.
    * Sonar el claxon `StickPressR`.
    * `START` para poner el juego en pausa.
 * Menú intro:
    * `A` para ir al menú principal.
 * En el menú principal:
    * `A` para seleccionar.
    * `Joystick` para moverse entre opciones.
    * `B` para atrás.
 * Desde el menú de pausa:
    * `A` para seleccionar.
    * `Joystick` para moverse entre opciones.
 * Desde la pantalla de final de partida:
    * `Joystick` para moverse entre opciones en el segundo menú.
    * `A` para volver al menú principal.

 
 ## Controles de debug
 * `F3` muestra el grafo y las colisiones.
 * `F1` oculta el HUD.
 * `LShift + F3` activa el debug de navmeshes, posición destino, el path de dijkstra e información sobre la IA seleccionada (por defecto está seleccionada la IA 1).
     * `LShift + 1/2/3` cambia la selección de la IA.
     * `LShift + 0` muestra información de todas las IA.
