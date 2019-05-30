// C Program for Message Queue (Reader Process) 
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
    char mesg_text[100]; 
} message; 
  
int main() 
{ 
    key_t key; 
    int msgid; 
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666); 
  
    for (int i = 0; i < NUM_MESSAGES; i++)
    {
        // msgrcv to receive message 
        msgrcv(msgid, &message, sizeof(message), 1, 0); 
      
        // display the message 
        printf("Data Received is : %s \n",  
                        message.mesg_text);
        sleep(2);
    }
    
    // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 
    
    return 0; 
} 

