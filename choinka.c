#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#define skrzaty 5//ilosc skrzatow
#define ilosc_pieter 5 //ilosc pieter
#define ozdoby_plus 1 //ozdoby+
#define ozdoby_dodaj 4 //ile mikolaj dodaje ozdob
#define max_na_magazynie 10 //max na magzaynie
int max_ozodoby = 0; //ile max ozdob na pietro
int ile_na_choince; //aktualna liczba ozdob na choince

int ozdoby[ilosc_pieter + 1] = {0}; //ilosc ozdob na kazdym pietrze
int pietra[ilosc_pieter + 1] = {0}; //ilosc skrzatow na kazdym pietrze
int limit_ozdob[ilosc_pieter + 1] = {0}; //limity ozdob na poziom
int limit_skrzatow[ilosc_pieter + 1] = {skrzaty}; //limity skrzatow na poziom

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

void s()
{
  printf("%s---SKRZATY---\n", RED);
  for (int pietro = 0; pietro <= ilosc_pieter; pietro++)
  {
    printf("%s[%d]", GREEN, pietro);
    for (int j= 1 ; j <= pietra[pietro]; j++)
    {
      printf("*");
    }
    printf(" - %d / %d", pietra[pietro], limit_skrzatow[pietro]);
    printf("\n");
  }
  printf("%s", CLEAR);
}

void rysuj()
{
  printf("%s---OZDOBY---\n", RED);
  for (int pietro = 1; pietro <= ilosc_pieter; pietro++)
  {
    printf("%s[%d]", GREEN, pietro);
    for (int j= 1 ; j <= ozdoby[pietro]; j++)
    {
      printf("*");
    }
    printf(" - %d / %d", ozdoby[pietro], limit_ozdob[pietro]);
    printf("\n");
  }
  printf("%s", CLEAR);
}

void* zanies_ozdobe(void* arg)
{
  static __thread int niesiona_ozdoba = 0;
  static __thread int aktualny_poziom = 0;
  // static __thread int ewakuacja = 0;

  while(ile_na_choince != max_ozodoby)
  {

    printf("%s[LOG][%ld] - ide sobie, mam: %d, na poziomie: %d\n%s", YELLOW, pthread_self(), niesiona_ozdoba, aktualny_poziom, CLEAR);
    // s();

    sleep(1);
    //##choinka ubrana

       if (niesiona_ozdoba == 0)
      {
        //##brak ozdob, parter
        if (aktualny_poziom == 0)
        {
          pthread_mutex_lock(&mtx_ozdoby_get);
          if(ile_na_choince != max_ozodoby)
            // while(aktualna_liczba_magazyn <= 0)
              // pthread_cond_wait(&can_get, &mtx_ozdoby_get);

          if (aktualna_liczba_magazyn > 0) //sprawdz, czy czasem nie koniec ubierania
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
        else if (pietra[aktualny_poziom - 1] <  limit_skrzatow[aktualny_poziom - 1] + 1 || aktualny_poziom == 1)
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
        if (pietra[aktualny_poziom + 1] <  limit_skrzatow[aktualny_poziom + 1])
        {
          printf("%s[LOG][%ld] - wchodze na %d\n%s", CYAN, pthread_self(), aktualny_poziom + 1, CLEAR);

          pthread_mutex_lock(&mtx_pietra);
          aktualny_poziom++;
          pietra[aktualny_poziom - 1]--;
          pietra[aktualny_poziom]++;
          pthread_mutex_unlock(&mtx_pietra);


        }

        if (ozdoby[aktualny_poziom] < limit_ozdob[aktualny_poziom])
        {
          pthread_mutex_lock(&mtx_ozdoby_put);
          ozdoby[aktualny_poziom]++;
          niesiona_ozdoba--;
          rysuj();
          pthread_mutex_unlock(&mtx_ozdoby_put);

          pthread_mutex_lock(&mtx_ile_choinka);
          ile_na_choince++;
          printf("%s[LOG][%ld] - zostawiam ozdobe na %d, mam %d ozdob, na choince jest: %d z %d\n%s", MAGENTA, pthread_self(), aktualny_poziom, niesiona_ozdoba, ile_na_choince, max_ozodoby, CLEAR);
          pthread_mutex_unlock(&mtx_ile_choinka);

        }
      }
  }
  printf("%s[LOG][%ld] - Choinka ubrana!\n%s", YELLOW, pthread_self(), CLEAR);

}

void* przynies_ozdobe(void* arg)
{
  // rysuj();

  while(ile_na_choince != max_ozodoby)
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
  pthread_cond_signal(&can_get);
  printf("%s[LOG][%ld] (mikolaj) - koniec ozdob\033[0\n%s", RED, pthread_self(), CLEAR);
}


int main(int arg, char *argv[])
{
  printf("%s[LOG][%ld] - program is starting%s\n", YELLOW, pthread_self(), CLEAR);

  pietra[0] = skrzaty;
  limit_ozdob[0] = 0;

  for (int i = 1 ; i <= ilosc_pieter; i++)
  {
    limit_ozdob[i] = ilosc_pieter - i + ozdoby_plus;
    limit_skrzatow[i] = ilosc_pieter - i + 2;
  }
  for (int i = 1; i < ilosc_pieter + 1; i++) max_ozodoby+=limit_ozdob[i];
  //uwzgledniamy korytarz ewakuacyjny
  for (int i = 1; i < ilosc_pieter; i++) limit_skrzatow[i]--;

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


  // rysuj();

  printf("%s[LOG][%ld] - program ended%s\n", YELLOW, pthread_self(), CLEAR);

}
