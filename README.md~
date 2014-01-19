sensbox
=======

software for arduino shield sensbox

26.10.2012 rev. 31.12.2012
--------------------------

sens_box_serial guida console dettaglio comandi
impostare la porta seriale a: 38400,8,N,1

esempio connessione all'accensione:

LOG CONSOLE                                     COMMENTO
----------------------------------------------- ------------------------------------------
SENS_BOX_SERIAL 20121119 FR:1381                messaggio all'accensione tipo/data versione aaaammgg FR: RAM libera (bytes)
SN:902390-41A7                                  serial number: 5 cifre univoco hardware + 4 cifre univoco chiave accensione
2013-09-06 17:33:26/02/NM+2                     AAAA-MM-GG HH:MM:SS/<lunar phase>/NM+2 (=new moon + 2 days)
SENS:0                                          codice risposta sensore STH1x
V 11.9                                          volt batteria   
RESTORED                                        1. i valori da EEPROM sono stati ripristinati dopo spegnimento
INIT                                            2. i valori EEPROM non erano validi, azzerati dopo la prima accensione
>
>R                                              risposta > e comando R=lettura
T[C]:22.5                                       temperatura [um=C]: <valore>
H[%]:62                                         umidita [RH%]: <valore>
D[C]:15.4                                       condensazione [C]: <valore>
>                                               attesa dall'utente
>V                                              richiesta volt batt
V 11.9                                          risposta: 11.9 volt
>
>
>C 1 Y                                          rele 1:ON
C 1 Y                                           conferma
>                                               
>C 2 N                                          rele 2:OFF
C 2                                             conferma
>                                               
>
>S                                              rele status
C 1 Y
C 2 N
>                                               1 ON, 2 OFF
>
>T 0 1 Y T>= 28                                 memory 0: scatta ad ON il rele 1 se T>=28
OK>
>T 1 1 N T<= 25                                 memory 1: scatta ad OFF il rele 1 se T<=25
OK>
>T 0 1 N T<=25                                  comando non valido, lo spazio separa i parametri
KO>
>
>Z 2                                            memory 2: disattiva
OK>
>Z 9872                                         memory 0..n: disattiva 
M0 FREE
M1 FREE
M2 FREE
M3 FREE
M4 FREE
M5 FREE
M6 FREE
M7 FREE
OK>
>
>M                                              rele memories
M0 1 Y T>= 28.00
M1 1 N T<= 25.00
M2 FREE
M3 4 Y H>= 16.00
M4 0 Y D<= 17.00
M5 1 N D>= 18.00
M6 2 Y T<= 19.00
M7 2 N T>= 20.00
>
>W 8543 1295                                    salva la rele memory in EEPROM con chiave
SAVED
OK>
>
M0 1 Y L>= 300									switch luce ambientale
M0 1 Y V>= 11.50								switch volt batteria
M0 1 Y P>= 22   								switch fase lunare 0=new moon .. 29
>D                                              ripristina i settings rele memory da EEPROM
OK>
>L 1 Y                                          LED 1 ON
L 1 Y
>L 2 Y                                          LED 2 OFF
L 2 Y
>
>L 1 B                                          LED 1 BLINK (freq=1Hz)
L 1 B 
>
BTNLP                                           button 1 (left) press
BTNLR                                           button 1 (left) release
                                                user enter
>B                                              BUZZER ON 0.5 secondi
>
LOWB
LOWB                                            low bat (freq circa 1 al minuto)
>
>
ALERTB                                          la tensione è inferiore ad 11 V il sistema
                                                viene fermato. Segnali: buzzer e leds.
>G                                              richiesta testata identificativa hardware
SENS_BOX_SERIAL 20121119 FR:1381                messaggio all'accensione tipo/data versione aaaammgg FR: RAM libera (bytes)
SN:902390-41A7                                  serial number: 5 cifre univoco hardware + 4 cifre univoco chiave accensione
1999-12-31 00:00:00                             AAAA-MM-GG HH:MM:SS
SENS:0                                          codice risposta sensore STH1x
V:11.9                                          volt batteria 
>
RTC:STOPPED
>
>X												lettura sensore luce in unita-luce-interne
>VL:371

SRAM_FAULT										SRAM/RTC mancante oppure in errore
EEPROM_FAULT                                    EPPROM esterna mancante oppure in errore 
DHT11_FAULT                                     DHT11 sensor mancante oppure in errore  

