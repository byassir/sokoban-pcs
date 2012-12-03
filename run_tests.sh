make

#Sequential execution
echo 'Start' > 'results_seq.txt'
for (( i = 1 ; i <= 100 ; i++ ))
do
    str=`./sokoban_seq test_boards/board$i.txt`
    echo $i' '$str >> 'results_seq.txt'
done

#Execution using C++11 threads
for (( i = 3 ; i <= 48 ; i *= 2 ))
do
    out_file='results_threads_'$i'.txt'
    echo 'Start' > $out_file
    for (( j = 1 ; j <= 100 ; j++ ))
    do
        str=`./sokoban_threads test_boards/board$j.txt $i`
        echo $j' '$str >> $out_file
    done
done

#Execution using OpenMP
for (( i = 3 ; i <= 48 ; i *= 2 ))
do
    out_file='results_omp_'$i'.txt'
    echo 'Start' > $out_file
    for (( j = 1 ; j <= 100 ; j++ ))
    do
        str=`OMP_NUM_THREADS=$i ./sokoban_omp test_boards/board$j.txt`
        echo $j' '$str >> $out_file
    done
done

