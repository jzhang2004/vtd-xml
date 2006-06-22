all : vtd-xml.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o
	gcc -o vtd-xml fastIntBuffer.o arrayList.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o vtd-xml.o
clean:
	del *.o *.exe

vtd-xml.o : vtd-xml.c
	gcc -c -fshort-wchar vtd-xml.c

arrayList.o : arrayList.c
	gcc -c arrayList.c
	
fastIntBuffer.o : fastIntBuffer.c
	gcc -c fastIntBuffer.c

fastLongBuffer.o : fastLongBuffer.c
	gcc -c fastLongBuffer.c

contextBuffer.o : contextBuffer.c
	gcc -c contextBuffer.c    

vtdGen.o : vtdGen.c
	gcc -c vtdGen.c
	
vtdNav.o : vtdNav.c
	gcc -c -fshort-wchar vtdNav.c

autoPilot.o : autoPilot.c
	gcc -c autoPilot.c

XMLChar.o : XMLChar.c
	gcc -c XMLChar.c	

