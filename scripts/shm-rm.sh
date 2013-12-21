for i in `seq 0 32`; do
	sudo ipcrm -M $i
done
sudo ipcrm -M 3185		#Writer's hash table
sudo ipcrm -M 3006		#Writer's hash table
