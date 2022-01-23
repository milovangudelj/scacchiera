# Scacchiera
Progetto finale del corso di "Laboratorio di Programmazione"  - 2021/2022

## Autori
[Yihui Zhu](https://github.com/Aehronburn)  
[Tommaso Jin](https://github.com/JiNT1)  
[Milovan Gudelj](https://github.com/milovangudelj)  

## Requisiti e compilazione
Per compilare il coodice è necessario aver installato [CMake](https://cmake.org/) sul proprio computer.

Successivamente basterà eseguire i seguenti comandi per compilare e generare i due eseguibili `scacchiera` e `replay` in una cartella `build`.

```sh
cmake . -B build/
cmake --build build/
```

## Esecuzione
Dopo la compilazione verranno generati due eseguibili nella cartella `build`.

- **`scacchiera`**  
	Accetta due parametri:
	- `[modalità/help]` - La modalità di gioco o la stampa del messaggio di aiuto. Il parametro può essere uno dei seguenti: *pc*, *cc*, *pp*, *help*. Dove **p** sta per player, e **c** per computer.
	- `(fen)` - *(facoltativo)* - Una stringa rappresentante la configurazione di partenza della scacchiera. Viene utilizzata la [Forsyth–Edwards Notation](https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation) (FEN).

	Esempio:
	```sh
	./build/scacchiera cc "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
	```
- **`replay`**  
	Accetta tre parametri:
	- `[destinazione]` - La destinazione dell'output del replay. Deve essere un carattere tra i seguenti: v, f. Rispettivamente per video e file.
	- `[log_file]` - Un file `.txt` di log di una partita di cui si vuole vedere il replay. Il file deve essere generato da `scacchiera`.
	- `(output_file)` - *(facoltativo)* - Nome del file `.txt` di output. Da specificare solo nel caso in cui si voglia stampare il replay su file anziché a terminale.

	Esempio:
	```sh
	./build/replay f history.txt output.txt
	```

## Note
Nel codice si fa uso estensivo dei caratteri ANSI/VT100 di controllo del terminale. Questo per formattare in maniera più efficace l'output nella finestra del terminale. I valori non sono dinamici, ma sono stati calcolati in base all'altezza fissa delle varie stampe della scacchiera.

Un esempio:
```cxx
std::cout << "\033[14A" << "\033[J";
```
In questo caso con il primo codice si sposta il cursore in alto di `14` righe, e con il secondo si cancella tutto il contenuto del terminale successivo alla posizione del cursore.