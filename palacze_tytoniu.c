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
int kurs[3] = {0};
int saldo[3] = {300};

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
  int mtype;
  int gielda[3];
};

int agent()
{
  struct cena dane;
  while(1)
  {
    //losowanie kursu
    printf("%s[LOG][%d] - Ustalam kurs gieldowy na: tyton - %d, papier - %d, zapalki - %d\n%s", YELLOW, getpid(), kurs[0], kurs[1], kurs[2], CLEAR);
    dane.mtype = 1;
    for (int i = 0; i < 3; i++)  { dane.gielda[i] = rand() % 100 + 110; }
    //wysylam dane
    for (int i = 0; i < 3; i++)  { msgsnd(gielda[i], &dane, sizeof(dane.gielda), 0); }
    sleep(1);
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

    msgrcv(wyplata[nr], &p, sizeof(p.ile), 0, 0);
    portfel += p.ile;

    msgrcv(gielda[nr], &kurs, sizeof(kurs.gielda), 1, 0);

    //sprawdzanei czy stac na produkty
    if (kurs.gielda[(nr+1)%3] + kurs.gielda[(nr+2)%3] <= portfel)
    {
      //placenie
      printf("%s[LOG][%d] - Mam %d, kupuje skladniki i pale\n%s", CYAN, getpid(), portfel, CLEAR);
      portfel -= kurs.gielda[(nr+1)%3] + kurs.gielda[(nr+2)%3];
      p.ile =  kurs.gielda[(nr+1)%3];
      msgsnd(wyplata[(nr+1)%3], &p, sizeof(p.ile), 0);
      p.ile =  kurs.gielda[(nr+2)%3];
      msgsnd(wyplata[(nr+2)%3], &p, sizeof(p.ile), 0);

      //palenie
      sleep(4);
    }
  }
}

int main(int arg, char *argv[])
{
  srand(time(NULL));

//tworzenie kolejek
  for (int i = 0; i < 3; i++) {  gielda[i] = msgget(45285 + i, IPC_CREAT|0600); }
  for (int i = 0; i < 3; i++) {  wyplata[i] = msgget(45285 + i, IPC_CREAT|0600); }


//tworze palaczy
  for (int i = 0; i < 3; i++)
  {
    if (fork() == 0)
    {
      palacz(i);
    }
  }
  //tworze agenta
  if (fork() == 0)
    agent();
  else
    wait(0);
}
