# Reflexión y evocación

## 1. 

Con la función del Ejercicio 1 todo es cómodo: hay fórmula, se puede evaluar en cualquier punto, y hasta se puede encontrar la primitiva. Eso permite conocer el valor exacto de la integral y medir el error real de cada método.

Con los datos del Ejercicio 2 eso se pierde. Solo hay 50 pares (x, y), y entre un dato y el siguiente no se sabe qué pasa. Suponer que la función es suave es una apuesta, no un hecho. No hay primitiva, no hay derivadas analíticas, y no hay forma de calcular el error real: solo queda comparar métodos entre sí y confiar en que si dos de distinto orden coinciden, el resultado es razonable.

Eso obliga a cambiar la estrategia. El paso h ya está fijado por el muestreo; no se puede reducir. No tiene sentido pedirle a un integrador una precisión de 1e-12 cuando los datos de base tienen 4 cifras. Y la elección del método cambia: Simpson es mejor con funciones suaves, pero el trapecio es más noble con datos ruidosos, porque no amplifica las variaciones bruscas. En resumen, al pasar de función a datos se pierde control, pero se gana realismo: así se trabaja en ingeniería de verdad.

---

## 2. 

Primero formulamos cada problema y pensamos qué lo complicaba: en el Ejercicio 1, la oscilación del seno; en el Ejercicio 2, tener solo 50 datos.

Después hicimos todo en Octave. Implementamos el trapecio a mano, lo comparamos con trapz y usamos quad como adaptativo. Verificamos que nuestro trapecio coincidiera exactamente con trapz, lo cual confirma que la fórmula está bien. Luego pasamos a C++ con GSL: el trapecio fue directo, pero integrar con spline cúbica requirió armar gsl_spline, el acelerador y liberar memoria. Finalmente en Python con SciPy todo fue más limpio; lo único que nos costó fue manejar el intervalo impar de Simpson, porque con 50 datos hay 49 intervalos. Lo resolvimos aplicando Simpson en los primeros 48 y trapecio en el último.

Lo más valioso fue la verificación cruzada. Cuando los tres entornos dieron exactamente 21.1908459000 para el trapecio, supimos que las implementaciones estaban bien. Esa coincidencia es una forma muy simple y muy fuerte de verificar el trabajo.

Con la diferenciación pasó algo interesante. Al agregar ruido, la integral apenas se movió pero la derivada se volvió un caos. Después entendimos: integrar es sumar, y los errores aleatorios se cancelan parcialmente. Derivar es restar y dividir por h, y eso amplifica cualquier variación. Es la misma razón por la que es más fácil medir el caudal acumulado de un río que su velocidad instantánea.

Nos quedamos con tres ideas: no existe un método mejor en abstracto, depende del problema; verificar con varios métodos no es opcional cuando no hay valor verdadero; y entender el método importa más que saber usar la biblioteca.