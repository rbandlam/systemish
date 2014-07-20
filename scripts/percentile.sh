# Compute the Nth percentile of a dataset

if [ "$#" -lt 2 ]; then
    echo "Illegal number of parameters"
	echo "Usage: ./percentile.sh <filename> <percentile1> <percentile2> ... "
	exit
fi

# Iterate over the percentiles requested
for i in `seq 2 $#`; do 
	echo "Computing ${!i} th percentile of file $1"
	cat $1 | sort -n | awk -v percentile=${!i} \
		'BEGIN{c = 0} {total[c] = $1; c ++;} END{print total[int(NR * (percentile / 100.0) - 0.5)]}'
done
