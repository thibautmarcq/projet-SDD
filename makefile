# MONO 3
# Maëlle LIU 21204734
# Thibaut MARCQ 21202966

CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
# CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = chaineMain reconstitueReseau mainComparaison reconstitueReseauBIS mainGraphe

.PHONY:	all clean resclean

all: $(PROGRAMS)

chaineMain: SVGwriter/SVGwriter.o Chaines/Chaine.o chaineMain.c 
	$(CC) -o $@ $(CFLAGS) $^ -lm

reconstitueReseau:  SVGwriter/SVGwriter.o Chaines/Chaine.o Reseau/Reseau.o Hachage/Hachage.o ArbreQuat/ArbreQuat.o reconstitueReseau.c
	$(CC) -o $@ $(CFLAGS) $^ -lm

mainComparaison: SVGwriter/SVGwriter.o Chaines/Chaine.o Reseau/Reseau.o Hachage/Hachage.o ArbreQuat/ArbreQuat.o mainComparaison.c
	$(CC) -o $@ $(CFLAGS) $^ -lm

reconstitueReseauBIS:  SVGwriter/SVGwriter.o Chaines/Chaine.o Reseau/Reseau.o Hachage/Hachage.o ArbreQuat/ArbreQuat.o reconstitueReseauBIS.c
	$(CC) -o $@ $(CFLAGS) $^ -lm

mainGraphe: File/Struct_File.o File/Graphe.o mainGraphe.c
	$(CC) -o $@ $(CFLAGS) $^

####

SVGwriter/SVGwriter.o: SVGwriter/SVGwriter.c
	$(CC) $(CFLAGS) -c $< -o $@

Chaines/Chaine.o: Chaines/Chaine.c
	$(CC) $(CFLAGS) -c $< -o $@ -lm

Reseau/Reseau.o: Reseau/Reseau.c
	$(CC) $(CFLAGS) -c $< -o $@ -lm

Hachage/Hachage.o : Hachage/Hachage.c
	$(CC) $(CFLAGS) -c $< -o $@ -lm

ArbreQuat/ArbreQuat.o : ArbreQuat/ArbreQuat.c
	$(CC) $(CFLAGS) -c $< -o $@

File/Graphe.o : File/Graphe.c
	$(CC) $(CFLAGS) -c $< -o $@

File/Struct_File.o : File/Struct_File.c
	$(CC) $(CFLAGS) -c $< -o $@

#Ou plus simplement
#%.o:%.c %.h
#	$(CC)  $(GCC_FLAGS) -c  $<


clean:
	rm -f *.o *~ $(PROGRAMS)
	rm -f SVGwriter/*.o Chaines/*.o Reseau/*.o Hachage/*.o ArbreQuat/*.o File/*.o
	rm -f *.html *.txt
	
resclean:
	rm -f *.html *.txt