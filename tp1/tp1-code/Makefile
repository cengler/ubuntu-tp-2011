export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_mfq.o
MAIN=simusched

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_mfq.h
sched_fcfs.o: sched_fcfs.cpp sched_fcfs.h basesched.h
sched_mfq.o: sched_mfq.cpp sched_mfq.h basesched.h
sched_rr.o: sched_rr.cpp sched_rr.h basesched.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

SchedRRTest: SchedRRTest.java
	javac SchedRRTest.java

clean:
	rm -f $(OBJS) $(MAIN)
	rm -f *.png
	rm -f *.log
	rm -f *.out
	rm -f *.class

