# Chreh Mfessel dyal Projet Codexion (EXPLANATION.md)

---

## 1. Chno howa had l-projet (What the project is)

Had l-projet smito **Codexion**. Howa simulation multithreading mktouba b l-langage **C**, mbenya 3la l-mochkil l-ma3rouf f l-informatique dyal **Dining Philosophers**.

### L-Fikra l-3amma:
- Tkhayyel 3endna wahed l-group dyal les développeurs (**coders**) galsin f tabla dayra f un espace de co-working.
- F l-west d l-tabla kayn compiler shared (partagé).
- Bach ay coder y9der ycompili l-code dyalo, khassou darori yakhod **jouj hardware USB dongles**: wahed 3la limn dyalo w wahed 3la lisr dyalo.
- Hit kolla coder kaypartagi les dongles m3a l-jiran dyalo (li 3la limn w li 3la lisr), joj coders las9in f ba3diyat-hom ma y9drouch ycompiliw f nefs l-wa9t.

### L-Boucle li kaydir kolla Coder:
1. **Compiling**: Kayakhod 2 dongles, w kaycompili l-code dyalo pendant wahed l-moudda (`time_to_compile`).
2. **Debugging**: Kaytele9 les 2 dongles bach ykhlli jirano ykhedmou, w kaygless ydebuggi (`time_to_debug`).
3. **Refactoring**: Kaygless y-refactori l-code dyalo (`time_to_refactor`) 9bel ma y3awed ydemandi les dongles.

### L-Khatar (Burnout):
- Kolla coder 3ndo wahed l-wa9t maxi smito `time_to_burnout`.
- Ila daz had l-wa9t kamel bla ma ybda ycompili, kayw9e3 lih **burnout** (kay3ya w kayt-bloqua), w l-simulation kamla katw9ef f l-blassa.

### L-Hadaf dyal l-projet:
- N-gériw les threads w les ressources partagées (dongles, variables, affichage) bla ma yw9e3:
  - **Deadlock**: Kol wahed chad dongle w kaytsenna lakhor ytele9, w kolchi ytbloqua.
  - **Data Race**: Jouj threads ybdlou nefs la variable f nefs l-wa9t bla mutex.
  - **Starvation**: Chi coder yb9a dima me7roum w maywessloch nobto hta ymot.

---

## 2. Chno kaydir kolla fichier (What each file does)

| Fichier | L-Khedma dyalo |
|---|---|
| `includes/codexion.h` | Header file fih ga3 les structures (`t_heap`, `t_dongle`, `t_coder`, `t_simulation`), les macros, w les prototypes d les fonctions. |
| `src/main.c` | L-point d'entrée dyal l-programme (`main`). Kay-checki les arguments, kay-initialisi kolchi, kay-lanci les threads, w kaydir cleanup f l-lakher. |
| `src/init.c` | Kaydir l-parsing dyal les 8 arguments men l-command line, w kay-initialisi la mémoire, les mutexes, w les condition variables. |
| `src/utils.c` | Fih des fonctions d l-moussa3ada: hissab l-wa9t b ms (`get_time_ms`), n3as dki bla ma ytbloqua l-programme (`smart_usleep`), l-ktaba f l-ecran b l-mutex (`print_log`), w conversion dyal string l ra9m (`ft_atoi_positive`). |
| `src/heap.c` | Implémentation dyal **Min-Heap** (Priority Queue). Kay-rttiib chkoun 3ndo l-asbaqiya yahed l-dongle swa b **FIFO** (li ja lwel) ola b **EDF** (li 9reb ymot). |
| `src/dongles.c` | Khedmtou hiya gestion dyal les dongles: ychouf wach msalin w cooldown salat (`can_take_dongles`), yakhodhom b un ordre mratteb bach yfout deadlock (`acquire_dongles`), w ytele9hom (`release_dongles`). |
| `src/coder.c` | Routine d kolla coder (thread loop): kayakhod dongles -> kaycompili -> kaytele9hom -> kaydebuggi -> kayrefactori. |
| `src/monitor.c` | Routine d l-3essas (monitor thread): kaydor kolla 1 milliseconde bach ychouf wach chi wahed w9e3 lih burnout awla wach kolchi sala l-quota dyal compiles. |
| `src/cleanup.c` | Kaymsseh kolchi f l-lakher: kay-destructi les mutexes w les condition variables, w kaydir `free` l ga3 les allocations bach ma yb9a hta fuite mémoire (leaks). |

---

## 3. Chno kadir kolla fonction b tafsil (What each function does)

---

### A. `src/main.c`

#### 1. `start_threads(t_simulation *sim)`
- **Chno kadir**: Kat-lanci thread dyal kolla coder wahed b wahed b `pthread_create`, w kat-lanci thread dyal l-monitor.
- **Les inputs**: `sim` (pointeur 3la la structure principale).
- **Chno katretoorner**: `0` ila t-lanca kolchi b nja7, awla `-1` ila fchel chi thread.

#### 2. `join_threads(t_simulation *sim)`
- **Chno kadir**: Kat-tsenna ga3 les threads ysalew khedmethom b `pthread_join` (l-monitor w ga3 les coders) bach l-programme ma yssalich 9bel l-wa9t.
- **Les inputs**: `sim`.
- **Chno katretoorner**: Walo (`void`).

#### 3. `main(int argc, char **argv)`
- **Chno kadir**: L-fonction l-assasiya:
  1. Kat-3eyyet l `parse_args` bach t-verifi les arguments.
  2. Kat-3eyyet l `init_simulation` bach t-allocé w t-prepari kolchi.
  3. Katssjel l-wa9t d l-bedya (`start_time`).
  4. Kat-lanci les threads b `start_threads`.
  5. Kat-tsennahom b `join_threads`.
  6. Kat-nqqi kolchi b `clean_simulation` w katsali.
- **Les inputs**: `argc` (3adad l-arguments), `argv` (tableau fih les textes d l-arguments).
- **Chno katretoorner**: `0` ila daz kolchi mzyan, `1` ila kant chi erreur.

---

### B. `src/init.c`

#### 1. `parse_args(int argc, char **argv, t_simulation *sim)`
- **Chno kadir**: Kat-tverifi wach 3endna exactement 8 d les arguments. Kat-t7owel kol argument men string l ra9m, kat-tverifi bli les nombres positive w valid (coders bin 1 w 200), w kat-chouf wach l-scheduler howa `fifo` awla `edf`.
- **Les inputs**: `argc`, `argv`, `sim`.
- **Chno katretoorner**: `0` ila kan kolchi s7i7, `-1` ila kan chi argument ghalat.

