#if ! defined( FUTUREACCESSES_ )
#define FUTUREACCESSES_ 

#include <stdbool.h>
#include <stdio.h>

typedef struct virtualaddress {
    
    unsigned int virtualPage ;

    unsigned int frameIndex ;

    int acesses ;

} VirtualAddress ;

int leastAccesses( void ) ;

void setFrameIndex( unsigned int frameIndex , unsigned int pageIndex ) ;

void decrementAcesses( unsigned int address ) ;

void storeAllTheFutureAccesses( FILE * fd  , unsigned int offsetBits  ) ;

#endif