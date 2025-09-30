TAREA 1 INFORME

Fabián Villalobos
Agustín Escobar 

Parte 1: Implementar la llamada al sistema getppid() en xv6 la cual retrona el ID del proceso padre del proceso que la retorna y realizar pruebas en ejecutable yosoytupadre.c

Para realizar la parte seguimos las instrucciones y recomendaciones que se nos entrego, para esto revisamos en el buscador global (CTRL + SHIFT + F) la palabra getpid() e investigarla como referencia, con esto buscamos el flujo de syscall. Con esto encontramos los siguientes archivos principales: 
    user/user.h
    user/usys.pl
    kernel/syscall.h
    kernel/syscall.c
    kernel/sysproc.c
Decidimos no modificar pero si leer los archivos usertests.c y grind.c (donde tambien se encontro la funcion getpid()) debido a que son tests y stress test de varias funcionalidades de xv6.

Luego de esto implemente la llamada al sistema getppid() en los archivos principales

    kernel/syscall.h agregamos #define SYS_getppid 22 // EL 22 DEBIDO A SER EL SIGUIENTE NUMERO DISPONIBLE
    kernel/syscall.c declaramos 
        extern uint64 sys_getppid(void);

        static uint64 (*syscalls[])(void) = {
           // DESPUES DE LAS OTRAS LLAMADAS AL SISTEMA
            [SYS_getppid] sys_getppid,
        };
    kernel/sysproc.c
        uint64
        sys_getppid(void)
        {
            struct proc *p = myproc();
            return p->parent ? p->parent->pid : -1;
        }
    user/user.h agregamos
        int getppid(void);
    user/usys.pl
        entry("getppid");

Luego de implementar los cambios en los respectivos archivos donde se encontraba getpid() desarrollamos el ejecutable yosoytupadre.c el cual utiliza fork() y wait() para confirmar el comportamiento de el PID del hijo que devuelve getppid().

Después se modifico el Makefile agregando el archivo yosoytupadre al final de este apartado: UPROGS = $U/_yosoytupadre
	                                                                                                

Finalmente se reviso el funcionamiento con make qemu obteniendo: 
$ yosoytupadre
[pre-fork] pid=2 ppid=2
[hijo]    pid=4 ppid=4
[padre]   pid=2 ppid=2 (esperé a 5)

Este funcionamiento fue uno de los problemas que obtuvimos ya que tanto pid como ppid retornan lo mismo por lo que decidimos modificar getpid() en sysproc.c obteniendo la siguiente salida:
$ yosoytupadre
[pre-fork] pid=3 ppid=2
[hijo]    pid=4 ppid=3
[padre]   pid=3 ppid=2 (esperé a 4)

Esta salida nos indica que ppid del hijo (4) es el pid del padre (3)
                            ppid del padre (3) es 2

Parte 2: Implementación avanzada getancestor(int) 

Para la parte 2 replicamos los pasos de la parte 1 aplicando exactamente las mismas modificaciónes con excepcion de la logica de sysproc.c donde colocamos:
    
                                                                                                                                                                uint64
                                                                                                                                                            sys_getancestor(void)
                                                                                                                                                            {
                                                                                                                                                            int n;
                                                                                                                                                            argint(0, &n);         
                                                                                                                                                            if(n < 0)
                                                                                                                                                                return -1;

                                                                                                                                                            struct proc *p = myproc();
                                                                                                                                                            while(n > 0) {
                                                                                                                                                                if(p->parent == 0)   
                                                                                                                                                                return -1;
                                                                                                                                                                p = p->parent;
                                                                                                                                                                n--;
                                                                                                                                                            }
                                                                                                                                                            return p->pid;          

Y tambien modificamos el archivo de prueba yosoytupadre.c para que acepte la nueva logica obteniendo: $ yosoytupadre
[pre-fork] pid=3 ppid=2 anc0=3 anc1=2 anc99=-1
[hijo]    pid=4 ppid=3 anc0=4 anc1=3 anc2=2 anc99=-1
[padre]   pid=3 ppid=2 anc0=3 anc1=2 (esperé a 4)
                                                                                                                                                