#### 2. `init_dongles(t_simulation *sim)`
- **Chno kadir**: Kat-dir `malloc` l tableau d les dongles, w kat-initialisi kol dongle: l-id dyalo, l-mutex dyalo, condition variable `cond`, w kat-initialisi l-heap (priority queue) dyalo.
- **Les inputs**: `sim`.
- **Chno katretoorner**: `0` ila nja7 l-allocation, `-1` ila w9e3 mochkil f la mémoire.

#### 3. `init_coders(t_simulation *sim)`
- **Chno kadir**: Kat-dir `malloc` l tableau d les coders, w kat-3ti l kol coder: l-id dyalo, l-compteur d les compiles (`compiles_count = 0`), w les pointeurs 3la l-dongle li 3la lisr dyalo (`left_dongle`) w li 3la limn dyalo (`right_dongle`).
- **Les inputs**: `sim`.
- **Chno katretoorner**: `0` ila nja7, `-1` ila fchel.

#### 4. `init_simulation(t_simulation *sim)`
- **Chno kadir**: Kat-jm3 l-initialisation kamla: kat-initialisi `state_mutex` w `log_mutex`, w kat-3eyyet l `init_dongles` w `init_coders`.
- **Les inputs**: `sim`.
- **Chno katretoorner**: `0` ila daz kolchi mzyan, `-1` ila kant chi erreur.

---

### C. `src/utils.c`

#### 1. `get_time_ms(void)`
- **Chno kadir**: Katjbed l-wa9t l-7ali dyal l-pc b l-millisecondes b l-fonction `gettimeofday`.
- **Les inputs**: Walo (`void`).
- **Chno katretoorner**: Timestamp b `long long` fih l-wa9t b ms.

#### 2. `smart_usleep(long long time_in_ms, t_simulation *sim)`
- **Chno kadir**: F 3oud ma dir sleep kbir d9a wehda w ytbloqua l-programme ila mat chi coder, kat-n3ess b chwiya b chwiya (500 microsecondes) w kolla merra kat-chouf wach l-simulation w9fat. Ila w9fat, katfiq f l-blassa.
- **Les inputs**: `time_in_ms` (chhal d l-wa9t khassou yn3ess b ms), `sim`.
- **Chno katretoorner**: Walo (`void`).

#### 3. `print_log(t_simulation *sim, int coder_id, const char *status)`
- **Chno kadir**: Kat-tbe3 l-messages f l-ecran (bhal `has taken a dongle`, `is compiling`, etc.) de maniere securisée b `log_mutex` w `state_mutex`, bach les phrases ma ydkhlouch f ba3diyat-hom ila ktbou jouj threads f nefs l-wa9t.
- **Les inputs**: `sim`, `coder_id`, `status` (message texte).
- **Chno katretoorner**: Walo (`void`).

#### 4. `ft_atoi_positive(const char *str)`
- **Chno kadir**: Kat-t7owel text l ra9m s7i7 mowejab (positive int). Ila l9at 7rouf awla ra9m salib awla overflow, kat-retoorner `-1`.
- **Les inputs**: `str` (chaine de caracteres).
- **Chno katretoorner**: Ra9m positive int, awla `-1` f l-ghalat.

---

### D. `src/heap.c`

#### 1. `is_higher_priority(t_heap_node a, t_heap_node b, int sched)`
- **Chno kadir**: Kat-9aren jouj d les demandes (nodes `a` w `b`) bach t-chouf chkoun 3ndo l-asbaqiya:
  - F **EDF**: Li 3ndo `deadline` sgher (9reb ymot) 3ndo l-asbaqiya.
  - F **FIFO**: Li 3ndo `request_time` sgher (ja lwel) 3ndo l-asbaqiya.
- **Les inputs**: `a`, `b`, `sched` (`SCHEDULER_FIFO` ola `SCHEDULER_EDF`).
- **Chno katretoorner**: `1` ila kan `a` 3ndo l-asbaqiya 3la `b`, sinoun `0`.

#### 2. `heap_init(t_heap *heap, int capacity, int scheduler_type)`
- **Chno kadir**: Kat-allocé la mémoire l tableau d les nodes dyal l-heap w kat-reggli l-capacity w l-mode (`scheduler_type`).
- **Les inputs**: `heap`, `capacity`, `scheduler_type`.
- **Chno katretoorner**: `0` ila nja7, `-1` ila fchel malloc.

#### 3. `heap_push(t_heap *heap, t_heap_node node)`
- **Chno kadir**: Kat-ziid demande jdida f l-heap w kat-tle3ha l l-fou9 (bubble up) hta tched blast-ha l-mounasiba selon l-priorité.
- **Les inputs**: `heap`, `node` (la demande jdida).
- **Chno katretoorner**: `0` ila dkhlat, `-1` ila l-heap 3amra.

#### 4. `heap_peek(t_heap *heap, t_heap_node *out_node)`
- **Chno kadir**: Kat-chouf chkoun howa mol l-asbaqiya l-wla li f rass d l-heap (index 0) bla ma t-ms7o.
- **Les inputs**: `heap`, `out_node` (fin an7oto l-ma3loumat).
- **Chno katretoorner**: `0` ila kan mojoud, `-1` ila kan l-heap khawi.

#### 5. `heap_pop(t_heap *heap, t_heap_node *out_node)`
- **Chno kadir**: Kat-akhod mol l-asbaqiya l-wla (index 0), kat-ms7o men l-heap, w kat-3awed t-rttiib l-heap (bubble down).
- **Les inputs**: `heap`, `out_node`.
- **Chno katretoorner**: `0` ila t-khda, `-1` ila kan l-heap khawi.

---

### E. `src/dongles.c`

#### 1. `can_take_dongles(t_coder *coder, long long now)`
- **Chno kadir**: Kat-tverifi 3 d l-chourout daroriya bach coder yakhod les dongles:
  1. Wach les 2 dongles ma khddamch bihom chi wahed akhor (`!in_use`).
  2. Wach salat l-moudda d l-cooldown dyalhom joj (`now >= last_released_ms + cd`).
  3. Wach had l-coder howa li f rass dyal la file d'attente (l-heap) f les 2 dongles bjoj.
- **Les inputs**: `coder`, `now` (l-wa9t l-7ali b ms).
- **Chno katretoorner**: `1` ila kan y9der yakhodhom b joj, sinoun `0`.

