#The pond symbol is used for writing the comments.

#make file overview :--	
#you can add a little description here.

#variable declaration :-
cc=gcc
MAKE=make
RM =rm

#targets .
all: honeychecker.c mainserver.c
	$(cc) -o user user.c
	$(cc) -o mainserver mainserver.c	
	$(cc) -o honeychecker honeychecker.c
	gnome-terminal -t honeychecker -e "./honeychecker"
	gnome-terminal -t mainserver -e "./mainserver"
	sleep 5s 
	$(MAKE) client_target


#another target for client
client_target:
	./user



clean:honeychecker mainserver
	$(RM) honeychecker
	$(RM) mainserver