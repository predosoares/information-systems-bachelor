#include <time.h>
#include <stdbool.h>

#include "sim-virtual.h"
#include "futureAccesses.h"
#include "algorithms.h"

#define NOT_IN_MAIN_MEMORY -1

int LRU( ListOfFrames * mainMemory , tpListOfVirtualPages * ListOfVirtualPages )
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

    ListOfVirtualPages->list[mainMemory->list[removedFrame].virtualPageIndex] = NOT_IN_MAIN_MEMORY ;

    return removedFrame ;
}

int NRU( ListOfFrames * mainMemory , tpListOfVirtualPages * ListOfVirtualPages )
{
    int removedFrame[ 4 ] = { -1 , -1 , -1 , -1 };
    int i;

    for ( i = 0 ; i < mainMemory->count ; i++ )
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

    ListOfVirtualPages->list[mainMemory->list[removedFrame[i]].virtualPageIndex] = NOT_IN_MAIN_MEMORY ;

    return removedFrame[i] ;
}

int NOVO( void )
{
    return leastAccesses();
}