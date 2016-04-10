CLIBS= 		-lm -lglut -lGL -lGLU -lX11 -pthread # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include # 2011
CPATHS=		-I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)

Kickham_Gillis_G4:		Kickham_Gillis_G4.cpp
		g++ Kickham_Gillis_G4.cpp -o Kickham_Gillis_G4 $(CFLAGS)

workingProgress:		workingProgress.cpp
		g++ workingProgress.cpp -o workingProgress $(CFLAGS)

clean:
		rm *.o
