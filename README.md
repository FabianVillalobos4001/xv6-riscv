# Lottery Scheduler Implementation for XV6

Este proyecto implementa un planificador por lotería (Lottery Scheduler) en el sistema operativo XV6, reemplazando el planificador Round-Robin original.

## Funcionamiento y Lógica de la Implementación

El planificador por lotería funciona de la siguiente manera:

1. Cada proceso tiene un número de tickets (por defecto 100) que representa su prioridad.
2. En cada ciclo de planificación:
   - Se calcula el total de tickets de todos los procesos RUNNABLE
   - Se genera un número aleatorio entre 1 y el total de tickets
   - Se selecciona el proceso que "posee" el ticket ganador
   - El proceso seleccionado se ejecuta por un quantum

## Modificaciones Realizadas

### Archivos y Cambios Clave

1. `kernel/proc.h`:
   - Agregado campo `tickets` para almacenar los tickets de cada proceso
   - Agregado campo `run_slices` para contabilidad de ejecución

2. `kernel/proc.c`:
   - Modificada función `allocproc()` para inicializar tickets y run_slices
   - Implementado nuevo scheduler con selección por lotería
   - Contabilidad de ejecuciones mediante run_slices

3. `kernel/syscall.h`, `kernel/syscall.c`:
   - Agregada nueva syscall `settickets`

4. `user/settickets.c`:
   - Implementado programa para modificar tickets de un proceso

5. `user/gdemo.c`:
   - Programa de prueba que crea múltiples procesos con diferentes tickets

## Dificultades y Soluciones

1. Generación de números aleatorios:
   - Solución: Utilización del generador pseudo-aleatorio existente

2. Contabilidad precisa:
   - Solución: Incremento de run_slices al momento de selección

3. Sincronización:
   - Solución: Mantenimiento cuidadoso de los locks existentes

## Problemas del Lottery Scheduler

1. No determinista:
   - La naturaleza aleatoria puede llevar a resultados impredecibles
   - Procesos importantes podrían sufrir retrasos inesperados

2. Overhead:
   - Necesidad de calcular total de tickets en cada ciclo
   - Generación de números aleatorios
   - Búsqueda del proceso ganador

3. Granularidad:
   - La asignación de tickets es discreta
   - Difícil ajustar proporciones precisas

4. Inanición posible:
   - Procesos con pocos tickets podrían no ser seleccionados por largo tiempo
   - No hay garantía de progreso

5. Complejidad de ajuste:
   - Difícil determinar la cantidad óptima de tickets
   - El comportamiento puede variar significativamente con diferentes distribuciones

6. Fairness a corto plazo:
   - La justicia solo se garantiza estadísticamente a largo plazo
   - Períodos cortos pueden mostrar gran desbalance