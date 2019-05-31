// C Program for Message Queue (Reader Process) 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h>

#define NUM_MESSAGES 64

// structure for message queue 
struct mesg_buffer { 
    long mesg_type;
    char mesg_text[100]; 
} message; 
  
int main( int argc, char * argv[] ) 
{ 
    key_t key; 
    int msgid; 
    long msgtyp;
    char padding[30];
    pid_t pid;

    if ( argc != 3 )
    {
        puts("Erro na passagem de parâmetros!");
        return -1;
    }
    
    pid = getpid();

    //define qual o tipo de  mensagem esse processo 
    msgtyp = atol(argv[2]) + 1;

    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666); 

    for (int i = 0; i < msgtyp; i++)
    {
        strcat(padding, "\t\t\t\t\t");
    }
  
    for (int i = 0; i < NUM_MESSAGES; i++)
    {
        // msgrcv to receive message 
        msgrcv(msgid, &message, sizeof(message), msgtyp, 0); 
      
        // display the message 
        printf("%s[pid: %d] Received: %s \n",
                        padding,
                        pid,  
                        message.mesg_text);
        sleep(2);
    }
    
    message.mesg_type = atol(argv[1]) + 1;
    sprintf(message.mesg_text, "pid: %d", pid);
    msgsnd(msgid, &message, sizeof(message), 0);
    
    return 0; 
} 

