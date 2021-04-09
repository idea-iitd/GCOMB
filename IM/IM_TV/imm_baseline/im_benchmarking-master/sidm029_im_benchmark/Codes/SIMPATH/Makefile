CC=g++
CFLAGS=-c -O3 -std=c++0x 
DEBFLAGS=-c -g
LEADERSPATH=../leaders-3.0
OFLAGS=-DPARSEINPUT_H=1

all: InfluenceModels

InfluenceModels: common.o anyoption.o LTTest.o MC.o LDAG.o LTSimplePath.o Baseline.o InfluenceModels.o 
	$(CC) -O3 common.o anyoption.o LTTest.o MC.o LDAG.o LTSimplePath.o Baseline.o InfluenceModels.o -o InfluenceModels

common.o: common.cc
	$(CC) $(CFLAGS) common.cc	

anyoption.o: anyoption.cc
	$(CC) $(CFLAGS) anyoption.cc 

LTTest.o: LTTest.cc
	$(CC) $(CFLAGS) LTTest.cc

MC.o: MC.cc
	$(CC) $(CFLAGS) MC.cc

LDAG.o: LDAG.cc
	$(CC) $(CFLAGS) LDAG.cc

LTSimplePath.o: LTSimplePath.cc
	$(CC) $(CFLAGS) LTSimplePath.cc

Baseline.o: Baseline.cc
	$(CC) $(CFLAGS) Baseline.cc

InfluenceModels.o: InfluenceModels.cc
	$(CC) $(CFLAGS) InfluenceModels.cc

debug: common_debug.o anyoption_debug.o LTTest_debug.o MC_debug.o LDAG_debug.o LTSimplePath_debug.o Baseline_debug.o InfluenceModels_debug.o
	$(CC) -g common.o anyoption.o LTTest_debug.o MC.o LDAG.o LTSimplePath.o Baseline.o InfluenceModels.o -o InfluenceModels_debug

common_debug.o: common.cc
	$(CC) $(DEBFLAGS) common.cc

anyoption_debug.o: anyoption.cc
	$(CC) $(DEBFLAGS) anyoption.cc 

LTTest_debug.o: LTTest.cc
	$(CC) $(DEBFLAGS) LTTest.cc

MC_debug.o: MC.cc
	$(CC) $(DEBFLAGS) MC.cc

LDAG_debug.o: LDAG.cc
	$(CC) $(DEBFLAGS) LDAG.cc

LTSimplePath_debug.o: LTSimplePath.cc
	$(CC) $(DEBFLAGS) LTSimplePath.cc

Baseline_debug.o: Baseline.cc
	$(CC) $(DEBFLAGS) Baseline.cc

InfluenceModels_debug.o: InfluenceModels.cc
	$(CC) $(DEBFLAGS) InfluenceModels.cc

clean:
	rm -f *.o *~

