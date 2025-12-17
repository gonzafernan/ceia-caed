#import "@preview/ieee-monolith:0.1.0": ieee

#show: ieee.with(
  title: [Computación, algoritmos y estructuras de datos],
  authors: (
    (
      name: "Gonzalo G. Fernandez",
      department: [Universidad de Buenos Aires],
      organization: [Laboratorio de Sistemas Embebidos],
      email: "fernandez.gfg@gmail.com",
    ),
  ),
  index-terms: (
    //   "Scientific writing",
    //   "Typesetting",
    //   "Document creation",
    //   "Syntax",
  ),
  bibliography: bibliography("refs.bib"),

  global-font: ("Charter", "Hiragino Kaku Gothic Interface"), // Choose Fonts used globally. For multiple languages, you may want to use more than 2.
)


// #outline(indent: true) // Show Table of Contents
#set page(numbering: "1 / 1") // Add page number at the bottom

= Problemas de máquinas de Turing
Se implementó una máquina de Turing que suma dos números en binario. La simulación se realizó con https://turingmachinesimulator.com/. Dado que en la página ya existe un ejemplo de una máquina que suma dos números binarios utilizando tres cintas, se realizó una implementación de una máquina con el mismo objetivo pero que solo utiliza dos cintas. La implementación se puede observar en el archivo adjunto a este documento `turing/turing_2tape-add_binary_1.txt`. En el archivo `turing/turing_2tape-add_binary.webm` se puede observar un video con tres sumas binarias distintas simuladas en la página mencionada previamente.

= Problemas de algoritmo tipo divide y vencerás
Sean A y B dos matrices de tamaño $n times n$ ¿Cuál es la complejidad en tiempo del producto usual en términos de n? Encuentre un algoritmo tipo divide y vencerás que calcule el producto entre dos matrices cuadradas de tamaño $n times n$ en tiempo de menor al algoritmo usual para el producto de matrices. Determine su complejidad en tiempo y en memoria. Implemente dicho algoritmo en su lenguaje de programación favorito con 3 productos de ejemplo de matrices de tamaño $10 times 10$.

== Complejidad temporal del producto usual de matrices
Sean dos matrices cuadradas A y B de tamaño $n times n$, el elemento $c_(i,j)$ de la matriz $C = A dot B$ para $i, j = 1, 2, ..., n$ se calcula como:

$
  c_(i, j) = sum_(k=1)^(n) a_(i, k) dot b_(k, j)
$ <eq:square-matrix-multiply>

Por lo tanto, dado que la matriz C también es de tamaño $n times n$, la ecuación @eq:square-matrix-multiply se repetirá $n^2$ veces y la complejidad de la operación es $O(n^3)$.

== Algoritmo de Strassen para multiplicación de matrices
El algoritmo de Strassen consta de cuatro pasos@cormen_introduction_2009:

1. Dividir las matrices de entrada A y B y la matriz de salida C en $n/2 times n/2$ submatrices como se muestra en la ecuación @eq:n_over_2_submatrices.

$
  A = mat(A_(1, 1), A_(1, 2); A_(2, 1), A_(2, 2)), B = mat(B_(1, 1), B_(1, 2); B_(2, 1), B_(2, 2)), C = mat(C_(1, 1), C_(1, 2); C_(2, 1), C_(2, 2))
$ <eq:n_over_2_submatrices>

Donde cada submatriz de C se calcula con las ecuaciones @eq:strassen-c-submatrices.

$
  C_(1, 1) = A_(1, 1) dot B_(1, 1) + A_(1, 2) dot B_(2, 1), \
  C_(1, 2) = A_(1, 1) dot B_(1, 2) + A_(1, 2) dot B_(2, 2), \
  C_(2, 1) = A_(2, 1) dot B_(1, 1) + A_(2, 2) dot B_(2, 1), \
  C_(2, 2) = A_(2, 1) dot B_(1, 2) + A_(2, 2) dot B_(2, 2), \
$ <eq:strassen-c-submatrices>

La complejidad de este paso es $O(1)$ si se realiza cálculo de índices, es decir, identificando cada submatriz como un rango de columnas y un rango de filas de la matriz original.

2. Crear 10 matrices $S_1, S_2, ..., S_(10)$ de tamaño $n/2 times n/2$ donde cada una es la suma o diferencia de dos matrices creadas en el paso 1. La complejidad de este paso es $O(n^2)$.

3. Utilizando las submatrices creadas en el paso 1 y las 10 matrices creadas en el paso 2, se calcula recursivamente los siete productos $P_1, P_2, ..., P_7$ de tamaño $n/2 times n/2$.

4. Las matrices $C_(1, 1), C_(1, 2), C_(2, 1), C_(2, 2)$ se calculan sumando o restando diferentes combinaciones de las matrices $P_i$. Las cuatro matrices se pueden calcular en tiempo $O(n^2)$.

Las matrices del paso 2 son las siguientes:

