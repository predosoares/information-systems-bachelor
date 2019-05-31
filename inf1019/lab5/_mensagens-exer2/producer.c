// C Program for Message Queue (Writer Process) 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <string.h>

#define NUM_MESSAGES 64
#define MAXFILA 8

// structure for message queue 
struct mesg_buffer { 
    long mesg_type;
    char mesg_text[15];
} message; 

struct msqid_ds message_config;

int main( int argc, char * argv[] ) 
{ 
    key_t key;
    int msgid;
    int num_consumers;


    if ( argc != 2 )
    {
        puts("Erro na passagem de parâmetros!");
        return -1;
    }

    num_consumers = atol(argv[1]);
  
    // ftok to generate unique key 
    key = ftok("progfile", 65);
    
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 

    // obtem a configuração do kernel para a fila identificada por msgid
    msgctl(msgid, IPC_STAT, &message_config);

    // Configura a fila de mensagens para o tamanho máximo MAXFILA
    message_config.msg_qbytes = (msglen_t) sizeof(struct mesg_buffer) * MAXFILA;

    // Set the configuration
    msgctl(msgid, IPC_STAT, &message_config);
    message.mesg_type = 1;

    for (int i = 0; i < NUM_MESSAGES; i++)
    {
        sprintf(message.mesg_text, "package %d", i);

        // msgsnd to send message 
        msgsnd(msgid, &message, sizeof(message), 0);
        
        // display the message 
        printf("Send package: %s\n", message.mesg_text);
        sleep(1);
    }
    

    // msgrcv to receive message
    for (int i = 0; i < num_consumers; i++)
    {
        msgrcv(msgid, &message, sizeof(message), 2, 0);
        printf("Consumer %d ended \n", i + 1);
    }
    
    // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 
    
    return 0;
} 

