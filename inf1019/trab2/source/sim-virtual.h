#if ! defined( SIMVIRTUAL_ )
#define SIMVIRTUAL_ 

#include <stdbool.h>
#include <time.h>

typedef struct frame {

    bool flagR ;
    
    bool flagM ;

    bool writed ;
    
    clock_t lastAcess ;

    int virtualPageIndex ;

} Frame ;

typedef struct listofframes {

    Frame * list ;
    
    int count ;

}  ListOfFrames ;

typedef unsigned int VirtualPage ;

typedef struct listofvirtualpages {

    VirtualPage * list ;
    
    unsigned int maxVirtualPages ;

}  tpListOfVirtualPages ;

int simulaMemoriaVirtual( const char * algoritmo, const char *  arquivo, unsigned int frameSize, unsigned int mainMemorySize, const char * mode ) ;

#endif

