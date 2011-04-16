rm -f SchedRRTest.out
for i in 40
do
	for j in 1 2 3 4 5
	do
		./simusched ejercicio7.tsk 1 SchedRR $i | java SchedRRTest >> SchedRRTest.out
	done
done