#### 2. `lock_dongles_ordered(t_dongle *d1, t_dongle *d2)`
- **Chno kadir**: Kat-verrouiller (lock) les mutexes d les joj dongles b un ordre mratteb dima (dongle li 3ndo sgher ID howa lwel). Had l-ordre l-mratteb howa li kaymne3 l-Deadlock b 100%.
- **Les inputs**: `d1` (dongle sghir), `d2` (dongle kbir).
- **Chno katretoorner**: Walo (`void`).

#### 3. `unlock_dongles_ordered(t_dongle *d1, t_dongle *d2)`
- **Chno kadir**: Kat-tele9 (unlock) les mutexes b l-3eks d l-ordre li t-verrouillaw bih.
- **Les inputs**: `d1`, `d2`.
- **Chno katretoorner**: Walo (`void`).

#### 4. `acquire_dongles(t_coder *coder)`
- **Chno kadir**: Kat-dkhl la demande d l-coder f les heaps d les dongles dyalo (FIFO ola EDF). Men be3d, katb9a f boucle:
  1. Kat-lock les 2 dongles b `lock_dongles_ordered`.
  2. Kat-chouf b `can_take_dongles` wach les dongles khawyin, salat l-cooldown, w l-coder howa f Top dyal la queue bjoj.
  3. **Ila mzyan**: Katmarki `in_use = 1`, kat-pop men les heaps, kat-tbe3 "has taken a dongle" f joj w katsali.
  4. **Ila machi mzyan (machgulin ola cooldown ola machi f Top)**: Kat-unlock les 2 dongles, kat-n3ess `usleep(500)` (500 microsecondes), w men be3d kat-lock 3awtani. Haka katsali hta l-cooldown wla chi coder ytele9 les dongles bach yfiq.
- **Les inputs**: `coder`.
- **Chno katretoorner**: `0` ila khdahom, `-1` ila w9fat l-simulation.

#### 5. `release_dongles(t_coder *coder)`
- **Chno kadir**: Fach kaysali l-coder men l-compilation, kay-3eyyet 3liha bach y-tele9 les dongles: kat-dir `in_use = 0`, kat-ssjel l-wa9t d l-tlaq f `last_released_ms` bach thseb l-cooldown, w kat-dir `pthread_cond_broadcast` bach tfiyyeq ga3 les coders li kano kaytsennaw had les dongles.
- **Les inputs**: `coder`.
- **Chno katretoorner**: Walo (`void`).

---

### F. `src/coder.c`

#### 1. `execute_compile_phase(t_coder *coder)`
- **Chno kadir**: Kat-kheddem l-marhala d l-compilation:
  1. Kat-3eyyet l `acquire_dongles` bach tched les 2 dongles.
  2. Kat-actualisi `last_compile_start = get_time_ms()` wst `state_mutex`.
  3. Kat-tbe3 "is compiling".
  4. Kat-n3ess b `smart_usleep` l-moudda dyal `time_to_compile`.
  5. Kat-tele9 les dongles b `release_dongles`.
  6. Kat-ziiid `compiles_count++`.
- **Les inputs**: `coder`.
- **Chno katretoorner**: `0` ila daz l-compile mzyan, `-1` ila w9fat l-simulation.

#### 2. `coder_routine(void *arg)`
- **Chno kadir**: L-khedma li kaydir l-thread d kolla coder f boucle. 9bel ma ybda l-boucle:
  - **Ila kan `coder->id % 2 == 0` (coder pair)**: Kaydir `smart_usleep(1ms)` (décalage sghir d 1 milliseconde) bach y-khlli les coders impairs (1, 3, 5...) ydemandaw les dongles lwel w kolchi ybda ycompili f nefs l-lahda bjoj b bjoj (1 w 3 ensemble f t=0, w 2 w 4 f t=1ms men be3d).
  - F l-boucle:
    - Kaycompili (`execute_compile_phase`).
    - Kaydebuggi: kat-tbe3 "is debugging" w kayn3ess `time_to_debug`.
    - Kayrefactori: kat-tbe3 "is refactoring" w kayn3ess `time_to_refactor`.
  - Had l-boucle katb9a t-3awed hta y-w9e3 burnout awla y-salew l-quota.
- **Les inputs**: `arg` (pointeur 3la structure `t_coder`).
- **Chno katretoorner**: `NULL`.

---

### G. `src/monitor.c`

#### 1. `broadcast_all_dongles(t_simulation *sim)`
- **Chno kadir**: Fach katssali l-simulation (b burnout ola b l-quota), kat-fiyyeq ga3 les threads li n3ssin f ay condition variable d ay dongle, bach hta wahed mayb9a mbloqui w ga3 les threads y9dro yssaliw.
- **Les inputs**: `sim`.
- **Chno katretoorner**: Walo (`void`).

#### 2. `check_coder_burnout(t_simulation *sim, int i, long long now)`
- **Chno kadir**: Kat-hseb l-ferq bin l-wa9t l-7ali w `last_compile_start` d l-coder ra9m `i`. Ila l9ato fatha `time_to_burnout`, kat-dir `stop_simulation = 1` w kat-3lem l-programme bli ra w9e3 burnout.
- **Les inputs**: `sim`, `i` (index d l-coder), `now` (l-wa9t l-7ali).
- **Chno katretoorner**: `1` ila mat l-coder, `0` ila ba9i 3ayech.

#### 3. `check_all_compiles_done(t_simulation *sim)`
- **Chno kadir**: Kat-chouf wach ga3 les coders wsslo l `compiles_required`. Ila kano kamlin wsslo, kat-dir `stop_simulation = 1`.
- **Les inputs**: `sim`.
- **Chno katretoorner**: `1` ila salaw kamlin, `0` ila ba9i chi wahed ma salach.

#### 4. `monitor_routine(void *arg)`
- **Chno kadir**: L-boucle d l-3essas (monitor thread):
  - Kolla 1 milliseconde (`usleep(1000)`), kay-locki `state_mutex`.
  - Kay-dwer 3la ga3 les coders b `check_coder_burnout`. Ila mat chi wahed, kay-unlocki, kay-fiyyeq kolchi b `broadcast_all_dongles`, w kay-tbe3 "X burned out".
  - Kay-chouf wach salaw kamlin b `check_all_compiles_done`.
  - Kay-unlocki `state_mutex`.
- **Les inputs**: `arg` (pointeur 3la structure `t_simulation`).
- **Chno katretoorner**: `NULL`.

---

### H. `src/cleanup.c`

#### 1. `is_simulation_stopped(t_simulation *sim)`
- **Chno kadir**: Kat-9ra le flag `stop_simulation` wst `state_mutex` bach te3ref wach l-programme khasso yhabess. Katmne3 data race f l-9raya d le flag.
- **Les inputs**: `sim`.
- **Chno katretoorner**: `1` ila w9fat l-simulation, `0` ila ba9a khddama.

