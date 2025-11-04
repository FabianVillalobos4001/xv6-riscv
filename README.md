# Implementación de Lottery Scheduler


Implementación de Lottery Scheduler en Xv6, reemplazando el planificador Round-Robin original.


## Funcionamiento y Lógica


1. Cada proceso tiene un número de tickets (por defecto 100) que representa su prioridad.
2. En cada ciclo de planificación:
   - Se calcula el total de tickets de todos los procesos RUNNABLE
   - Se genera un número aleatorio entre 1 y el total de tickets
   - Se selecciona el proceso que "posee" el ticket ganador
   - El proceso seleccionado se ejecuta por un quantum


## Modificaciones que Realizamos


`kernel/proc.h`:
   - Agregamos el campo `tickets` para almacenar los tickets de cada proceso
   - Agregamos el campo `run_slices` para contabilidad de ejecución


`kernel/proc.c`:
   - Modificamos la función `allocproc()` para inicializar tickets y run_slices
   - Implementamos el nuevo scheduler con selección por lotería
   - Se implementó el uso de contabilidad de ejecuciones mediante run_slices


`kernel/syscall.h`, `kernel/syscall.c`:
   - Se agregó la llamada al sistema `settickets`


`user/settickets.c`:
   - Implementado programa para modificar tickets de un proceso


`user/gdemo.c`:
   - Este es el programa de prueba que crea múltiples procesos con diferentes tickets


## Dificultades enfrentadas y sus Soluciones


Generación de números aleatorios:
   - Solución: Utilización del generador pseudo-aleatorio existente


Contabilidad precisa:
   - Solución: Incremento de run_slices al momento de selección


Sincronización:
   - Solución: Mantenimiento cuidadoso de los locks existentes


## Problemas del Lottery Scheduler


No determinista:
   - La naturaleza aleatoria puede llevar a resultados impredecibles
   - Procesos importantes podrían sufrir retrasos inesperados


Overhead:
   - Necesidad de calcular total de tickets en cada ciclo
   - Generación de números aleatorios
   - Búsqueda del proceso ganador
	
Granularidad:
   - La asignación de tickets es discreta
   - Difícil ajustar proporciones precisas


Inanición posible:
   - Procesos con pocos tickets podrían no ser seleccionados por largo tiempo
   - No hay garantía de progreso


Complejidad de ajuste:
   - Difícil determinar la cantidad óptima de tickets
   - El comportamiento puede variar significativamente con diferentes distribuciones


Fairness a corto plazo:
   - La justicia solo se garantiza estadísticamente a largo plazo
   - Períodos cortos pueden mostrar gran desbalance
