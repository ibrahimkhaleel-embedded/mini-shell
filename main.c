#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int main()
{
    
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, signal_handler);
    system("clear");
    scan_input();
    return 0;
}