#### 2. `heap_free(t_heap *heap)`
- **Chno kadir**: Kat-dir `free` l tableau d les nodes li f l-heap w kat-rejje3 capacity w size l `0`.
- **Les inputs**: `heap`.
- **Chno katretoorner**: Walo (`void`).

#### 3. `clean_simulation(t_simulation *sim)`
- **Chno kadir**: Kat-détruire ga3 les mutexes (`pthread_mutex_destroy`) w ga3 les condition variables (`pthread_cond_destroy`) dyal les dongles w dyal l-etat w log, kat-libéri les heaps, w kat-dir `free` l tableau d les dongles w tableau d les coders. Haka ma kayb9a hta fuite d mémoire (zero leaks).
- **Les inputs**: `sim`.
- **Chno katretoorner**: Walo (`void`).

---

## 4. Kifach t-lanci l-programme etape b etape (How to run the program step by step)

### Etape 1: Compili l-projet
F l-terminal, dkhoul l dossier d l-projet w kteb:
```bash
make
```
Ghay-tcreea fichier smito `codexion`.

Ila bghiti tmsseh les `.o`:
```bash
make clean
```
Ila bghiti tmsseh kolchi w t3awed t-compili:
```bash
make re
```

---

### Etape 2: Fhem les 8 arguments li khass t3ti l-programme
L-programme kayakhod 8 d les parametres b tartib:
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

1. **`number_of_coders`**: 3adad d les coders (w howa nefsou 3adad d les dongles li kaynin f tabla, men 1 tal 200).
2. **`time_to_burnout`**: L-wa9t b ms li y9der coder yb9a bla compile 9bel ma ymot (burnout).
3. **`time_to_compile`**: Chhal d l-wa9t b ms kaygless l-coder ycompili (fih kaykoun chad 2 dongles).
4. **`time_to_debug`**: Chhal d l-wa9t b ms kaygless ydebuggi.
5. **`time_to_refactor`**: Chhal d l-wa9t b ms kaygless yrefactori.
6. **`number_of_compiles_required`**: Chhal khass kol coder ycompili men merra bach l-programme yssali b nja7.
7. **`dongle_cooldown`**: L-wa9t b ms li khass dongle yb9a mberred men be3d ma ytl9o coder 9bel ma yakhdo coder akhor.
8. **`scheduler`**: L-mode dyal l-asbaqiya: kteb `fifo` (First In, First Out) ola `edf` (Earliest Deadline First).

---

## 5. Amtila d les inputs w les outputs (Example inputs and outputs)

### Mital 1: Coder wahed (Burnout f l-blassa)
Coder wahed 3ndo ghir dongle wahed f l-tabla, walakin khassou 2 dongles bach ycompili. Donc ma y9derch ycompili w ghaymot f `800 ms`.

- **L-Command**:
  ```bash
  ./codexion 1 800 200 200 200 10 0 fifo
  ```

- **L-Output li ghaykhrej**:
  ```text
  800 1 burned out
  ```

---

### Mital 2: 5 Coders normal b FIFO (Kolchi mzyan)
5 coders galsin, kol wahed khassou 2 compiles. L-burnout kbir (2000ms) w ma kaynch cooldown (0ms). Kolchi ghaycompili w yssali bla burnout.

- **L-Command**:
  ```bash
  ./codexion 5 2000 100 100 100 2 0 fifo
  ```

- **L-Output li ghaykhrej (chwiya men l-bedya)**:
  ```text
  0 1 has taken a dongle
  0 1 has taken a dongle
  0 1 is compiling
  100 1 is debugging
  100 2 has taken a dongle
  100 2 has taken a dongle
  100 2 is compiling
  200 1 is refactoring
  200 2 is debugging
  200 3 has taken a dongle
  200 3 has taken a dongle
  200 3 is compiling
  ...
  ```
  *(L-programme kaysali b zero error w bla hta burnout hit kolchi sala 2 compiles).*

---

### Mital 3: 5 Coders b EDF w Cooldown d 50ms
Hna kayn cooldown dyal 50ms 3la kol dongle, w l-asbaqiya kat3ta l-coder li 9reb ymot b l-burnout (`edf`).

- **L-Command**:
  ```bash
  ./codexion 5 3000 200 200 200 3 50 edf
  ```

- **L-Khedma**: L-coder li 3ndo a9rab burnout deadline howa li kayakhod les dongles f l-w9ita li kayberdo fiha.

---

### Mital 4: T-verifi la mémoire b Valgrind
Bach t-t'akked bli ma kayn hta leak f la mémoire:
```bash
valgrind --leak-check=full ./codexion 5 2000 100 100 100 2 0 fifo
```

- **L-Output l-matloub f l-lakher**:
  ```text
  All heap blocks were freed -- no leaks are possible
  ERROR SUMMARY: 0 errors from 0 contexts
  ```

---

### Mital 5: T-verifi les Data Races b Helgrind
Bach t-t'akked bli l-mutexes w thread synchronization khddamin 100% bla data race:
```bash
valgrind --tool=helgrind ./codexion 5 2000 100 100 100 2 0 fifo
```

- **L-Output l-matloub**:
  ```text
  ERROR SUMMARY: 0 errors from 0 contexts
  ```

---

## 6. Legend: Les Fonctions Standard d C li kheddamna bihom (Built-in C Functions)

Hna ghadi ncher7o b tafssil ga3 les fonctions li deja kaynin f l-langage **C** (POSIX pthreads, libc, time...) li kheddamna bihom f had l-projet, chno kaydiro, les arguments dyalhom, w 3lach ssta3melnahom:

---

### A. Les Fonctions d POSIX Threads (`<pthread.h>`)

#### 1. `pthread_create`
- **Chno kadir**: Kat-tkhle9 (spawn) thread jdid f l-programme kaykheddem wahed la fonction f l-arriere-plan (background) en parallèle m3a les autres threads.
- **Les arguments**:
  - `pthread_t *thread`: Pointeur fin kayt7et l-identifiant dyal l-thread jdid.
  - `const pthread_attr_t *attr`: Les options w les attributs d l-thread (derna `NULL` bach yakhod les attributs par défaut).
  - `void *(*start_routine)(void *)`: L-fonction li ghadi ybda biha l-thread khedmtou (bhal `coder_routine` ola `monitor_routine`).
  - `void *arg`: L-donnée li bghina n-passiw l dik la fonction (bhal `&sim->coders[i]` ola `sim`).
