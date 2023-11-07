# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = theboys.o conjunto.o  lef.o fila.o 
     
all: theboys testa_conjunto testa_fila testa_lef

theboys: theboys.o conjunto.o  lef.o fila.o
	$(CC) -o theboys theboys.o conjunto.o  lef.o fila.o $(LDFLAGS)

testa_conjunto: testa_conjunto.o conjunto.o
	$(CC) -o testa_conjunto testa_conjunto.o conjunto.o $(LDFLAGS)

conjunto.o: conjunto.c
	$(CC) -c $(CFLAGS) conjunto.c

testa_conjunto.o: testa_conjunto.c
	$(CC) -c $(CFLAGS) testa_conjunto.c

testa_lef: testa_lef.o lef.o
	$(CC) -o testa_lef testa_lef.o lef.o $(LDFLAGS)

lef.o: lef.c
	$(CC) -c $(CFLAGS) lef.c

testa_lef.o: testa_lef.c
	$(CC) -c $(CFLAGS) testa_lef.c

testa_fila: testa_fila.o fila.o
	$(CC) -o testa_fila testa_fila.o fila.o $(LDFLAGS)

fila.o: fila.c
	$(CC) -c $(CFLAGS) fila.c

testa_fila.o: testa_fila.c
	$(CC) -c $(CFLAGS) testa_fila.c

theboys.o: theboys.c
	$(CC) -c $(CFLAGS) theboys.c

clean:
	rm -f $(objects) theboys