------------------------ sens_box_serial con RTC e EEPROM 128K
>A                                              imposta tutti i rele ad off
OFF
>E S 10                                         start log temporale, 1 lettura ogni 10 minuti
>E L 0											log status
LOG ON 10 2500 2013-06-12 10:20:30              log status, LOG [ON|OFF] <timings> <nr readings> <last timestamps>
>
>K 2012-11-10 10:20:30 8592                     set  RTC: AAAA-MM-GG HH:MM:SS <key=8592> 
2012-11-10 10:20:30 
>I                                              read RTC: AAAA-MM-GG HH:MM:SS
2012-11-10 10:20:33
>
*						write log

*						write log

>Q 1											simula i 2 bottoni da serial
Q 1                                             Q 1 -> left Q 2 -> right
BTNLP
BTNRP
>
>F                                              legge il log
<log>
 <----- send F
  <n>1,AAAA-MM-GG HH:MM:SS,T[C]:22.5,H[%]:62,D[C]:15.4,C1Y,C2N,C3Y,C4Y,<stato></n>   // stato = numeric, 0=OK, 1=CRC error
 <----- send F
...
 <----- send F
  <n>7,AAAA-MM-GG HH:MM:SS,T[C]:22.5,H[%]:62,D[C]:15.4,V:11.69,L:284,C1Y,C2N,C3Y,C4Y,<stato></n>
 <----- send F
</log>

Nota:
  bisogna inviare un comando F per ogni record che si vuole scaricare
  un numero indica la progressione dei records scaricati
  se si inviano altri comandi mentre si scarica il log questi vengono eseguiti
  se si invia un comando G il log dump viene interrotto


ATT !: ora una procedura automatica verifica lo stato EEPROM
ed esegue queste istruzioni se siamo alla prima accensione
****************************************************************
in fase di installazione del software dare come primo comando il
>Z 9872 per azzerare tutte le memorie RAM che sono indeterminate
poi fare, per inizializzare la EEPROM:
>W 8543 1295
****************************************************************

errori sensore in uscita dopo un comando

esempio:
>R
S_Err_NoACK
>

elenco:
CODICE          DESCRIZIONE
S_Err_NoACK     Error: No response (ACK) received from sensor!
S_Err_CRC       Error: CRC mismatch!
S_Err_TO        Error: Measurement timeout!
S_Err_UNK       Unknown error received!

FUNZIONI TASTIERA
------------------------------------------------
LEFT BTN                               RIGHT BTN

nessuna definita: mandano solo i segnali sulla seriale

FORMATO FILE DI LOG
------------------------------------------------
<log>
  <n>1,AAAA-MM-GG HH:MM:SS,T[C]:22.5,H[%]:62,D[C]:15.4,V:11.69,L:284,C1Y,C2N,C3Y,C4Y,<stato></n>   // stato = codice stato sistema+CRC error
  <n>2,AAAA-MM-GG HH:MM:SS,T[C]:22.5,H[%]:62,D[C]:15.4,V:11.69,L:284,C1Y,C2N,C3Y,C4Y,<stato></n>
  <n>3,AAAA-MM-GG HH:MM:SS,T[C]:22.5,H[%]:62,D[C]:15.4,V:11.69,L:284,C1Y,C2N,C3Y,C4Y,<stato></n>
...
</log>

------------------------------------------------
ESITI TEST ARDUINO
06.11.2012 
OK: interrupt: scattano sempre, mettere resistenze pullup prima dei pulsanti
OK: comandi: G,L yes no blink - OK

inserire in R - lettura valori un valore N/A 
se cè stato un errore sul sensore che non è stato ripristinato
se M da vuoto, segnalarlo

OK:  >W 8543 1295  

OK: superato: in fase di installazione del software dare come primo comando il
>Z 9872 per azzerare tutte le memorie EEPROM

OK:test: 74HC595 e reles

mettere un comando per staccare subito tutti i reles
NON ORA: impostare il giorno della settimana

inizialmente mettere ad OFF tutti i relais
CHECK: rele 4, C 4 Y / C 4 N

Nota programmazione:
per riprogrammare Arduino -> staccare l'alimentazione al modulo bluetooth
modo normale BT -> alimentare il modulo bluetooth

30.03.2013 Nota batteria CR2032: inserire la batteria altrimenti si ha un
           reset da wathdog alla lettura dell'RTC

30.03.2013 TO DO ARDUINO
03.04.2012 OK: test voltage 'V'
03.04.2012 OK: montare sensirion + test
              ATT: alla prima accensione il sensore si calibra, tempo .....
              ATT: un problema su STH11, risponde dati non validi...T40, H100%