- **Chno katretoorner**: `0` ila nja7 l-khlaq d l-thread, awla ra9m d l-erreur ila fchel.
- **Fhad l-projet**: Kheddamna biha bach n-creew thread l kolla coder w thread l l-monitor.

#### 2. `pthread_join`
- **Chno kadir**: Kat-wa99ef l-thread l-7ali (f l-hala dyalna le thread principal `main`) w kat-khellih ytsenna thread akhor hta yssali khedmtou kamla 9bel ma ykemmel.
- **Les arguments**:
  - `pthread_t thread`: L-identifiant d l-thread li bghina ntsennawh yssali.
  - `void **retval`: Pointeur fin yakhod la valeur de retour li retorner l-thread (derna `NULL` hit ma me7tajinch return value).
- **Chno katretoorner**: `0` f nja7, ra9m d l-erreur f l-fachal.
- **Fhad l-projet**: Daroriya bzaf f `join_threads` bach `main` ma yssalich l-programme w y-free la mémoire w les threads ba9in kheddamin f l-khwa.

#### 3. `pthread_mutex_init`
- **Chno kadir**: Kat-initialisi wahed l-verrou (Mutex = Mutual Exclusion). L-mutex howa sarout kaykhlli ghir thread wahed f nefs l-wa9t li 3ndo l-haq ydkhul l wahed la zone critique f l-code.
- **Les arguments**:
  - `pthread_mutex_t *mutex`: Pointeur 3la la variable dyal l-mutex.
  - `const pthread_mutexattr_t *attr`: Attributs dyal l-mutex (`NULL` bach ykoun standard).
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Initialisina biha `state_mutex`, `log_mutex`, w l-mutex d kolla dongle.

#### 4. `pthread_mutex_lock`
- **Chno kadir**: Kat-sedd l-bab b l-verrou. Ila kan l-mutex khawi, l-thread kayakhdo w kaykemmel f l-blassa. Walakin ila kan chi thread akhor deja chaddo, had l-thread kayt-bloqua w kayb9a ytsenna hta lakhor ytele9 l-mutex.
- **Les arguments**:
  - `pthread_mutex_t *mutex`: L-mutex li bghina n-verrouillaw.
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kat-mne3 Data Race. Kheddamna biha fach kanbdlou l-etat d simulation, fach kan-tbe3 f l-ecran, w fach coder kayji yakhod dongle.

#### 5. `pthread_mutex_unlock`
- **Chno kadir**: Kat-7ell l-verrou (kat-tele9 l-sarout) bach ay thread akhor kan kaytsenna y9der ydkhul.
- **Les arguments**:
  - `pthread_mutex_t *mutex`: L-mutex li bghina n-tele9ou.
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Dima fach kansaliw men modification d la variable critique, darori ndiro unlock f l-blassa.

#### 6. `pthread_mutex_destroy`
- **Chno kadir**: Kat-détruire l-mutex w kat-libéri les ressources d l-systeme li kano liés biha.
- **Les arguments**:
  - `pthread_mutex_t *mutex`: L-mutex li bghina n-destructiw.
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kheddamna biha f `clean_simulation` bach n-nettoyew kolchi f l-lakher.

#### 7. `pthread_cond_init`
- **Chno kadir**: Kat-initialisi Condition Variable (`pthread_cond_t`). Howa moyen de signalisation bin les threads: kaykhlli thread yn3es ytsenna signal bla ma y-consommer l-CPU (pas de busy-waiting).
- **Les arguments**:
  - `pthread_cond_t *cond`: Pointeur 3la la condition variable.
  - `const pthread_condattr_t *attr`: Attributs (`NULL` par défaut).
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kol dongle 3ndo condition variable dyalo bach les coders li kaytsennawh yn3so fiha.

#### 8. `pthread_cond_timedwait`
- **Chno kadir**: Kat-n3ess l-thread f la condition variable, walakin m3aha wahed l-moudda d l-wa9t d l-kher (timeout b `timespec`). Fach l-thread kaydkhul fiha, kay-tele9 l-mutex atomiquement. Ghayfiq f jouj d l-halat: ya ima ja signal men thread akhor, ya ima fat l-wa9t (timeout). Fach kayfiq, kay-3awed y-locki l-mutex b zerba.
- **Les arguments**:
  - `pthread_cond_t *cond`: Condition variable fin an-n3so.
  - `pthread_mutex_t *mutex`: L-mutex li tied biha (kayt-tele9 wst n3as w kayt-locka fach nfi9o).
  - `const struct timespec *abstime`: L-wa9t l-absolu li ila wsslat lih l-magana w ma ja hta signal, l-thread yfiq bo7dou.
- **Chno katretoorner**: `0` ila fiy9o signal, `ETIMEDOUT` ila salat l-moudda.
- **Fhad l-projet**: Kheddamna biha wst `acquire_dongles` bach n-gériw l-cooldown w n-evitiw boucle infinie mbloquiya bla signal.

#### 9. `pthread_cond_broadcast`
- **Chno kadir**: Kat-fiyyeq **GA3** les threads li na3ssin f dik la condition variable d9a wehda.
- **Les arguments**:
  - `pthread_cond_t *cond`: La condition variable li fiha threads na3ssin.
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kheddamna biha fach coder kaytele9 les dongles (`release_dongles`) bach ga3 li m-nobbiyen yfiqo ychoufo wach nobethom, w fach l-monitor kayw9e7 burnout bach yfiyyeq ga3 les coders ykherjo cleanly.

#### 10. `pthread_cond_destroy`
- **Chno kadir**: Kat-détruire la condition variable fach kansaliw menha f l-programme.
- **Les arguments**:
  - `pthread_cond_t *cond`: La condition variable.
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kheddamna biha f `clean_simulation`.

---

### B. Les Fonctions d l-Wa9t w N3as (Time & Sleep)

#### 1. `gettimeofday` (`<sys/time.h>`)
- **Chno kadir**: Katjbed l-wa9t l-7ali dyal l-systeme b ddi99a 3alya b l-microsecondes (1 seconde = 1,000,000 microsecondes).
- **Les arguments**:
  - `struct timeval *tv`: Structure fiha `tv_sec` (thawani) w `tv_usec` (microsecondes).
  - `struct timezone *tz`: Zone d l-wa9t (derna `NULL` hit ma me7tajinch fuzo horaire).
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kheddamna biha f `get_time_ms` bach n-calculiw: `(tv.tv_sec * 1000) + (tv.tv_usec / 1000)` w n-rej3o timestamp b l-millisecondes l ga3 les logs w les calculs d burnout.

