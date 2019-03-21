// Lab 1 (Fork e Exec) - inf1019 - Sistemas de Computação

// Exer 2 - prática de exec

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

int main(int argc, char* argv[]){

    if ( fork() != 0 ){ 
        // exec olamundo
        execv(argv[1], argv);
    } else { 
        //exec echoo
        execv(argv[2], argv);
    }
    
    return 0;
}
