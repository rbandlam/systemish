# Compute the Nth percentile of a dataset

if [ "$#" -ne 2 ]; then
    echo "Illegal number of parameters"
	echo "Usage: ./percentile.sh <filename> <percentile>"
	exit
fi

echo "Computing $2th percentile of file $1"
cat $1 | sort -n | awk -v percentile=$2 \
	'BEGIN{c = 0} {total[c] = $1; c ++;} END{print total[int(NR * (percentile / 100.0) - 0.5)]}'
