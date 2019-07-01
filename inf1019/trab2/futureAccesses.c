#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "futureAccesses.h"
#include "LISTA.H"

#define NOT_IN_MAIN_MEMORY -1

LIS_tppLista allAccessesToMemory = NULL ;

static void destroyVirtualAddress( void * virtualAddress )
{
    free( virtualAddress ) ;
}

static bool searchForAddress( unsigned int address )
{
    VirtualAddress * curr ;
    int count ;

    IrInicioLista( allAccessesToMemory ) ;
    LIS_NumElemenLista( allAccessesToMemory , &count ) ;

    for ( int i = 0; i < count ; i++ )
    {
        curr = ( VirtualAddress * ) LIS_ObterValor( allAccessesToMemory ) ;
    
        if ( curr->virtualPage == address )
        {
            return true ;
        }
        
        LIS_AvancarElementoCorrente( allAccessesToMemory , count ) ;
    }
    
    return false ;
}

int leastAccesses( void )
{
    VirtualAddress * curr ;
    int count , removedFrame , leastNumberOfAccesses = RAND_MAX ;

    IrInicioLista( allAccessesToMemory ) ;
    LIS_NumElemenLista( allAccessesToMemory , &count ) ;

    for ( int i = 0; i < count; i++ )
    {
        curr = ( VirtualAddress * ) LIS_ObterValor( allAccessesToMemory ) ;
    
        if ( curr->frameIndex != NOT_IN_MAIN_MEMORY && curr->acesses < leastNumberOfAccesses  )
        {
            leastNumberOfAccesses = curr->acesses ;
            removedFrame = curr->frameIndex ;
        }
    }

    curr->frameIndex = NOT_IN_MAIN_MEMORY ;

    if ( curr->acesses == -1 )
    {
        LIS_ExcluirElemento( allAccessesToMemory ) ;
    }

    return removedFrame ;
}

void setFrameIndex( unsigned int frameIndex , unsigned int pageIndex )
{
    VirtualAddress * curr ;

    searchForAddress(  pageIndex ) ;

    curr =  ( VirtualAddress * ) LIS_ObterValor( allAccessesToMemory ) ;
    curr->frameIndex = frameIndex ;
}

void decrementAcesses( unsigned int address )
{
    VirtualAddress * curr ;
    searchForAddress( address ) ;
    curr =  ( VirtualAddress * ) LIS_ObterValor( allAccessesToMemory ) ;
    curr->acesses--;
}



void storeAllTheFutureAccesses( FILE * fd  , unsigned int offsetBits  )
{
    VirtualAddress * aux , * address = ( VirtualAddress * ) malloc ( sizeof( VirtualAddress ) ) ;
    unsigned int addr ;
    char rw ;

    allAccessesToMemory = LIS_CriarLista( destroyVirtualAddress ) ;

    while( fscanf(fd, "%x %c ", &addr, &rw) != EOF )
    {
        address->virtualPage = addr >> offsetBits ;

        if ( !searchForAddress( address->virtualPage ) )
        {
            address->acesses = 0 ;
            address->frameIndex = NOT_IN_MAIN_MEMORY ;
            LIS_InserirElementoApos( allAccessesToMemory , address ) ;
        } else {
            aux =  ( VirtualAddress * ) LIS_ObterValor( allAccessesToMemory ) ;
            aux->acesses ++ ;
        }
    }

    rewind( fd ) ;
}