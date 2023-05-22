# Ejemplo de uso de pipes.

## Ejemplo 1. Normalización de matrices.
Se normaliza una matriz entrada con la siguiente formula:
```
x' = (x - min(x)) / (max(x) - min(x))
```
Donde x es un elemento de la matriz de entrada y x' es el elemento de la matriz de salida.

Para compilar el programa se debe ejecutar el siguiente comando:
```
gcc -o matrix_normalizer matrix_normalizer.c
```
Para ejecutar el programa se debe ejecutar el siguiente comando:
```
./matrix_normalizer -f 3 -c 3 -p matrix.txt
```
Donde:
* -f: Número de filas de la matriz.
* -c: Número de columnas de la matriz.
* -p: Ruta del archivo de la matriz de entrada.

## Ejemplo 2. Contador de palabras.
Este programa cuenta la cantidad de palabras que hay en un archivo de texto.
Para compilar el programa se debe ejecutar el siguiente comando:
```
gcc -o word_counter word_counter.c
```
Para ejecutar el programa se debe ejecutar el siguiente comando:
```
./word_counter input.txt count
```
Donde:
* input.txt: Ruta del archivo de texto.
* count: Palabra a contar.
