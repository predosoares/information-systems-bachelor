#if ! defined( ALGORITHM_ )
#define ALGORITHM_ 

/**
 * 
 */
int LRU( ListOfFrames * mainMemory , tpListOfVirtualPages * ListOfVirtualPages ) ;

/**
 * 
 */
int NRU( ListOfFrames * mainMemory , tpListOfVirtualPages * ListOfVirtualPages ) ;

/**
 * 
 */
int NOVO( void ) ;

#endif