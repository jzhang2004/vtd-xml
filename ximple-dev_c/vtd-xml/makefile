CC=gcc
 
CFLAGS= -c -O3 -Wall -fomit-frame-pointer -fforce-addr -frerun-cse-after-loop -fexpensive-optimizations -fregmove -frerun-loop-opt -fmerge-all-constants -fno-branch-count-reg -funroll-loops -fpeephole -march=i686
CFLAGS2 = -c -ggdb
LDFLAGS = -O3 -fomit-frame-pointer -fforce-addr -frerun-cse-after-loop -fexpensive-optimizations -fregmove -frerun-loop-opt -march=pentium -lm 
LDFLAGS2  = -ggdb

all :  vtd-xml RSSReader stats soap benchmark_vtdxml update indexWriter indexReader testNodeRecorder testOverwrite testBookMark

benchmark_vtdxml: benchmark_vtdxml.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

soap: soap.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

stats: stats.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

RSSReader: RSSReader.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

update: update.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

indexWriter: indexWriter.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

indexReader: indexReader.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o
	
testNodeRecorder: testNodeRecorder.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

testOverwrite: testOverwrite.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

testBookMark: testBookMark.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o

vtd-xml: vtd-xml.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o bookMark.o


	${CC} ${LDFLAGS} -o vtd-xml vtd-xml.o fastIntBuffer.o arrayList.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o unionExpr.o decoder.o XMLModifier.o nodeRecorder.o indexHandler.o

clean:
	-rm *.o soap RSSReader vtd-xml stats

benchmark_vtdxml.o: example/benchmark_vtdxml.c
	${CC} ${CFLAGS} example/benchmark_vtdxml.c

vtd-xml.o: example/vtd-xml.c
	${CC} ${CFLAGS} example/vtd-xml.c

RSSReader.o : example/RSSReader.c
	${CC} ${CFLAGS} example/RSSReader.c

update.o : example/update.c
	${CC} ${CFLAGS} example/update.c

stats.o : example/stats.c
	${CC} ${CFLAGS} example/stats.c

soap.o	: example/soap.c
	${CC} ${CFLAGS} example/soap.c

indexWriter.o	: example/indexWriter.c 
	${CC} ${CFLAGS} example/indexWriter.c

indexReader.o	: example/indexReader.c 
	${CC} ${CFLAGS} example/indexReader.c

testNodeRecorder.o	: example/testNodeRecorder.c
	${CC} ${CFLAGS} example/testNodeRecorder.c

testOverwrite.o	: example/testOverwrite.c
	${CC} ${CFLAGS} example/testOverwrite.c

testBookMark.o 	: example/testBookMark.c
	${CC} ${CFLAGS} example/testBookMark.c

bookMark.o : bookMark.c
	${CC} ${CFLAGS} bookMark.c

nodeRecorder.o	: nodeRecorder.c
	${CC} ${CFLAGS} nodeRecorder.c	

indexHandler.o	: indexHandler.c
	${CC} ${CFLAGS} indexHandler.c

decoder.o : decoder.c
	${CC} ${CFLAGS} decoder.c

intHash.o : intHash.c
	${CC} ${CFLAGS} intHash.c

arrayList.o : arrayList.c
	${CC} ${CFLAGS} arrayList.c
	
fastIntBuffer.o : fastIntBuffer.c
	${CC} ${CFLAGS} fastIntBuffer.c

fastLongBuffer.o : fastLongBuffer.c
	${CC} ${CFLAGS} fastLongBuffer.c

contextBuffer.o : contextBuffer.c
	${CC} ${CFLAGS} contextBuffer.c    

vtdGen.o : vtdGen.c
	${CC} ${CFLAGS} vtdGen.c
	
vtdNav.o : vtdNav.c
	${CC} ${CFLAGS} vtdNav.c

autoPilot.o : autoPilot.c
	${CC} ${CFLAGS} autoPilot.c

XMLModifier.o : XMLModifier.c
	${CC} ${CFLAGS} XMLModifier.c
	

XMLChar.o : XMLChar.c
	${CC} ${CFLAGS} XMLChar.c	

unionExpr.o : unionExpr.c
	${CC} ${CFLAGS} unionExpr.c

filterExpr.o : filterExpr.c
	${CC} ${CFLAGS} filterExpr.c

literalExpr.o : literalExpr.c
	${CC} ${CFLAGS} literalExpr.c

numberExpr.o : numberExpr.c
	${CC} ${CFLAGS} numberExpr.c

unaryExpr.o : unaryExpr.c
	${CC} ${CFLAGS} unaryExpr.c

locationPathExpr.o : locationPathExpr.c
	${CC} ${CFLAGS} locationPathExpr.c

pathExpr.o : pathExpr.c
	${CC} ${CFLAGS} pathExpr.c

lex.yy.o : lex.yy.c
	${CC} ${CFLAGS} lex.yy.c

l8.tab.o : l8.tab.c
	${CC} ${CFLAGS} l8.tab.c

funcExpr.o : funcExpr.c
	${CC} ${CFLAGS} funcExpr.c

helper.o : helper.c
	${CC} ${CFLAGS} helper.c
