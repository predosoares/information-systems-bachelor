// C Program for Message Queue (Writer Process) 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <string.h>

#define NUM_MESSAGES 10

// structure for message queue 
struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[15]; 
} message; 
  
int main() 
{ 
    key_t key;
    int msgid;
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
    message.mesg_type = 1;
    
    for (int i = 0; i < NUM_MESSAGES; i++)
    {
        puts("Making package to send");        
        
        sprintf(message.mesg_text, "package %d", i);

        // msgsnd to send message 
        msgsnd(msgid, &message, sizeof(message), 0); 

        // display the message 
        printf("Data send is : %s \n", message.mesg_text);
        
        sleep(1);
    }
    /*
    // msgrcv to receive message 
    msgrcv(msgid, &message, sizeof(message), 1, 0); 
      
    // display the message 
    printf("Data Received is : %s \n",  
                        message.mesg_text);
    */
    // to destroy the message queue 
    //msgctl(msgid, IPC_RMID, NULL); 
    
    return 0; 
} 

