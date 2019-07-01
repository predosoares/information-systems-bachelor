#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim-virtual.h"

int main( int argc, char * argv[] )
{
    char mode[3];

    if ( argc < 5 )
    {
        puts("How to run: ./sim-virtual <algoritm> <file.log> <frameSize in KB> <mainMemorySize in KB> [ -D ]");
        exit( -1 ) ;
    }

    if ( argc == 5 )
    {
        strcpy( mode , "-N" ) ;
    } else
    {
        strcpy( mode , argv[5] ) ;
    }

    simulaMemoriaVirtual(argv[1], argv[2], atoi(argv[3]) , atoi(argv[4]), mode );

    return 0 ;
}