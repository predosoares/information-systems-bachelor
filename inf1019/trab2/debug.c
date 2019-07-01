#include <stdbool.h>
#include <string.h>

#include "debug.h"

bool debbugingMode = false ;

bool isDebuggingMode( void )
{
    return debbugingMode;
}

void checkIfDebuggingModeIsOn( const char * mode )
{
    if ( strcmp( "-D" , mode ) == 0 )
    {
        debbugingMode = true ;
    }
}