#### 2. `clock_gettime` (`<time.h>`)
- **Chno kadir**: Kat-3tina l-wa9t l-7ali b nanosecondes (1 seconde = 1,000,000,000 nanosecondes).
- **Les arguments**:
  - `clockid_t clk_id`: No3 d l-horloge (kheddamna b `CLOCK_REALTIME` li howa l-wa9t d l-systeme réel).
  - `struct timespec *tp`: Structure fiha `tv_sec` (secondes) w `tv_nsec` (nanosecondes).
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kheddamna biha f `acquire_dongles` bach n-7essbo `ts` li ghadi n3tiw l `pthread_cond_timedwait`.

#### 3. `usleep` (`<unistd.h>`)
- **Chno kadir**: Kat-wa99ef l-khedma d l-thread l wahed l-moudda sghira m-7essouba b les microsecondes (µs).
- **Les arguments**:
  - `useconds_t usec`: 3adad l-microsecondes (mital: 1000 µs = 1 ms).
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Kheddamna biha wst `smart_usleep` (`usleep(500)`) bach n-n3ssou chwiya b chwiya bla ma n-bloquiw l-programme, w f `monitor_routine` (`usleep(1000)`) bach l-3essas y-tcheki kolla 1 milliseconde bla ma y-helek l-CPU b 100%.

---

### C. Les Fonctions d La Mémoire (`<stdlib.h>`)

#### 1. `malloc`
- **Chno kadir**: Memory Allocation. Kat-reservi wahed l-espace f la mémoire vive (f l-Heap) b 3adad d l-octets (bytes) li tlbti menha, w katretoorner pointeur `void *` 3la l-bedya d dik l-blassa.
- **Les arguments**:
  - `size_t size`: L-hajm b l-octets li bghiti t-allocé (mital: `sizeof(t_coder) * sim->num_coders`).
- **Chno katretoorner**: Pointeur 3la la mémoire l-jdida, awla `NULL` ila ma b9atch la mémoire f l-pc.
- **Fhad l-projet**: Kheddamna biha bach n-allocé tableau d les coders, tableau d les dongles, w tableau d les nodes li wst kol min-heap.

#### 2. `free`
- **Chno kadir**: Kat-tele9 w kat-libéri l-espace mémoire li knti allocitih 9bel b `malloc`.
- **Les arguments**:
  - `void *ptr`: L-adresse d la mémoire li bghiti t-mseh.
- **Chno katretoorner**: Walo (`void`).
- **Fhad l-projet**: Kheddamna biha f `clean_simulation` w `heap_free` bach n-msho ga3 les allocations w n-7esslo 3la `0 leaks` f Valgrind.

---

### D. Les Fonctions d L'Affichage w Les Chaines (`<stdio.h>`, `<string.h>`)

#### 1. `printf`
- **Chno kadir**: Kat-tbe3 texte m-formati f l-sortie standard (`stdout`), bhal les entiers (`%d`), les longs (`%lld`), w les strings (`%s`).
- **Les arguments**:
  - `const char *format`: L-format d l-message.
  - `...`: Les variables li ghadi yt-7etto wst l-format.
- **Chno katretoorner**: 3adad d les caracteres li t-tbe3o.
- **Fhad l-projet**: Kheddamna biha f `print_log` bach n-affichiw les etats: `<timestamp> <coder_id> <status>`.

#### 2. `fflush`
- **Chno kadir**: Kat-fowwed (force flush) l-buffer d l-sortie. Normalement, `printf` katkhlli l-ktaba wst wahed l-tampon (buffer) f la mémoire w ma kat-kherrejhach l l-ecran f l-blassa hta y3mer l-buffer. `fflush(stdout)` kat-fowwdi l-systeme ykherrej l-texte f l-kser d l-lahda.
- **Les arguments**:
  - `FILE *stream`: L-flux li bghina n-vider (hrena derna `stdout`).
- **Chno katretoorner**: `0` f nja7.
- **Fhad l-projet**: Mouhimma bzaf f les applications multithreadées bach l-logs ma yb9awch m3etlin w ybanou b tartib l-haqiqi dyalhom f l-w9ita fash w93o.

#### 3. `strcmp`
- **Chno kadir**: String Compare. Kat-9aren bin jouj d les chaines de caracteres (strings) 7arf b 7arf.
- **Les arguments**:
  - `const char *s1`: L-string l-wla.
  - `const char *s2`: L-string l-tanya.
- **Chno katretoorner**: Kat-retoorner `0` ila kano les deux strings identiques (bhal bhal 100%), awla ra9m ghayr sifr ila kano mbadlin.
- **Fhad l-projet**: Kheddamna biha f `parse_args` bach n-verifiw wach l-argument howa `"fifo"` ola `"edf"`, w f `print_log` bach n-verifiw wach le status howa `"burned out"`.

---

## 7. Chreh Mfessel d les Structures d Données (`includes/codexion.h`)

F had l-projet, 3endna 5 d les structures assasiyin. Hna ghadi ncher7o kol structure chno fiha w kol champ 3lach ssta3melnah:

### 1. `t_heap_node` (Demande d'acquisition d'un dongle)
Howa wahed l-bloc sghir kay-mettel talab (request) dyal coder bgha yahed dongle:
- `int coder_id`: Ra9m d l-coder li tlba l-dongle (men 1 tal N).
- `long long request_time`: L-wa9t b ms fash tlba l-coder had l-dongle. Kay-sta3mel f mode **FIFO** bach n-3erfo chkoun ja lwel.
- `long long deadline`: L-wa9t l-kher 9bel ma ymout had l-coder (`last_compile_start + time_to_burnout`). Kay-sta3mel f mode **EDF** bach n-3tiw l-asbaqiya l-coder li 9reb ymot.

### 2. `t_heap` (Priority Queue / Min-Heap)
Howa la structure li m-responsabla 3la la file d'attente mratba d kol dongle:
- `t_heap_node *nodes`: Tableau dynamique (m-alloué b `malloc`) fih ga3 les demandes li na3ssin kaytsennaw had l-dongle.
- `int capacity`: L-capacity l-maxi dyal had l-heap (dima katsawi `num_coders`).
- `int size`: 3adad d les demandes li kaynin f l-heap f had l-lahda.
- `int scheduler_type`: No3 d l-arbitrage (`SCHEDULER_FIFO` ola `SCHEDULER_EDF`).

