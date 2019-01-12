all: main.o
main.o:Karta.cpp Karta.h main.cpp Hra.cpp Hra.h Hrac.cpp Hrac.h Kopka.cpp Kopka.h Menu.cpp Menu.h
	gcc main.cpp Karta.cpp Hra.cpp Hrac.cpp Kopka.cpp Menu.cpp -lstdc++ -o main
run: main.o
	./main.o
