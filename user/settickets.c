#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(2, "Usage: settickets number_of_tickets\n");
        exit(1);
    }
    
    int tickets = atoi(argv[1]);
    settickets(tickets);
    exit(0);
}