#include <stdlib.h>
#include <stdio.h>
#include "processor.h"
int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Enter file's name to run it");
        return -1;
    }

    Processor proc;

    proc.Run(argv[1]);
    
    return 0;
}