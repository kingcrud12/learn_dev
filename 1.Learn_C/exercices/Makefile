# Makefile generique pour les exercices du module C.
#
#   make calculator     compile calculator.c  -> calculator
#   make                compile TOUS les .c du dossier
#   make run P=maths    compile maths.c puis l'execute
#   make clean          supprime les binaires et les .dSYM

CC      = gcc
CFLAGS  = -Wall -Wextra -g
LDFLAGS = -lm

# Tous les .c du dossier -> la liste des binaires correspondants
SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=)

# Cible par defaut : construit tout
all: $(TARGETS)

# LA REGLE GENERIQUE
# %  est un joker : "n'importe quel nom"
# $< est la premiere dependance  (le .c)
# $@ est le nom de la cible       (le binaire)
%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Compile puis execute : make run P=calculator
run: $(P)
	./$(P)

clean:
	rm -f $(TARGETS)
	rm -rf *.dSYM

.PHONY: all run clean
