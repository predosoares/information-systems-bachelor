/***************************************************************************
*  $MCI Módulo de implementação: SIM  Simulador de Memória Virtual
*
*  Arquivo gerado:              sim-virtual.c
*  Letras identificadoras:      SIM
*
*  Nome da base de software:    Simulação de Paginação e Algoritmos de Substitição
*                               de Páginas Redigidos em C
*
*  Projeto: INF1019 Simulador de Algoritmo de Substituição de Páginas em C
*  Autores: phs - Pedro Henrique Soares
*           jr  - Jonny Russo
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data      Observações
*     1.2.0    phs   01/jul/2019 implementação do algoritmo NOVO
*     1.1.0    jr    29/jun/2019 adição de algoritmos de substituição NRU e LRU
*     1.0.0 phs e jr 28/jun/2019 início desenvolvimento
*
***************************************************************************/
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "debug.h"
#include "sim-virtual.h"
#include "algorithms.h"
#include "futureAccesses.h"

#define DEBUG if ( isDebuggingMode( ) )

#define NOT_IN_MAIN_MEMORY -1
#define SIZE_MEMORY_ADRESS 32
#define CLEAR_INTERVAL 10000

/***** Protótipos das funções encapuladas no Módulo *****/

static void inicializeMainMemory( ListOfFrames * const mainMemory,
                                  const unsigned int numFrames    ) ;

static void destroyMainMemory( ListOfFrames * const mainMemory ) ;

static void inicializeListOfVirtualPages( tpListOfVirtualPages * const ListOfVirtualPages ,
                                          const unsigned int numVirtualPages               ) ;

static void destroyListOfVirtualPages( tpListOfVirtualPages * const ListOfVirtualPages ) ;

static void allocNewFrame( Frame * const ListOfFrames ,
                           const int frameIndex ,
                           const int virtualPagaIndex ,
                           const char rw ) ;

static void validateFramePageSize( const unsigned int frameSize ) ;

static void validateMainMemorySize( const unsigned int mainMemorySize ) ;


/*****  Código das funções exportadas pelo Módulo  *****/

/***************************************************************************
*
*  Função: SIM  &Simula Memória Virtual
*
*******/

int simulaMemoriaVirtual( const char * algoritmo, const char * arquivo, unsigned int frameSize, unsigned int mainMemorySize, const char * mode )
{
    char rw, program[50] ;
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
    
    checkIfDebuggingModeIsOn( mode ) ;

    DEBUG puts("Executing simulator...") ;

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

    // tratamento da string passada como parâmetro
    strcpy(program, "./../programs/" ) ;
    strcat(program, arquivo ) ;

    fd = fopen( program , "r" ) ;

    storeAllTheFutureAccesses( fd , offsetBits ) ;

    while( fscanf(fd, "%x %c ", &addr, &rw) != EOF )
    {
        pageIndex = addr >> offsetBits ;

        if ( ListOfVirtualPages.list[pageIndex] == NOT_IN_MAIN_MEMORY )
        {
            pageFaultCounter++ ;

            DEBUG printf("Page fault of page nº : %d\n", pageIndex) ;

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
                    frameIndex = NOVO( ) ;
                }
                else
                {
                    puts("Chosen algorithm do no exist!") ;
                    destroyListOfVirtualPages( &ListOfVirtualPages ) ;
                    destroyMainMemory( &mainMemory ) ;
                    exit( -1 ) ;   
                }

                // Se página retirada da memória foi escrita incrementa nº de páginas escritas no disco
                if ( mainMemory.list[frameIndex].writed == true ) pagesWritedInDisc++ ;
            }
            else // Main memory is not full yet
            {
                frameIndex = mainMemory.count ;
                mainMemory.count++ ;
            }

            setFrameIndex( frameIndex , pageIndex ) ;
            allocNewFrame( mainMemory.list , frameIndex , pageIndex , rw ) ;
            ListOfVirtualPages.list[pageIndex] = frameIndex ;
        }
        else // Process is already in the memory
        {
            DEBUG printf("Page hit of page nº : %d\n", pageIndex) ;

            frameIndex = ListOfVirtualPages.list[pageIndex] ;

            mainMemory.list[frameIndex].flagR = rw == 'R' ? true : mainMemory.list[frameIndex].flagR ;
            mainMemory.list[frameIndex].flagM = rw == 'W' ? true : mainMemory.list[frameIndex].flagM ;
            mainMemory.list[frameIndex].writed = rw == 'W' ? true : mainMemory.list[frameIndex].writed ;
            mainMemory.list[frameIndex].lastAcess = clock( ) ;
        }

        if ( resetAlarm == CLEAR_INTERVAL )
        {
            for (int i = 0; i < mainMemory.count ; i++ )
            {
                mainMemory.list[i].flagR = false ;
                mainMemory.list[i].flagM = false ;
            }

            resetAlarm = 1 ;
        }

        decrementAcesses( pageIndex ) ;

        DEBUG getchar() ;

        resetAlarm++ ;
        timeTookCounter++ ;
    }

    printf("--------------------------===============--------------------------\n") ;
    printf("Name of file : %s\n", arquivo ) ;
    printf("Used algorithm : %s\n", algoritmo ) ;
    printf("Size of a frame : %d KB\n", ( unsigned int ) ( frameSize / pow(2, 10) ) ) ;
    printf("Size of main memory (RAM) : %d MB\n", ( unsigned int ) ( mainMemorySize / pow(2, 20) ) ) ;
    printf("Nº of frames : %d \n", numFrames ) ;
    printf("Simulated time took : %d units\n", timeTookCounter ) ;
    printf("Nº of page-faults : %d\n", pageFaultCounter ) ;
    printf("Nº of pages writed in the disc : %d\n", pagesWritedInDisc ) ;
    printf("--------------------------===============--------------------------\n") ;

    destroyListOfVirtualPages( &ListOfVirtualPages ) ;
    destroyMainMemory( &mainMemory ) ;

    return 0 ;

} /* Fim Função: SIM  &Simula Memória Virtual */