### 3. `t_dongle` (USB Hardware Dongle)
Kay-mettel dongle USB hardware wahed f l-tabla:
- `int id`: Ra9m d l-dongle (men 1 tal N).
- `int in_use`: Wach chi coder chad had l-dongle daba (`1`) ola mssali w tal9o (`0`).
- `long long last_released_ms`: Timestamp b ms dyal aakher merra t-tlaq fiha had l-dongle. Kay-sta3mel bach n-7essbo wach salat l-moudda dyal `dongle_cooldown`.
- `pthread_mutex_t mutex`: L-verrou (sarout) li kay7mi had l-dongle w la queue dyalo men ay Data Race.
- `pthread_cond_t cond`: Condition variable fin kay-n3so les threads d les coders li bghaw had l-dongle.
- `t_heap queue`: La file d'attente (Min-Heap) dyal had l-dongle li kat-rttiib chkoun 3ndo l-asbaqiya fih.

### 4. `t_coder` (Développeur / Thread)
Kay-mettel coder wahed f l-simulation:
- `int id`: Ra9m d l-coder (men 1 tal N).
- `int compiles_count`: Chhal men merra compila had l-coder hta l daba.
- `long long last_compile_start`: Timestamp b ms dyal aakher merra bda fiha compilation. Kheddam biha l-monitor bach ychouf wach w9e3 burnout, w kheddama f hissab l-deadline dyal EDF.
- `pthread_t thread`: L-identifiant dyal l-thread POSIX dyal had l-coder.
- `t_dongle *left_dongle`: Pointeur 3la l-dongle li 3la lisr dyalo (`&sim->dongles[i]`).
- `t_dongle *right_dongle`: Pointeur 3la l-dongle li 3la limn dyalo (`&sim->dongles[(i + 1) % num_coders]`).
- `struct s_simulation *sim`: Pointeur 3la la structure l-kbira d l-simulation bach y9der y9ra les options w les timings.

### 5. `t_simulation` (L-Moteur Principal)
Fiha ga3 les informations partagées dyal l-programme kamel:
- `num_coders`: 3adad l-coders w dongles.
- `time_to_burnout`: L-wa9t maxi bla compile 9bel l-burnout.
- `time_to_compile`: Mouddat l-compilation.
- `time_to_debug`: Mouddat l-debugging.
- `time_to_refactor`: Mouddat l-refactoring.
- `compiles_required`: Quota d les compiles bach nssaliw.
- `dongle_cooldown`: Cooldown d les dongles b ms.
- `scheduler`: Mode d l-asbaqiya (`SCHEDULER_FIFO` ola `SCHEDULER_EDF`).
- `stop_simulation`: Flag (0 = khddama, 1 = habess kolchi).
- `start_time`: Timestamp b ms d l-lahda fash bdaw les threads.
- `pthread_mutex_t state_mutex`: Sarout kay7mi `stop_simulation`, `last_compile_start`, w `compiles_count`.
- `pthread_mutex_t log_mutex`: Sarout kay7mi l-affichage f `stdout`.
- `pthread_t monitor_thread`: L-identifiant d l-thread d l-3essas.
- `t_coder *coders`: Tableau d les coders.
- `t_dongle *dongles`: Tableau d les dongles.

---

## 8. Kifach kheddam l-Min-Heap w L-Algorithme d Scheduling (FIFO vs EDF)

### A. 3lach derna Min-Heap f 3oud tableau 3adi?
- F tableau 3adi (Array), ila bghiti t3ref chkoun mol l-asbaqiya (le minimum), khassk t-boucli 3la ga3 les elements: hadchi kayakhod wa9t $O(N)$.
- Walakin f **Min-Heap** (arbre binaire complet m-mettel f tableau):
  - Bach tchouf chkoun mol l-asbaqiya (`heap_peek`): Kaykoun dima f **index 0** -> Kayakhod $O(1)$ f l-blassa.
  - Bach t-ziid talab jdid (`heap_push`) ola t-mseh talab sala (`heap_pop`): Kayakhod $O(\log N)$ hit katsle7 ghir wahed l-branche f l-arbre.
  - Hadchi kaykhlli l-programme ykoun srii3 bzaf w yfout les tests d performance bla ta2khir.

### B. Les Formules d l-Arbre f Tableau
Kol element f index `i` 3ndo:
- **L-Waled dyalo (Parent)**: `(i - 1) / 2`
- **L-Wald li 3la lisr (Left child)**: `2 * i + 1`
- **L-Wald li 3la limn (Right child)**: `2 * i + 2`

### C. Kifach kheddam `heap_push` (Bubble Up):
1. Kanjibou l-talab l-jdid w kan7ottouh f aakher blassa f l-heap (`heap->nodes[heap->size]`).
2. Kan-compariwh m3a l-waled dyalo (`parent = (i - 1) / 2`).
3. Ila kan l-jdid 3ndo asbaqiya kter men bah (priorité kbira), kan-beddlou blast-hom (swap).
4. Kanb9aw n-tel3ouh haka l l-fou9 hta yssal maw9i3 li kay-7tarem l-priorité dyalo ola ywsal l l-qimma (index 0).

### D. Kifach kheddam `heap_pop` (Bubble Down):
1. Kanakhdo l-element li f index 0 (howa mol a3la asbaqiya).
2. Kanjibou aakher element f l-heap w kan7ottouh f index 0, w kan-n9sou `heap->size--`.
3. Kanchoufo wlado bjoj (left w right): chkoun fihom li 3ndo a3la priorité.
4. Ila kan chi wald 3ndo priorité kter men l-waled, kan-beddlou blast-hom (swap) w kan-hebtouh l l-teht hta yched blasto l-mounasiba.

### E. L-Ferq bin FIFO w EDF f `is_higher_priority`:
- **F FIFO (First In, First Out)**:
  - Kanchoufo `request_time`.
  - Coder li tlba l-dongle 9bel (sgher timestamp) howa li 3ndo l-asbaqiya.
- **F EDF (Earliest Deadline First)**:
  - Kanchoufo `deadline` (`last_compile_start + time_to_burnout`).
  - Coder li 3ndo a9rab ajal d l-mot (sgher deadline) howa li kayakhod l-dongle lwel bach n-3et9ouh men l-burnout.
  - Ila kano joj coders 3ndhom nefs l-deadline f nefs l-milliseconde, kan-rej3o n-choufo chkoun fihom li tlba lwel (`request_time`).

---

## 9. As'ila w Ajwiba dyal Soutenance (Peer-Evaluation Q&A)

Had l-as'ila homa li kay-sewlou 3lihom les correcteurs w les pairs f 42 f l-evel:

