# concurrent-programming
## Choinka - POSIX

Małe skrzaty stroją choinkę na Boże Narodzenie, zawieszając na niej ozdoby, dostarczane przez Św. Mikołaja. W tym celu skonstruowały n-poziomowe rusztowanie pozwalające dotrzeć do wyższych gałązek. Wytrzymałość rusztowania jest ograniczona — na poziomie i-tym może być jednocześnie co najwyżej ki skrzatów. Poziom 1 jest najniższy oraz k1>k2>...>kn. Żeby dotrzeć na poziom i-ty, skrzat musi przejść kolejno przez wszystkie niższe poziomy, nie przeciążając rusztowania. Podobnie, żeby zejść, musi przejść przez kolejne niższe poziomy. Liczba ozdób, jakie można zawiesić na choince, również jest ograniczona i na poziomie i-tym wynosi pi. Ozdoby zabierane są z poziomu 0, na który dostarcza je sukcesywnie po kilka sztuk Św. Mikołaj, przy czym jest tam ograniczona przestrzeń — co najwyżej p0 ozdób. Skrzat w jednym swoim przejściu zdoła przenieść jedną ozdobę (ozdoba nie stanowi dodatkowego obciążenia rusztowania).

Cel zadania: synchronizacja skrzatów oraz Św. Mikołaja zakładając, że zawieszanie ozdoby na choince zajmuje pewien czas.

## Palacze tytoniu - IPC

Zadanie oparte jest na problemie palaczy tytoniu (opis w Wikipedii (po angielsku)) przy czym:

1. palacze rozliczają się między sobą za udostępniane składniki po aktualnym kursie giełdowym;
2. rola agenta sprowadza się do ustalania kursu giełdowego składników;
3. palacze nie mogą mieć debetu, więc jeśli palacza nie stać na zakup składników, musi poczekać na zmianę kursu lub przypływ gotówki za sprzedaż własnego składnika (początkowy stan konta palacza jest większy od 0);
4. palacz nabywa jednocześnie oba składniki;
5. należy użyć kolejki komunikatów do przekazywania składników między palaczami.


Created by Konrad Romański ©
