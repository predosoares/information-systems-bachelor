// C Program for Message Queue (Reader Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h>
  
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
  
    while(1)
    {
        if (strncmp(message.mesg_text, "Bye", 3) == 0)
        {
            break;
        }
        
        // msgrcv to receive message 
        msgrcv(msgid, &message, sizeof(message), 1, 0); 
      
        // display the message 
        printf("Data Received is : %s \n",  
                        message.mesg_text);
        
        if (strncmp(message.mesg_text, "Bye", 3) == 0)
        {
            break;
        }
        
        printf("Write Data : "); 
        fgets(message.mesg_text, 100, stdin); 
        
        // msgsnd to send message 
        msgsnd(msgid, &message, sizeof(message), 0); 

        // display the message 
        printf("Data send is : %s \n", message.mesg_text); 
    }
    // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 
  
    return 0; 
} 

