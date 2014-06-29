// Process BATCH_SIZE pkts starting from lo
int process_pkts_in_batch(int *pkt_lo)
{
	// Like a foreach loop
	for(batch_index = 0; batch_index < BATCH_SIZE; batch_index ++) {

		volatile struct rte_mbuf *a;
		int a1 = 1, a2 = 1, a3 = 1;
		a1 = 5;
		
		static volatile int **ptrs[5];
		
		unsigned int a_1 = hash(pkt_lo[batch_index]) & LOG_CAP_;
		
		int a_2 = hash(a_1) & LOG_CAP_;
		sum += ht_log[a_20];
    }   
}