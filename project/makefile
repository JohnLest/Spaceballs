.SILENT:

GRILLESDL=GrilleSDL
RESSOURCES=Ressources
ECRAN=Ecran
BILLE=Bille
TOOLS=Tools

CC = gcc -DKNOPPIX -I$(ECRAN) -I$(GRILLESDL) -I$(RESSOURCES) -I /opt/guengel/include/SDL
OBJS = $(GRILLESDL)/GrilleSDL.o $(RESSOURCES)/Ressources.o $(ECRAN)/Ecran.o $(BILLE)/Bille.o $(TOOLS)/Tools.o
PROGRAMS = SpaceBalls

ALL: $(PROGRAMS)

SpaceBalls:	SpaceBalls.c $(OBJS)
	echo Creation de SpaceBalls...
	$(CC) SpaceBalls.c -o SpaceBalls $(OBJS) -lrt -lpthread -lSDL

$(GRILLESDL)/GrilleSDL.o:	$(GRILLESDL)/GrilleSDL.c $(GRILLESDL)/GrilleSDL.h
		echo Creation de GrilleSDL.o ...
		$(CC) -c $(GRILLESDL)/GrilleSDL.c
		mv GrilleSDL.o $(GRILLESDL)

$(RESSOURCES)/Ressources.o:	$(RESSOURCES)/Ressources.c $(RESSOURCES)/Ressources.h
		echo Creation de Ressources.o ...
		$(CC) -c $(RESSOURCES)/Ressources.c
		mv Ressources.o $(RESSOURCES)

$(ECRAN)/Ecran.o:	$(ECRAN)/Ecran.c $(ECRAN)/Ecran.h
		$(CC) $(ECRAN)/Ecran.c -c
		mv Ecran.o $(ECRAN)/Ecran.o

$(BILLE)/Bille.o:	$(BILLE)/Bille.c $(BILLE)/Bille.h
		$(CC) $(BILLE)/Bille.c -c
		mv Bille.o $(BILLE)/Bille.o

$(TOOLS)/Tools.o:	$(TOOLS)/Tools.c $(TOOLS)/Tools.h
		$(CC) $(TOOLS)/Tools.c -c
		mv Tools.o $(TOOLS)/Tools.o

clean:
	@rm -f $(OBJS) core

clobber:	clean
	@rm -f tags $(PROGRAMS)
