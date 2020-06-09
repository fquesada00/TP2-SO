---------ARQUI-TPE-1Q2020------------
Alumnos:
	* Serpe, Octavio
	* Quesada, Francisco

**PARA COMPILAR Y EJECUTAR EN LINUX**
1) Entrar a la carpeta "RowDaBoat-x64barebones-d4e1c147f975" y abrir una terminal.
2) Ejecutar el comando 'cd Toolchain && make all && cd ..'
3) Ejecutar el comando 'make all'
4) Dar permisos de ejecucion al archivo 'run.sh'
5) Ejecutar el archivo 'run.sh'
ACLARACION:
Se recomienda compilar los archivos utilizando, en docker, la imagen provista por la cátedra.

**USO DEL SISTEMA**
Una vez iniciado el sistema se desplegara un mensaje preguntando que programa desea ejecutar.
Presione 'Ctrl + 1' si desea ejecutar la calculadora.
Presione 'Ctrl + 2' si desea ejecutar la consola de comandos.
La calculadora siempre aparecerá en la ventana izquierda y la shell en la ventana derecha.

**Calculadora**
Calculadora que realiza suma, resta, producto y cociente.
Toda cuenta debe estar encerrada entre parentesis, en base a la asociatividad que desee el usuario.
Para realizar una cuenta, escribirla de manera correcta, escribir un "=" al final y presionar la tecla enter.
Para borrar el ultimo caracter introducido, presionar la tecla backspace.
Para borrar toda la linea, presionar la tecla enter.
Para los simbolos *,/,( y ), presionarlos manteniendo presionada la tecla shift.
Para los simbolos + y -, presionar las teclas de la parte izquierda del teclado, al igual que en el item anterior.
Ejemplos de uso:
((3+2)*4)=
> 20
(6*(4+2))=
> 36
**SHELL**
SHELL COMMANDS LIST:
inforeg: Muestra por salida estandard el valor de los registros.
Presionar ctrl + r para guardar los registros en el momento deseado y luego ejecutar el comando.
printmem: Recibe una direccion de memoria y realiza un volcado de memoria de 32 bytes a partir de la direccion indicada.
time: Muestra por salida estandard la hora del sistema.
processor: Muestra por salida estandard la marca, marca extendida, familia y modelo del procesador.
temp: Muestra por salida estandard la temperatura. del procesador medida en grados centigrados.
testdivzero: Ejecuta la excepcion 0 de tipo division por cero.
Muestra el funcionamiento de la rutina de atencion de dicha excepcion
testinvopcode: Ejecuta la excepcion 6 de tipo invalid op code.
Muestra el funcionamiento de la rutina de atencion de dicha excepcion.
help: Muestra por salida estandar este panel.
Para ejecutar cualquier comando, se debe presionar la tecla enter al final.
