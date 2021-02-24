#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

enum palacz{tyton, papier, zapalki};

char* RED = "\033[1;31m";
char* GREEN = "\033[1;92m";
char* YELLOW = "\033[1;93m";
char* CYAN = "\033[1;96m";
char* MAGENTA = "\033[1;35m";
char* CLEAR = "\033[0m";

int gielda[3] = {0};
int wyplata[3] = {0};

struct przelew
{
  long mtype;
  int ile;
};

struct cena
{
  long mtype;
  int gielda[3];
};


int agent()
{
  struct cena dane;
  while(1)
  {
    //losowanie kursu
    dane.mtype = 1;
    for (int i = 0; i < 3; i++)  { dane.gielda[i] = rand() % 100 + 110; }
    printf("%s[LOG][%d] - Ustalam kurs gieldowy na: tyton - %d, papier - %d, zapalki - %d\n%s", YELLOW, getpid(), dane.gielda[0], dane.gielda[1], dane.gielda[2], CLEAR);
    //wysylam dane
    for (int i = 0; i < 3; i++)  { msgsnd(gielda[i], &dane, sizeof(dane), 1); }
    sleep(2);
  }
}


int palacz(int nr)
{
  int portfel = 300;
  struct cena kurs;
  struct przelew p;
  kurs.mtype = 1;
  p.mtype = 0;
  while(1)
  {
    msgrcv(wyplata[nr], &p, sizeof(p), 1, IPC_NOWAIT);
    portfel += p.ile;

    msgrcv(gielda[nr], &kurs, sizeof(kurs), 1, 0);


    //sprawdzanei czy stac na produkty
    if (kurs.gielda[(nr+1)%3] + kurs.gielda[(nr+2)%3] <= portfel)
    {
      printf("%s[LOG][%d] - Mam %d, kupuje skladniki po %d i %d. Ide palic\n%s", CYAN, getpid(), portfel, kurs.gielda[(nr+1)%3], kurs.gielda[(nr+2)%3], CLEAR);
      //placenie
      portfel -= kurs.gielda[(nr+1)%3] + kurs.gielda[(nr+2)%3];
      p.ile =  kurs.gielda[(nr+1)%3];
      msgsnd(wyplata[(nr+1)%3], &p, sizeof(p), 1);
      p.ile =  kurs.gielda[(nr+2)%3];
      msgsnd(wyplata[(nr+2)%3], &p, sizeof(p), 1);

      //palenie
      sleep(3);
    }
  }
}


int main(int arg, char *argv[])
{
  srand(time(NULL));

//tworzenie kolejek
  for (int i = 0; i < 3; i++) {  gielda[i] = msgget(IPC_PRIVATE, IPC_CREAT|0600); }
  for (int i = 0; i < 3; i++) {  wyplata[i] = msgget(IPC_PRIVATE, IPC_CREAT|0600); }

  //tworze agenta
  if (fork() == 0)
    agent();

//tworze palaczy
  for (int i = 0; i < 3; i++)
  {
    if (fork() == 0)
      palacz(i);
  }
}