06.04.2012 OK: montare light sensor + test
09.04.2012 OK: log su EEPROM
11.04.2013 OK: J 2 -> read log status / parameters
12.04.2013 OK: cmd E - start log
12.04.2013 OK: J 1 -> EXEDUMP EEPROM
12.04.2013 test log dump
13.04.2013 OK: test scrittura/lettura eeprom 00 -> FF su tutta l'area
15.04.2013 OK: escludere IL BYTE AD ADDRESS 00000
13.04.2013 escluso: test scrittura 2 / bytes on eeprom paged
15.04.2013 OK: finire scrittura
16.04.2013 test log library
           test download log xml
16.04.2013 OK: montare nuovi sensori ambientali
09.06.2013 OK: montare lvl converter + test
09.06.2013 OK: montare bluetooth + test
09.06.2013 bug: versione a con hc595 library, non scrive l'ultimo bit e il rele non si muove
09.06.2013 test in corso: exeREAD_LOG
           obiettivi: con progressione circolare convergente eseguire:
		     1. read log -> controllo cmd F
			 2. su failure: indagare sul log writer
		   difetti: 1. rele che non girano se collegati al main board, con RTC = 0
                    2. OK: RTC bloccato 
09.06.2013 OK: su set RTC 'K': mettere scrittura con chiave 8592	
10.06.2013 OK: modificare il comando E
			-->E S <delay min>     log start
			  <--LOG START 2013-08-26 16:09:55
			-->E S 0               log stop / pause
			  <--LOG STOP
			-->E R <key>           log clear con chiave key = 12379
			  <--LOG CLEAR
            -->E L 0	           log status
              1<-LOG OFF <timings> <nr readings>
			  <--LOG OFF 0 0
              2<-LOG ON <timings> <nr readings> <last timestamps>	
              <--LOG ON 2 0 2013-08-26 16:09:55		
              <--WLOG 1 2013-08-26 16:11:55	
              <--LOG ON 2 1 2013-08-26 16:11:55
			  <--LOG OFF 0 1
			  
M0 3 Y L<= 300.00
M1 3 N L>= 500.00
M2 4 Y L>= 380.00
M3 4 N L<= 350.00
M4 1 Y L<= 50.00
M5 1 Y T>= 35.00
M6 FREE
M7 FREE

15.06.2013 OK: comando E
             spuria in lettura sRAM, fare una lettura a vuoto
26.06.2013 OK: slot log circular buffer overwriting
             gli slot del buffer sono fissi di <n> bytes:
             se si riempie sovrascrive esattamente il primo a suo tempo scritto
			 lo slot contiene da se la marca temporale,
			 non ce più il record di testata 'T'
			 lo slot che contiene la marca temporale permette la ricerca da->a
26.06.2013 hardware: att ! arduino impazzisce se ci sono sbalzi di tensione
		   mettere un grosso condensatore sul +12 V
26.06.2013 OK: finire e modificare per scrittura slot e overflow: i2c_write_buffer_24AA1025
26.06.2013 OK: exeREAD_LOG
01.07.2013 proposta: read log F
              F 0 -> read all log
			  F 1 -> <from date> <to date>  
			           <from date> = null | date
					   <to date>   = null | date
			  F 2 -> limit min, max date of recording
01.07.2013 OK: se spengo e riaccendo il log rimane ma cmd F da log vuoto
01.07.2013 OK: inoltre ci vogliono 2 F per chiudere il log
23.08.2013 OK PCB: bug--> collegare A1-19 con PLR3
23.08.2013 OK PCB: fare i pad A-arduino con un foro un po piu grosso (35 mil)
24.08.2013 OK: RTC non avanza dopo essere stato impostato con 'K'
25.08.2013 OK: buttons aggiungere un debounce 5ms
26.08.2013 OK: il log deve avere anche VX, V batt
26.08.2013 OK: un led deve lampeggiare lentamente se il log sta registrando
26.08.2013 OK: fasi lunari
26.08.2013 OK: scatto rele se temp/h/d    0.0 ... 99.99
26.08.2013 OK: scatto rele se luce        0   ... 9999
26.08.2013 OK: scatto rele su v batt      0   ... 99
26.08.2013 OK: scatto rele su fasi lunari 0   ... 29
26.08.2013 OK: logStatusCodeDef sul log al termine deve resettare i singoli flags, CRC error
27.08.2013 OK: solo il giorno dopo: se spengo e riaccendo il log rimane ma cmd F da log vuoto
           in particolare il primo byte 'S' del log diventa 1B da solo
27.08.2013 OK: fare un log da 5 e vedere come fa la sovrascittura
28.08.2013 test: scatto memorie per confronto misure
28.08.2013 OK: se il log è accesso al riavvio deve proseguire con le misure
06.09.2013 attenzione ! arduino con i rele. 
           ce un ordine nelle alimentazioni
           altrimenti si avra 'low battery'
