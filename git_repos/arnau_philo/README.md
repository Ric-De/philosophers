# 🍝 **Proyecto philosophers** 🧠

**Philosophers** es uno de los proyectos clásicos de 42, centrado en la gestión de concurrencia y sincronización en C mediante hilos y mutexes.  
🧑‍💻 El objetivo es simular el problema de los filósofos comensales, asegurando que los filósofos puedan comer, pensar y dormir sin provocar condiciones de carrera ni deadlocks.

---

## ✨ **Lo que aprendí realizando el proyecto**

Durante el desarrollo de philosophers, adquirí conocimientos clave sobre programación concurrente y sincronización:

- 🧵 **Gestión de hilos (threads)**  
  Aprendí a crear y manejar múltiples hilos con `pthread`, permitiendo la ejecución simultánea de los filósofos.

- 🔒 **Sincronización con mutexes**  
  Utilicé **mutexes** para controlar el acceso a los recursos compartidos (tenedores), evitando condiciones de carrera y garantizando la integridad de los datos.

- 🍽️ **Resolución de deadlocks y starvation**  
  Implementé estrategias para evitar bloqueos y asegurar que todos los filósofos puedan comer sin quedar bloqueados indefinidamente.

- ⏱️ **Gestión de tiempos y estados**  
  Controlé los tiempos de vida, comida y sueño de cada filósofo, así como la detección de muerte por inanición.

- 🛡️ **Gestión de errores y memoria**  
  Manejo correcto de errores, liberación de recursos y memoria dinámica para evitar fugas.

---

## 🏁 **El resultado final**

El resultado es una simulación robusta del problema de los filósofos comensales, donde cada filósofo alterna entre pensar, comer y dormir, respetando las restricciones de concurrencia y sincronización.  
Este proyecto me dio una base sólida para comprender los retos de la programación concurrente y la gestión de recursos compartidos.

---

## ⚡ **Ejemplo de uso**

```sh
./philo <number_of_philos> <time_to_die> <time_to_eat> <time_to_sleep> [number_times_each_philosopher_must_eat]