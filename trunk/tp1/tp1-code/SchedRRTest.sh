rm -f SchedRRTest.count
for i in 1 5 10 15 20 25 30 35
do
	eval png=SchedRRTest$i.png
	eval out=SchedRRTest$i.out
	./simusched ejercicio7.tsk 1 SchedRR $i > $out
	tail -n 1000 $out | java SchedRRTest >> SchedRRTest.count
	tail -n 1000 $out | python graphsched.py > $png
done