11.09.2013 proposal: fare una modalita passiva invia misure solo se cambiano
11.09.2013 proposal: arduino voltage selector 11->13 @ 12v altrimenti lowb
11.09.2013 OK: pubblicare la shield board su un sito tipo wordpress
19.12.2013 bug: se si forza un rele in chiusura ma ce una regola in M contraria
           il sistema deve segnalare un comando contrario es. C 4 Y -> C 4 N
			 
------------------------------------------------
proposte funzioni arduino
26.1.2012 funzioni aggiuntive arduino:
inserire i power cycle
graph: average, peak, peak, fft, pass-fail
un comando per variare l'SSID bluetooth
un sito: www.ideamicro.in
una versione con comunicazioni GSM
29.1.2012 se ci sono più limiti T>=tx il grafico ne mostrerà il maggiore
proposta board arduino:
sens pressione + 2 pwm + 2 fet + 2 485 + web server
------------------------------------------------
proposte funzioni android
link al sito della board PCB
versione dati demo


------------------------------------------------
ESITI TEST ANDROID
2.1.2013 il pulsante connect se cambia pagina non rimane nello stato impostato
collegati oppure invisible
il pulsante 'collegati' non si collega
4.1.2013 le impostazioni non rimangono sempre: da N/D all'apertura dei settings
4.1.2013 OK CODE UPDATED - in arduino: il comando G non deve rigenerare il numero di serie
5.1.2013 personalizzare il TransitChechBox come extends CheckBox, refactoring
7.1.2013 OK in arduino: definire un comando Q per simulare i 2 bottoni da serial e definirne il significato finale
8.1.2012 OK in arduino: il comando Q 1 -> left Q 2 -> right è cosi fatto:
>Q 1
Q 1
BTNLP
BTNRP
18.1.2012 OK in arduino: definire un comando per il sens status
>U
S_Err_NoACK
SENS:FF                                          codice risposta sensore STH1x
21.1.2012 OK in arduino: nel log, codice stato = numeric
26.1.2012 in arduino: far partire l'app in android 2.1 anche senza collegarsi in BT, in demo
11.9.2013 pubblicare IdraBT su play.google.com


------------------------------------------------
3.2.2012 TO DO ANDROID
- salvare le memory in un file 'mem'+numero di serie
- mettere in magenta se il valore non è confermato
- se collegato inviare il file mem e mostrare a video la conferma con il codice colore
- elimina memoria con dialog di conferma
------------------------------------------------
10.2.2012 WEB SITE
wordpress e-commerce plugin + payments

Attenzione ! un log di questo tipo è un fuori sequenza....
se una data è > della data odierna va eliminato e segnalato....
<log>
 <n>1,2030-04-17 09:47:06,T[C]:13.00,H[%]:61.00,D[C]:5.66,V:10.22,L:506,C1N,C2N,C3N,C4N,16</n>
 <n>2,2013-11-26 18:15:22,T[C]:14.00,H[%]:42.00,D[C]:1.30,V:11.71,L:0,C1Y,C2Y,C3N,C4N,0</n>
 <n>3,2013-11-26 19:15:22,T[C]:14.00,H[%]:42.00,D[C]:1.30,V:10.48,L:128,C1Y,C2Y,C3N,C4N,0</n>
 <n>4,2013-11-26 20:15:22,T[C]:14.00,H[%]:42.00,D[C]:1.30,V:10.79,L:132,C1Y,C2Y,C3N,C4N,0</n>
 <n>5,2013-11-26 21:15:22,T[C]:14.00,H[%]:42.00,D[C]:1.30,V:10.43,L:0,C1Y,C2Y,C3N,C4N,0</n>
 <n>6,2013-11-26 22:15:22,T[C]:14.00,H[%]:42.00,D[C]:1.30,V:11.19,L:0,C1Y,C2Y,C3N,C4N,0</n>
 <n>7,2013-11-26 23:15:22,T[C]:14.00,H[%]:41.00,D[C]:0.96,V:11.14,L:0,C1Y,C2Y,C3N,C4N,0</n>
 <n>8,2013-11-27 00:15:22,T[C]:14.00,H[%]:41.00,D[C]:0.96,V:11.61,L:0,C1Y,C2Y,C3N,C4N,0</n>
 <n>9,2013-11-27 01:15:22,T[C]:13.00,H[%]:41.00,D[C]:0.06,V:11.84,L:0,C1Y,C2Y,C3N,C4N,0</n>
</log>








