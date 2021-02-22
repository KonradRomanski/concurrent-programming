# concurrent-programming
## Choinka - POSIX

### Zadanie
Małe skrzaty stroją choinkę na Boże Narodzenie, zawieszając na niej ozdoby, dostarczane przez Św. Mikołaja. W tym celu skonstruowały n-poziomowe rusztowanie pozwalające dotrzeć do wyższych gałązek. Wytrzymałość rusztowania jest ograniczona — na poziomie i-tym może być jednocześnie co najwyżej ki skrzatów. Poziom 1 jest najniższy oraz k1>k2>...>kn. Żeby dotrzeć na poziom i-ty, skrzat musi przejść kolejno przez wszystkie niższe poziomy, nie przeciążając rusztowania. Podobnie, żeby zejść, musi przejść przez kolejne niższe poziomy. Liczba ozdób, jakie można zawiesić na choince, również jest ograniczona i na poziomie i-tym wynosi pi. Ozdoby zabierane są z poziomu 0, na który dostarcza je sukcesywnie po kilka sztuk Św. Mikołaj, przy czym jest tam ograniczona przestrzeń — co najwyżej p0 ozdób. Skrzat w jednym swoim przejściu zdoła przenieść jedną ozdobę (ozdoba nie stanowi dodatkowego obciążenia rusztowania).

Cel zadania: synchronizacja skrzatów oraz Św. Mikołaja zakładając, że zawieszanie ozdoby na choince zajmuje pewien czas.
### Opis wykonania zadania

Zadanie polega na ubraniu choinki przez skrzaty (wątki). Każdy skrzat rozpoczyna swoją wędrówkę na poziomie 0 - poziomie gruntu.  
Następnie, jeśli może, to pobiera ozdobę i, jeśli tylko jest wolny, wdrapuje się na kolejny poziom rusztowania, z którego będzie mógł zawiesić ozdoby. Gdy na poziomie wyższym są skrzatowi koledzy nasz skrzat czeka. Gdy na poziomie, na który się wdrapał jest osiągnięty limit ozdób, skrzat idzie dalej.  
Gdy skrzat założy swoją ozdobę będzie próbował zejść. Zejść może, gdy na poziomie niżej jest wolne miejsce, czyli. Wolne miejsce wtedy, gdy na poziomie niżej jest mniej skrzatów niż wynosi limit na danym piętrze. Skrzaty schodzące mają dodatkowo na każdym piętrze 1 miejsce specjalne, do którego nie maja dostępu skrzaty wchodzące (przez co na każdym piętrze maja one dostępne o 1 miejsce mniej). Istnieje ono, by uniemożliwić skrzatom zablokowanie się. Skrzat schodzi sukcesywnie na dół po kolejna ozdobę.  
Ozdoby dostarczane są co jakiś czas przez Świętego Mikołaja, który dostarcza je, gdy na magazynie jest miejsce i choinka nie została jeszcze ubrana.  
Gdy choinka zostanie ubrana wszystkie skrzaty wraz ze Świętym Mikołajem się zatrzymują.


## Palacze tytoniu - IPC

### Zadanie
Zadanie oparte jest na problemie palaczy tytoniu (opis w Wikipedii (po angielsku)) przy czym:

1. palacze rozliczają się między sobą za udostępniane składniki po aktualnym kursie giełdowym;
2. rola agenta sprowadza się do ustalania kursu giełdowego składników;
3. palacze nie mogą mieć debetu, więc jeśli palacza nie stać na zakup składników, musi poczekać na zmianę kursu lub przypływ gotówki za sprzedaż własnego składnika (początkowy stan konta palacza jest większy od 0);
4. palacz nabywa jednocześnie oba składniki;
5. należy użyć kolejki komunikatów do przekazywania składników między palaczami.

### Opis wykonania zadania

Created by Konrad Romański ©