$
  S_1 = B_(1, 2) - B_(2, 2), \
  S_2 = A_(1, 1) + A_(1, 2), \
  S_3 = A_(2, 1) + A_(2, 2), \
  S_4 = B_(2, 1) - B_(1, 1), \
  S_5 = A_(1, 1) + A_(2, 2), \
  S_6 = B_(1, 1) + B_(2, 2), \
  S_7 = A_(1, 2) - A_(2, 2) \
  S_8 = B_(2, 1) + B_(2, 2) \
  S_9 = A_(1, 1) - A_(2, 1) \
  S_(10) = B_(1, 1) + B_(1, 2) \
$

Como se suman $n/2 times n/2$ matrices 10 veces, el paso 2 tiene complejidad $O(n^2)$.

En el paso 3 se multiplica recursivamente $n/2 times n/2$ matrices siete veces para calcular las $n/2 times n/2$ matrices de la ecuación @eq:strassen-step3, cada una suma o diferencia de productos de las submatrices de A y B:

$
  P_1 = A_(1, 1) dot S_1 = A_(1, 1) dot B_(1, 2) - A_(1, 1) dot B_(2, 2), \
  P_2 = S_2 dot B_(2, 2) = A_(1, 1) dot B_(2, 2) + A_(1, 2) dot B_(2, 2), \
  P_3 = S_3 dot B_(1, 1) = A_(2, 1) dot B_(1, 1) + A_(2, 2) dot B_(1, 1), \
  P_4 = A_(2, 2) dot S_4 = A_(2, 2) dot B_(2, 1) - A_(2, 2) dot B_(1, 1), \
  P_5 = S_5 dot S_6 = A_(1, 1) dot B_(1, 1) + A_(1, 1) dot B_(2, 2) + A_(2, 2) dot B_(1, 1) + A_(2, 2) dot B_(2, 2), \
  P_6 = S_7 dot S_8 A_(1, 2) dot B_(2, 1) + A_(1, 2) dot B_(2, 2) - A_(2, 2) dot B_(2, 1) - A_(2, 2) dot B_(2, 2), \
  P_7 = S_9 dot S_(10) = A_(1, 1) dot B_(1, 1) + A_(1, 1) dot B_(1, 2) - A_(2, 1) dot B_(1, 1) - A_(2, 1) dot B_(1, 2) \
$ <eq:strassen-step3>

En el paso 4 se adiciona las matrices $P_i$ creadas en el paso 3 para construir las cutras submatrices de C de tamaño $n/2 times n/2$ como se observa en las ecuaciones @eq:strassen-c-submatrices-with-p.

$
  C_(1, 1) = P_5 + P_4 - P_2 + P_6 = A_(1, 1) dot B_(1, 1) + A_(1, 2) dot B_(2, 1), \
  C_(1, 2) = P_1 + P_2 = A_(1, 1) dot B_(1, 2) + A_(1, 2) dot B_(2, 2), \
  C_(2, 1) = P_3 + P_4 = A_(2, 1) dot B_(1, 1) + A_(2, 2) dot B_(2, 1), \
  C_(2, 2) = P_5 + P_1 - P_3 - P_7 = A_(2, 2) dot B_(2, 2) + A_(2, 1) dot B_(1, 2)
$ <eq:strassen-c-submatrices-with-p>

Se puede observar cómo las ecuaciones @eq:strassen-c-submatrices-with-p corresponden con las ecuaciones @eq:strassen-c-submatrices.

El análisis de tiempo de ejecución del algoritmo expuesto puede resumirse en la ecuación @eq:strassen-running-time.

$
  T(n) = cases(
    O(1) "si" n = 1,
    7 T(n/2) + O(n^2) "si" n > 1,
  )
$ <eq:strassen-running-time>

El algoritmo reemplaza la multiplicación de matrices por adiciones de un número constante de matrices. La complejidad del algoritmo de Strassen es $O(n^(lg 7))$, dado que $lg 7$ es un valor entre 2.80 y 2.81, la complejidad es $O(n^(2.81))$.

= Alineación de secuencias genéticas mediante el algoritmo de Needleman-Wunsch
== Definición de una secuencia de nucleótidos
Una *secuencia de nucleótidos* es el orden específico de los componentes básicos del ADN o ARN. Estos componentes son moléculas formadas por una base nitrogenada, un azúcar y un grupo fosfato. En el ADN, las bases son adenina (A), timina (T), citosina (C) y guanina (G); en el ARN, la timina se sustituye por uracilo (U).

La secuencia de nucleótidos codifica información genética: determina qué proteínas se sintetizan y cómo se regula su producción. Cambios en la secuencia (mutaciones) pueden modificar funciones celulares, provocar enfermedades o generar variación entre organismos@bocsci_nucleotide_sequence.

== Comparación de secuancias de nucleótidos
La comparación de secuencias de nucleótidos consiste en alinear dos o más secuencias para identificar regiones similares o diferentes. Esta comparación es crucial para:

- Inferir relaciones evolutivas: las especies con secuencias muy similares comparten un ancestro en común más reciente que las que son más diferentes.
- Identificar genes y funciones: si una secuencia desconocida es similar a otra con función conocida, podemos inferir la función de la nueva.
- Detectar mutaciones: comparar secuencia de individuos sanos y enfermos permite encontrar variantes asociadas con enfermedades genéticas.
- Diagnóstico y epidemiología: en patología molecular y vigilancia de epidemias (como SARS-CoV-2), la comparación rápida de genomas virales ayuda a rastrear variantes y su propagación.

