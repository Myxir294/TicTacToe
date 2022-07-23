#
#  To sa opcje dla kompilacji
#
CPPFLAGS= -Wall -pedantic -o

kolko_i_krzyzyk: main.o funkcje_pomocnicze.o drzewo.o ruch.o
	g++ ${CPPFLAGS} kolko_i_krzyzyk *.o
                                   
main.o: main.cpp funkcje_pomocnicze.hh drzewo.hh ruch.hh
	g++ -c ${CPPFLAGS} main.o main.cpp

funkcje_pomocnicze.o: funkcje_pomocnicze.cpp funkcje_pomocnicze.hh
	g++ -c ${CPPFLAGS} funkcje_pomocnicze.o funkcje_pomocnicze.cpp

drzewo.o: drzewo.cpp drzewo.hh ruch.hh 
	g++ -c ${CPPFLAGS} drzewo.o drzewo.cpp

ruch.o: ruch.cpp ruch.hh
	g++ -c ${CPPFLAGS} ruch.o ruch.cpp

clean:
	rm *.o kolko_i_krzyzyk