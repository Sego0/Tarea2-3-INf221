# Tarea2-3-INf221
En este repositorio se encontraran los algoritmos usados para las pruebas realizadas en el informe. El archivo "EDbrute" contiene el codigo usado para para el algoritmo de minima distancia de edicion con paradigma de fuerza bruta, mientras que el archivo "EDdynamic" contiene el algoritmo con enfoque de programacion dinamica. Luego en la carpeta output encontraremos los datasets usados para los resultados del informe, estos estan generados con el archivo llamado "generador" y tiene la siguiente forma "<caso>_strings_<tamaño>.txt".

Para la ejecucion de cualquiera de los 2 algortimos deberemos...
1.  Dentro de "main" dirigirnos y eleguir el tamaño de entrada que estudiaremos segun el nombre de los archivos donde guardaremos los casos (puede ser 10, 100 o 1000).
2.  Luego, dentro del ciclo "while" eleguiremos el caso a estudiar en las variable st1 y str2 (los casos pueden ser similares, repetidas, transposiciones o distintas)
3.  Por ultimo, si queremos estudiar cada iteracion por si misma se debera quitar el ciclo "while" para luego ir avanzado de iteracion manualmente mediante la variable "n".
