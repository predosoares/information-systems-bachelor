#include <unistd.h>
#include "algorithms.h"

#define NOT_IN_MAIN_MEMORY -1
#define SIZE_MEMORY_ADRESS 32

void inicializeMainMemory( ListOfFrames * mainMemory, unsigned int numFrames )
{
    mainMemory->list = ( Frame * ) malloc ( numFrames * sizeof(Frame) ) ;
    mainMemory->count = 0 ;
}

void inicializeListOfVirtualPages( tpListOfVirtualPages * ListOfVirtualPages , unsigned int numVirtualPages )
{
    ListOfVirtualPages->maxVirtualPages = numVirtualPages ;

    ListOfVirtualPages->list = ( VirtualPage * ) malloc ( numVirtualPages * sizeof( VirtualPage ) ) ;

    for ( int i = 0 ; i < ListOfVirtualPages->maxVirtualPages ; i++ )
    {
        ListOfVirtualPages->list[i] = NOT_IN_MAIN_MEMORY ;
    }
}

void allocNewFrame( Frame * ListOfFrames, int frameIndex )
{
    ListOfFrames[frameIndex].flagR = 0 ;
    ListOfFrames[frameIndex].flagM = 0 ;
    ListOfFrames[frameIndex].lastAcess = clock() ;
}

void validateFramePageSize( unsigned int frameSize )
{
    if ( frameSize < 8 || frameSize > 32 )
    {
        puts ("Size of page frame invalid! Try supported values: 8 to 32 KB") ;
        exit ( -1 ) ;
    }
}

void validateMainMemorySize( unsigned int mainMemorySize )
{
    if ( mainMemorySize < 1024 || mainMemorySize > 16384 )
    {
        puts ("Size of main memory invalid! Try supported values: 1024 to 16384 KB") ;
        exit ( -1 ) ;
    }
}

int simulaMemoriaVirtual( char * algoritmo, char * arquivo, unsigned int frameSize, unsigned int mainMemorySize, char * mode )
{
    char rw ;
    unsigned int numFrames ,
                 numVirtualPages ,
                 offsetBits ,
                 pageBits ,
                 addr ,
                 pageIndex ,
                 frameIndex ,
                 resetAlarm = 0 ,
                 pageFaultCounter = 0 ,
                 pagesWritedInDisc = 0 ,
                 timeTookCounter = 0 ;

    ListOfFrames mainMemory ;
    tpListOfVirtualPages ListOfVirtualPages ;

    FILE * fd = NULL ;

    validateFramePageSize( frameSize ) ;

    frameSize *= 1024;

    validateMainMemorySize( mainMemorySize ) ;

    mainMemorySize *= 1024;

    offsetBits = ( unsigned int ) ( log10( frameSize ) / log10( 2 ) ) ;

    numFrames = ( unsigned int ) ( mainMemorySize / frameSize ) ;
    inicializeMainMemory( &mainMemory , numFrames ) ;

    pageBits = SIZE_MEMORY_ADRESS - offsetBits ;
    numVirtualPages = pow( 2 , pageBits ) ;
    inicializeListOfVirtualPages( &ListOfVirtualPages , numVirtualPages ) ;

    fd = fopen( arquivo , "r" ) ;

    while( fscanf(fd, "%x %c ", &addr, &rw) != EOF )
    {
        pageIndex = addr >> offsetBits ;

        if ( ListOfVirtualPages.list[pageIndex] == NOT_IN_MAIN_MEMORY )
        {
            pageFaultCounter++ ;

            if ( mainMemory.count == numFrames )
            {
                if ( strcmp( "LRU" , algoritmo ) == 0 )
                {
                    frameIndex = LRU( &mainMemory ) ;
                    
                }
                else if ( strcmp( "NRU" , algoritmo ) == 0 )
                {
                    frameIndex = NRU( &mainMemory ) ;
                }
                else if ( strcmp( "NOVO" , algoritmo ) == 0 )
                {
                    frameIndex = NOVO( &mainMemory ) ;
                }
            }
            else // Main memory is not full yet
            {
                frameIndex = mainMemory.count ;
                mainMemory.count++ ;
            }

            allocNewFrame( mainMemory.list , frameIndex ) ;
            ListOfVirtualPages.list[pageIndex] = frameIndex ;
        }
        else // Process is already in the memory
        {
            mainMemory.list[frameIndex].flagR = rw == 'R' ? true : false ;
            mainMemory.list[frameIndex].flagM = rw == 'W' ? true : false ;
        }

        if ( rw == 'W' ) { pagesWritedInDisc++ ; }

        if ( resetAlarm == 100 )
        {
            for (int i = 0; i < mainMemory.count ; i++)
            {
                mainMemory.list[frameIndex].flagR = false ;
                mainMemory.list[frameIndex].flagM = false ;
            }

            resetAlarm = 1;
        }

        resetAlarm++ ;
        timeTookCounter++ ;
    }

    printf("--------------------------===============--------------------------\n") ;
    printf("Name of file : %s\n", arquivo ) ;
    printf("Used algorithm : %s\n", algoritmo ) ;
    printf("Size of a frame : %d KB\n", frameSize ) ;
    printf("Size of main memory (RAM) : %d KB\n", mainMemorySize ) ;
    printf("Nº of frames : %d \n", numFrames ) ;
    printf("Simulated time took : %d units\n", timeTookCounter ) ;
    printf("Nº of page-faults : %d\n", pageFaultCounter ) ;
    printf("Nº of pages writed in the disc : %d\n", pagesWritedInDisc ) ;
    printf("--------------------------===============--------------------------\n") ;

    return 0;
}