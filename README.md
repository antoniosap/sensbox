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

