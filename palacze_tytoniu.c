#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

enum palacz{tyton, papier, zapalki};
int kurs[3] = {0};
int saldo[3] = {300};

char* RED = "\033[1;31m";
char* GREEN = "\033[1;92m";
char* YELLOW = "\033[1;93m";
char* CYAN = "\033[1;96m";
char* MAGENTA = "\033[1;35m";
char* CLEAR = "\033[0m";

int agent()
{
  while(1)
  {
    //losowanie kursu
    for (int i = 0; i < 3; i++) kurs[i] = rand() % 100 + 10;

  }

}

int palacz(int nr)
{
  while(1)
  {
    //sprawdzanei czy stac na produkty
    if (kurs[(nr+1)%3] + kurs[(nr+2)%3] <= saldo[nr])
    {
      //placenie
      saldo -= kurs[(nr+1)%3] + kurs[(nr+2)%3];
      saldo[(nr+1)%3] += kurs[(nr+1)%3];
      saldo[(nr+2)%3] += kurs[(nr+2)%3];
      //palenie
      sleep(4);
    }

  }
}

int main(int arg, char *argv[])
{
  srand(time(NULL));

  for (int i = 0; i < 3; i++)
  {
    if (fork() == 0)
    {
      palacz(i);
    }
  }
  if (fork() == 0)
    agent();
  else
    wait(0);
}
