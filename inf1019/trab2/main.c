#include <stdio.h>
#include <stdlib.h>
#include "sim-virtual.h"

int main( int argc, char * argv[] )
{
    if ( argc < 6 )
    {
        puts("How to run: ./sim-virtual <algoritm> <file.log> <frameSize in KB> <mainMemorySize in KB> -D");
        exit( -1 ) ;
    }

    simulaMemoriaVirtual(argv[1], argv[2], atoi(argv[3]) , atoi(argv[4]), argv[5] );

    return 0 ;
}