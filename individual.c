#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
#define PIN_LENGTH 2
#define PIN_WAIT_INTERVAL 2
 
void getPIN(char pin[PIN_LENGTH + 1]) 

{
  srand(getpid() + getppid());
 
  pin[0] = 49 + rand() % 7;
 
  for(int i = 1; i < PIN_LENGTH; i++)
  {
    pin[i] = 48 + rand() % 7;
  }
 
  pin[PIN_LENGTH] = '\0';
}

void sigint_handler(int sig)
{
printf("This is interrupted message. You has been interrupted ");
exit(1);
}

int main(void) 
{
  while(1) 
  {
    int fds[2];
    char pin[PIN_LENGTH + 1];
    char buffer[PIN_LENGTH + 1];
    signal(SIGINT,sigint_handler);
    
    pipe(fds);
 
    pid_t pid = fork();
 
    if(pid == 0) 
    {
      getPIN(pin); 
      close(fds[0]); 
      write(fds[1], pin, PIN_LENGTH + 1); 
 
      printf("Welcome to lucky number generator. \n");
 
      sleep(PIN_WAIT_INTERVAL);
 
      exit(EXIT_SUCCESS);
    }
 
    if(pid > 0) 
    {
      wait(NULL); 
      int y;
      printf("Enter any integer: ");
      scanf("integer: %d", &y);
      close(fds[1]); 
      read(fds[0], buffer, PIN_LENGTH + 1); 
      close(fds[0]); 
      printf("\nCongratulations, we generated '%s' as lucky number.\n\n", buffer);
      
        
    }
  }
 
  return EXIT_SUCCESS;
}
