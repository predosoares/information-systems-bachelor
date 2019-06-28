#include <unistd.h>
#include <assert.h>
#include "algorithms.h"

#define DEBUG if(isDebugging())

#define NOT_IN_MAIN_MEMORY -1
#define SIZE_MEMORY_ADRESS 32

static bool debbugingMode = false;

bool isDebugging( void )
{
    return debbugingMode;
}

void inicializeMainMemory( ListOfFrames * const mainMemory, const unsigned int numFrames )
{
    mainMemory->list = ( Frame * ) malloc ( numFrames * sizeof(Frame) ) ;
    mainMemory->count = 0 ;
}

void inicializeListOfVirtualPages( tpListOfVirtualPages * const ListOfVirtualPages , const unsigned int numVirtualPages )
{
    ListOfVirtualPages->maxVirtualPages = numVirtualPages ;

    ListOfVirtualPages->list = ( VirtualPage * ) malloc ( numVirtualPages * sizeof( VirtualPage ) ) ;

    for ( int i = 0 ; i < ListOfVirtualPages->maxVirtualPages ; i++ )
    {
        ListOfVirtualPages->list[i] = NOT_IN_MAIN_MEMORY ;
    }
}

void allocNewFrame( Frame * const ListOfFrames, const int frameIndex , const int virtualPagaIndex )
{
    ListOfFrames[frameIndex].flagR = false ;
    ListOfFrames[frameIndex].flagM = false ;
    ListOfFrames[frameIndex].lastAcess = clock( ) ;
    ListOfFrames[frameIndex].virtualPageIndex = virtualPagaIndex ;
}

void validateFramePageSize( const unsigned int frameSize )
{
    if ( frameSize < 8 || frameSize > 32 )
    {
        puts ("Size of page frame invalid! Try supported values: 8 to 32 KB") ;
        exit ( -1 ) ;
    }
}

void validateMainMemorySize( const unsigned int mainMemorySize )
{
    if ( mainMemorySize < 1024 || mainMemorySize > 16384 )
    {
        puts ("Size of main memory invalid! Try supported values: 1024 to 16384 KB") ;
        exit ( -1 ) ;
    }
}

int simulaMemoriaVirtual( const char * algoritmo, const char * arquivo, unsigned int frameSize, unsigned int mainMemorySize, const char * mode )
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

    frameSize *= 1024 ;

    validateMainMemorySize( mainMemorySize ) ;

    mainMemorySize *= 1024 ;

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
                    frameIndex = LRU( &mainMemory , &ListOfVirtualPages ) ;
                }
                else if ( strcmp( "NRU" , algoritmo ) == 0 )
                {
                    frameIndex = NRU( &mainMemory , &ListOfVirtualPages ) ;
                }
                else if ( strcmp( "NOVO" , algoritmo ) == 0 )
                {
                    frameIndex = NOVO( &mainMemory ) ;
                }
                else
                {
                    puts("Chosen algorithm do no exist!") ;
                    exit( -1 ) ;   
                }
            }
            else // Main memory is not full yet
            {
                frameIndex = mainMemory.count ;
                mainMemory.count++ ;
            }

            allocNewFrame( mainMemory.list , frameIndex , pageIndex ) ;
            ListOfVirtualPages.list[pageIndex] = frameIndex ;
        }
        else // Process is already in the memory
        {
            frameIndex = ListOfVirtualPages.list[pageIndex] ;

            mainMemory.list[frameIndex].flagR = rw == 'R' ? true : mainMemory.list[frameIndex].flagR ;
            mainMemory.list[frameIndex].flagM = rw == 'W' ? true : mainMemory.list[frameIndex].flagM ;
            mainMemory.list[frameIndex].lastAcess = clock( ) ;
        }

        if ( rw == 'W' ) { pagesWritedInDisc++ ; }

        if ( resetAlarm == 1000 )
        {
            for (int i = 0; i < mainMemory.count ; i++ )
            {
                mainMemory.list[frameIndex].flagR = false ;
                mainMemory.list[frameIndex].flagM = false ;
            }

            resetAlarm = 1 ;
        }

        DEBUG ;

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