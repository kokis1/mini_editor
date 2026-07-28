kilo: kilo.c
	$(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=c99 -Wno-strict-prototypes

modular: main.c
	$(CC) main.c -o modular_kilo -Wall -Wextra -pedantic -std=c99 -Wno-strict-prototypes