== Alineación de secuencias de nucleótidos
Una alineación de secuencias es el proceso de colocar dos o más secuencias biológicas (de ADN, ARN o proteínas) una debajo de la otra, introduciendo espacios cuando es necesario, para identificar similitudes y diferencias entre ellas@posada_bioinformatics_2009.

El objetivo es resaltar regiones homólogas que puedan indicar relación evolutiva, estructural o funcional.

La *alineación global* intenta alinear las secuencias completas, desde el primer hasta el último elemento.

Es adecuada cuando las secuencias:
- Tienen longitudes similares
- Son globalmente similares
- Penaliza fuertemente las diferencias en cualquier parte de la secuencia.

El algoritmo clásico utilizado para este tipo de alineación es _Needleman–Wunsch_.

La *alineación local* busca las regiones más similares entre dos secuencias, sin obligar a alinear toda su longitud.

Es adecuada cuando:
- Las secuencias tienen longitudes diferentes
- Solo comparten fragmentos conservados
- Ignora regiones no similares en los extremos o en el medio.

El algoritmo clásico utilizado para este tipo de alineación es _Smith–Waterman_.

Durante una alineación, cada posición puede clasificarse como:
- Coincidencias (_matches_): ocurren cuando los símbolos alineados son idénticos.
- Desajustes (_mismatches_): ocurren cuando los símbolos alineados son diferentes.
- Huecos (_gaps_): Son espacios insertados en una secuencia para mejorar la alineación con la otra.

En escencia, el proceso de alineación es la optimización de una puntuación que equilibra:
- Maximizar coincidencias
- Minimizar desajustes
- Minimizar huecos


== Algoritmo de Needleman–Wunsch

El algoritmo de *Needleman–Wunsch* es un algoritmo clásico de alineamiento global de secuencias, ampliamente utilizado en bioinformática para alinear ADN, ARN o proteínas. Su objetivo es encontrar el alineamiento óptimo considerando toda la longitud de ambas secuencias@needleman_general_1970@kleinberg_algorithm_2006@posada_bioinformatics_2009.

=== Construcción de la matriz de programación dinámica

Sean dos secuencias:

$ X = x_1, x_2, dots, x_n $
$ Y = y_1, y_2, dots, y_m $

Se construye una matriz de programación dinámica $F$ de tamaño $(n + 1) times (m + 1)$, donde cada elemento:

$ F(i, j) $

representa el mejor puntaje posible para alinear los prefijos:

- $x_1 dots x_i$
- $y_1 dots y_j$

1. *Inicialización:*

Las condiciones iniciales son:

$ F(0, 0) = 0 $
$ F(i, 0) = i * g $
$ F(0, j) = j * g $

donde $g$ es la penalización por *gap*.

Estas condiciones reflejan que alinear una secuencia no vacía con una secuencia vacía implica únicamente inserciones o eliminaciones.

2. *Ecuación de recurrencia:*

Para cada celda $F(i, j)$ se consideran tres posibles transiciones:

- *Match / mismatch*: alinear $x_i$ con $y_j$
- *Gap en Y*: alinear $x_i$ con un gap
- *Gap en X*: alinear un gap con $y_j$

La ecuación de recurrencia es:

$
  F(i, j) = max(
    F(i - 1, j - 1) + s(x_i, y_j),
    F(i - 1, j) + g,
    F(i, j - 1) + g
  )
$

donde:

- $s(x_i, y_j)$ es la función de puntuación (match o mismatch)
- $g$ es la penalización por gap

Cada celda almacena el puntaje máximo y un puntero hacia la celda desde la cual se obtuvo dicho máximo.

=== Proceso de traceback

Una vez completada la matriz, el puntaje óptimo global se encuentra en la celda:

$ F(n, m) $

El alineamiento óptimo se recupera mediante un proceso de *traceback* que comienza en $(n, m)$ y finaliza en $(0, 0)$.

En cada celda $(i, j)$:

- Si $F(i, j) = F(i - 1, j - 1) + s(x_i, y_j)$ entonces:
  - se alinean $x_i$ y $y_j$
  - se avanza a $(i - 1, j - 1)$

- Si $F(i, j) = F(i - 1, j) + g$ entonces:
  - se alinea $x_i$ con un gap
  - se avanza a $(i - 1, j)$

- Si $F(i, j) = F(i, j - 1) + g$ entonces:
  - se alinea un gap con $y_j$
  - se avanza a $(i, j - 1)$

El alineamiento se construye en orden inverso y debe invertirse al finalizar el proceso.

=== Complejidad
- La complejidad temporal es $O(n times m)$.
- La complejidad espacial es $O(n times m)$.

== Implementación

En el archivo `needleman_wunsch.py` se puede observar una implementación en Python del algoritmo en estudio. En el archivo `needleman_wunsch_output.txt` se puede observar la salida obtenida al ejecutar el programa.
