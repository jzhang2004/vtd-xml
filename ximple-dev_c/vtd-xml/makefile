CC=gcc
 
CFLAGS= -c -O3 -Wall -fomit-frame-pointer -fforce-addr -frerun-cse-after-loop -fexpensive-optimizations -fregmove -frerun-loop-opt -march=i686 
CFLAGS2 = -c -ggdb
LDFLAGS = -O3 -fomit-frame-pointer -fforce-addr -frerun-cse-after-loop -fexpensive-optimizations -fregmove -frerun-loop-opt -march=k6 -lm 
LDFLAGS2  = -ggdb

all :  vtd-xml RSSReader stats soap benchmark_vtdxml

benchmark_vtdxml: benchmark_vtdxml.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o

soap: soap.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o

stats: stats.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o

RSSReader: RSSReader.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o

vtd-xml: vtd-xml.o arrayList.o fastIntBuffer.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o

	${CC} ${LDFLAGS} -o vtd-xml vtd-xml.o fastIntBuffer.o arrayList.o fastLongBuffer.o contextBuffer.o vtdNav.o vtdGen.o autoPilot.o XMLChar.o helper.o lex.yy.o l8.tab.o literalExpr.o numberExpr.o pathExpr.o filterExpr.o binaryExpr.o unaryExpr.o funcExpr.o locationPathExpr.o intHash.o

clean:
	-rm *.o soap RSSReader vtd-xml stats

benchmark_vtdxml.o: benchmark_vtdxml.c
	${CC} ${CFLAGS} benchmark_vtdxml.c

vtd-xml.o : vtd-xml.c
	${CC} ${CFLAGS} vtd-xml.c

RSSReader.o : RSSReader.c
	${CC} ${CFLAGS} RSSReader.c

stats.o : stats.c
	${CC} ${CFLAGS} stats.c

soap.o	: soap.c
	${CC} ${CFLAGS} soap.c

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

XMLChar.o : XMLChar.c
	${CC} ${CFLAGS} XMLChar.c	

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