/*****  Código das funções encapsuladas no Módulo  *****/

static void inicializeMainMemory( ListOfFrames * const mainMemory, const unsigned int numFrames )
{
    mainMemory->list = ( Frame * ) malloc ( numFrames * sizeof(Frame) ) ;

    if ( mainMemory->list == NULL )
    {
        puts("Error allocating memory for main memory frames!") ;
        exit( -1 ) ;
    }

    mainMemory->count = 0 ;
}

static void destroyMainMemory( ListOfFrames * const mainMemory )
{
    free( mainMemory->list ) ;
}

static void inicializeListOfVirtualPages( tpListOfVirtualPages * const ListOfVirtualPages , const unsigned int numVirtualPages )
{
    ListOfVirtualPages->maxVirtualPages = numVirtualPages ;

    ListOfVirtualPages->list = ( VirtualPage * ) malloc ( numVirtualPages * sizeof( VirtualPage ) ) ;

    if ( ListOfVirtualPages->list == NULL )
    {
        puts("Error allocating memory for list of virtual pages!") ;
        exit( -1 ) ;
    }

    for ( int i = 0 ; i < ListOfVirtualPages->maxVirtualPages ; i++ )
    {
        ListOfVirtualPages->list[i] = NOT_IN_MAIN_MEMORY ;
    }
}

static void destroyListOfVirtualPages( tpListOfVirtualPages * const ListOfVirtualPages )
{
    free( ListOfVirtualPages->list ) ;
}

static void allocNewFrame( Frame * const ListOfFrames, const int frameIndex , const int virtualPagaIndex , const char rw  )
{
    ListOfFrames[frameIndex].flagR = false ;
    ListOfFrames[frameIndex].flagM = false ;
    ListOfFrames[frameIndex].writed = rw == 'W' ? true : false ;
    ListOfFrames[frameIndex].lastAcess = clock( ) ;
    ListOfFrames[frameIndex].virtualPageIndex = virtualPagaIndex ;
}

static void validateFramePageSize( const unsigned int frameSize )
{
    if ( frameSize < 8 || frameSize > 32 )
    {
        puts ("Size of page frame invalid! Try supported values: 8 to 32 KB") ;
        exit ( -1 ) ;
    }
}

static void validateMainMemorySize( const unsigned int mainMemorySize )
{
    if ( mainMemorySize < 1024 || mainMemorySize > 16384 )
    {
        puts ("Size of main memory invalid! Try supported values: 1024 to 16384 KB") ;
        exit ( -1 ) ;
    }
}