### Q1: Kifach hresti l-Deadlock b ddi99a? Chno derti m3a les 4 Conditions d Coffman?
**L-Jawab**:
L-Deadlock ma y9der yw9e3 illa ila t-7eqqo 4 d les conditions bjouj f nefs l-wa9t (Conditions de Coffman):
1. **Mutual Exclusion**: Daroriya f l-projet hit dongle hardware ma y9derch ykoun 3nd 2 coders f nefs l-lahda.
2. **No Preemption**: Ma kan-fkouche l-dongle b zzez men coder li rah kaycompili hta kaysali bo7dou.
3. **Hold and Wait**: Hrena coders ma kay-chdouch dongle wahed w yb9aw wa7lin kaytsennaw lakhor b l-mutex. Ila l9a wahed fihom 3amer ola f cooldown, kay-n3ess wst condition variable w ma kayched hta dongle.
4. **Circular Wait (HADI HIYA LI HRRESSNA)**:
   - F 3oud ma kol coder yakhod l-dongle d lisr 3ad d limn (li kat-khelli wahed cycle dayer binathum), hrena derna **Strict Lock Hierarchy** f fonction `lock_dongles_ordered`:
     ```c
     d1 = min(left->id, right->id);
     d2 = max(left->id, right->id);
     pthread_mutex_lock(&d1->mutex);
     pthread_mutex_lock(&d2->mutex);
     ```
   - **Mital**:
     - Coder 1 khasso dongle 1 w 2 -> ghay-locki 1 3ad 2.
     - Coder 5 khasso dongle 5 w 1 -> f 3oud ma y-locki 5 3ad 1, ghay-locki **1 3ad 5**!
   - Hit ga3 les threads kay-verrouillaw f nefs l-ittijah dima (men sgher ID l kbir ID), cycle ma y9derch y-tcreea ga3 f la mémoire -> **Zero Deadlock garanti**.

---

### Q2: Chno kayw9e3 ila kan Coder wahed (`./codexion 1 800 200 200 200 10 0 fifo`)?
**L-Jawab**:
- Fach kaykoun coder wahed, `left_dongle` w `right_dongle` bjoj kay-pointiw 3la **nefs l-dongle** (`dongles[0]`).
- Ila jiti dir `pthread_mutex_lock(&d1->mutex)` w men be3d `pthread_mutex_lock(&d2->mutex)`, ghayw9e3 **Self-Deadlock** (thread kaybloqui raso b raso hit kay-tlba nefs l-mutex li rah deja chaddo).
- **L-Hal f l-code dyalna**:
  - F `acquire_dongles` w `lock_dongles_ordered`:
    ```c
    if (coder->left_dongle == coder->right_dongle)
        d2 = NULL;
    ```
  - Mutex kayt-locka ghir merra wehda.
  - W f `can_take_dongles`:
    ```c
    if (d2 && can_take_dongles(...))
    ```
    Hit `d2 == NULL`, l-coder ma ghay9derch yahed les 2 dongles (hit khassou 2 w kayn ghir 1).
  - Coder 1 kayb9a kaytsenna, hta katwsal 800ms, l-monitor kayfiq w kay-tbe3: `800 1 burned out` w l-programme kaysali cleanly bla crash w bla freeze!

---

### Q3: Kifach kheddam Dongle Cooldown b ddi99a?
**L-Jawab**:
- Fach coder kaysali compilation w kaytele9 les dongles f `release_dongles`:
  ```c
  now = get_time_ms();
  coder->left_dongle->last_released_ms = now;
  coder->right_dongle->last_released_ms = now;
  ```
- Fach chi coder akhor kayji yakhod dongle f `can_take_dongles`:
  ```c
  if (now < left->last_released_ms + cd || now < right->last_released_ms + cd)
      return (0);
  ```
- W bach l-coder li kaytsenna l-cooldown ma yb9ach na3ss f condition variable ila ma ja hta signal akhor, kheddamna b `pthread_cond_timedwait` b wahed l-moudda sghira (5ms). Kolla 5ms kayfiq ychouf wach l-cooldown salat w yakhod l-dongle f l-wa9t l-monasib.

---

### Q4: Kifach derti bach Burnout Detection tkoun f 9ell men 10ms?
**L-Jawab**:
- L-monitor thread f `monitor_routine` 3ndo boucle katdor b `usleep(1000)` (1 milliseconde ghir hiya).
- Kolla 1 milliseconde, kay-locki `state_mutex` w kay-tcheki:
  ```c
  if (now - sim->coders[i].last_compile_start >= sim->time_to_burnout)
  ```
- Hit l-3essas kaydor kolla 1ms, l-moudda d l-khatar bin l-lahda fash kaymot coder w l-lahda fash kayt-tbe3 l-log hiya bin 1ms w 2ms maximum, li hiya bzzaf sgher men 10ms li matlouba f l-projet.

---

### Q5: Kifach mna3ti Data Race bin Monitor w Coders?
**L-Jawab**:
- 3endna `state_mutex`:
  - Kolla coder fach kaybda ycompili, kay-locki `state_mutex`, kay-beddel `last_compile_start = get_time_ms()`, w kay-unlocki.
  - Fach kaysali, kay-locki `state_mutex`, kay-ziiid `compiles_count++`, w kay-unlocki.
  - L-monitor fach kayji y-tcheki l-burnout wla l-quota, kay-locki `state_mutex` 3ad kay9ra had les variables.
- Haka ma ymkench monitor y9ra variable w coder kaykteb fiha f nefs l-lahda -> Helgrind w ThreadSanitizer kay-3tiw **0 data races**.

---

### Q6: Kifach kaysali l-programme cleanly bla fuites mémoire w bla thread yab9a mbloqui?
**L-Jawab**:
1. Fach l-monitor kaychouf burnout ola quota sala:
   - Kaydir `sim->stop_simulation = 1` wst `state_mutex`.
   - Kay-3eyyet l `broadcast_all_dongles(sim)` li kat-fiyyeq ga3 les coders li kano na3ssin f condition variables d les dongles.
2. Ga3 les threads dyal les coders fach kayfiqo, kay-l9aw `is_simulation_stopped(sim) == 1`, kaykherjo men l-boucle w kay-terminaw b `return (NULL)`.
3. L-thread principal `main` kan dayr `pthread_join` l ga3 les threads, kay-tsennahom kamlin yssaliw b tartib.
4. Men be3d, kay-3eyyet l `clean_simulation`:
   - Kat-destructi ga3 les mutexes w les condition variables.
   - Kat-libéri la mémoire d les heaps (`heap_free`), les dongles (`free(sim->dongles)`), w les coders (`free(sim->coders)`).
5. Valgrind kay-3ti: `All heap blocks were freed -- no leaks are possible`.


