#include <stdio.h>
#include "grava_structs.h"

typedef struct exemplo{
	 int i;
} Exemplo;

int main(void){
	
	Exemplo teste[] = {{0x00001000},{0x05060708},{0x4ffcab07},{0x090a0b0c},{0x0d0e0f10},{0x01020304},{0x01100000},{0x34153080},{0x09080706},{0x05040302}};
    grava_structs (10, (void*)teste, "i", 'B', "exemplo.bin");
    dump_structs ("exemplo.bin");

    return 0;
 }
