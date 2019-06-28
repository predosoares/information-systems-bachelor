#include <time.h>
#include <sys/types.h>
#include "algorithms.h"

int LRU( ListOfFrames * mainMemory )
{
    int removedFrame ;

    double  diff_time ,
            lastRecent ;
    
    clock_t now = clock() ;

    for ( int i = 0 ; i < mainMemory->count ; i++ )
    {
        diff_time = (double) ( now - mainMemory->list[i].lastAcess ) ;

        if ( diff_time > lastRecent || i == 0 )
        {
            lastRecent = diff_time ;
            removedFrame = i ;
        }
    }

    return removedFrame ;
}

int NRU( ListOfFrames * mainMemory )
{
    int removedFrame[ 4 ] = { -1 , -1 , -1 , -1 };
    int i;

    for ( i = 0 ; i < mainMemory->count; i++ )
    {
        if ( mainMemory->list[i].flagM == false && mainMemory->list[i].flagR == false )
        {
            removedFrame[0] = i ;
            break ;
        }
        else if ( mainMemory->list[i].flagM == true && mainMemory->list[i].flagR == false )
        {
            removedFrame[1] = i ;
        }
        else if ( mainMemory->list[i].flagM == false && mainMemory->list[i].flagR == true )
        {
            removedFrame[2] = i ;
        } 
        else if  ( mainMemory->list[i].flagM == true && mainMemory->list[i].flagR == true )
        {
            removedFrame[3] = i ;
        }
    }
    
    for ( i = 0 ; i < 4 ; i++ )
    {
        if ( removedFrame[i] == -1 )
        {
            continue ;
        }

        break ;
    }

    return removedFrame[i] ;
}

int NOVO( ListOfFrames * mainMemory )
{
    return 0;
}