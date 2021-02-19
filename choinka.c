#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#define skrzaty 5 //ilosc skrzatow
#define ilosc_pieter 4 //ilosc pieter
#define max_ozodoby 2 //ile max ozdob na pietro
#define max_skrzatow_na_poziom 5 //ile max skrzatow na pietro
#define ozdoby_dodaj 2 //ile mikolaj dodaje ozdob
#define max_na_magazynie 3 //max na magzaynie
int ile_na_choince; //aktualna liczba ozdob na choince

int ozdoby[ilosc_pieter + 1] = {0}; //ilosc ozdob na kazdym pietrze
int pietra[ilosc_pieter + 1] = {0}; //ilosc skrzatow na kazdym pietrze

int aktualna_liczba_magazyn = 0; //ozdoby na dole

pthread_mutex_t mtx_ozdoby_get = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_ozdoby_put = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_pietra = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_ile_choinka = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_get;

char* RED = "\033[1;31m"; // BŁĄD
char* GREEN = "\033[1;92m"; //POBIERAM, ZOSTAWIAM
char* YELLOW = "\033[1;93m"; // KONIEC
char* CYAN = "\033[1;96m"; //SCHODZĘ, WCHODZĘ
char* MAGENTA = "\033[1;35m";
char* CLEAR = "\033[0m";

void rysuj()
{
  printf("%s---PREZENTY---\n", RED);
  for (int i = 1; i <= ilosc_pieter; i++)
  {
    printf("%s[%d]", GREEN, i);
    for (int j= 1 ; j <= max_ozodoby; j++)
    {
      printf("*");
    }
    printf(" - %d", ozdoby[i]);
    printf("\n");
  }
  printf("%s", CLEAR);
}

void* zanies_ozdobe(void* arg)
{
  static __thread int niesiona_ozdoba = 0;
  static __thread int aktualny_poziom = 0;

  while(ile_na_choince != max_ozodoby*ilosc_pieter)
  {
    sleep(1);
    //##choinka ubrana
      if (niesiona_ozdoba == 0)
      {
        //##brak ozdob, parter
        if (aktualny_poziom == 0)
        {
          pthread_mutex_lock(&mtx_ozdoby_get);
          while(aktualna_liczba_magazyn <= 0)
            pthread_cond_wait(&can_get, &mtx_ozdoby_get);

          aktualna_liczba_magazyn--;
          pthread_mutex_unlock(&mtx_ozdoby_get);
          niesiona_ozdoba++;
          printf("%s[LOG][%ld] - biore ozdobe, mam %d, pozostalo %d\n%s", MAGENTA, pthread_self(), niesiona_ozdoba, aktualna_liczba_magazyn, CLEAR);
        }
        //##brak ozdob, 1. pietro
        else if (aktualny_poziom == 1)
        {
          printf("%s[LOG][%ld] - schodze na parter\n%s", CYAN, pthread_self(), CLEAR);
          aktualny_poziom--;
          pthread_mutex_lock(&mtx_pietra);
          pietra[aktualny_poziom + 1]--;
          pthread_mutex_unlock(&mtx_pietra);
        }
        //##brak ozdob, schodze
        else if (pietra[aktualny_poziom - 1] < max_skrzatow_na_poziom)
        {
          printf("%s[LOG][%ld] - schodze na %d\n%s", CYAN, pthread_self(),  aktualny_poziom - 1, CLEAR);
          aktualny_poziom--;
          pthread_mutex_lock(&mtx_pietra);
          pietra[aktualny_poziom + 1]--;
          pietra[aktualny_poziom]++;
          pthread_mutex_unlock(&mtx_pietra);

        }
      }
      else
      {
        //##ma ozdoby, idzie
        if (pietra[aktualny_poziom + 1] < max_skrzatow_na_poziom)
        {
          printf("%s[LOG][%ld] - wchodze na %d\n%s", CYAN, pthread_self(), aktualny_poziom + 1, CLEAR);
          aktualny_poziom++;

          pthread_mutex_lock(&mtx_pietra);
          pietra[aktualny_poziom - 1]--;
          pietra[aktualny_poziom]++;
          pthread_mutex_unlock(&mtx_pietra);


          if (ozdoby[aktualny_poziom] < max_ozodoby)
          {
            printf("%s[LOG][%ld] - zostawiam ozdobe na %d, mam %d ozdob\n%s", MAGENTA, pthread_self(), aktualny_poziom, niesiona_ozdoba, CLEAR);
            pthread_mutex_lock(&mtx_ozdoby_put);
            ozdoby[aktualny_poziom]++;
            rysuj();
            pthread_mutex_unlock(&mtx_ozdoby_put);
            niesiona_ozdoba--;

            pthread_mutex_lock(&mtx_ile_choinka);
            ile_na_choince++;
            pthread_mutex_unlock(&mtx_ile_choinka);

          }
        }
      }
  }
  printf("%s[LOG][%ld] - Choinka ubrana!\n%s", YELLOW, pthread_self(), CLEAR);

}

void* przynies_ozdobe(void* arg)
{
  rysuj();

  while(ile_na_choince != max_ozodoby*ilosc_pieter)
  {
    pthread_mutex_lock(&mtx_ozdoby_get);
    if (aktualna_liczba_magazyn + ozdoby_dodaj <= max_na_magazynie)
      aktualna_liczba_magazyn+=ozdoby_dodaj;
    else
      aktualna_liczba_magazyn = max_na_magazynie;
    printf("%s[LOG][%ld] (mikolaj) - dodaje ozdoby, stan: %d\n%s", GREEN, pthread_self(), aktualna_liczba_magazyn, CLEAR);
    pthread_cond_signal(&can_get);
    pthread_mutex_unlock(&mtx_ozdoby_get);
    sleep(2);
  }
  printf("%s[LOG][%ld] (mikolaj) - koniec ozdob\033[0\n%s", RED, pthread_self(), CLEAR);
}


int main(int arg, char *argv[])
{
  printf("%s[LOG][%ld] - program is starting%s\n", YELLOW, pthread_self(), CLEAR);
  pthread_t proc[skrzaty];
  pthread_t proc_mikolaj;

  pthread_create(&proc_mikolaj, NULL, *przynies_ozdobe, NULL);

  for (int i = 0; i < skrzaty; i++)
  {
    sleep(0.2);
    pthread_create(&proc[i], NULL, *zanies_ozdobe, NULL);
  }

  for (int i = 0; i < skrzaty; i++)
  {
    sleep(0.2);
    pthread_join(proc[i], NULL);
  }
  pthread_join(proc_mikolaj, NULL);


  rysuj();

  printf("%s[LOG][%ld] - program ended%s\n", YELLOW, pthread_self(), CLEAR);

}
