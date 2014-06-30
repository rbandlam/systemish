static void cuckoo_forward_burst_2(struct lcore_params *lp,
									struct rte_mbuf *m1,
									struct rte_mbuf *m2,
									uint8_t port,
									uint32_t socket)
{
	
	
	m1_ether_addr[I] = 0, m2_ether_addr[I] = 0, m3_ether_addr[I] = 0, m4_ether_addr[I] = 0;
	

	for (i[I] = 0; i[I] < 6; i[I]++)
		m1_ether_addr[I] = (m1_ether_addr[I] << 8) | (eth[I]->d_addr.addr_bytes[i[I]]);
	for (i[I] = 0; i[I] < 6; i[I]++)
		m2_ether_addr[I] = (m2_ether_addr[I] << 8) | (eth[I]->d_addr.addr_bytes[i[I]]);

	st[I] = cuckoo_find_burst_2(h,
						m1_ether_addr[I], &m1_outport[I],
						m2_ether_addr[I], &m2_outport[I]);
	assert(st[I] == ok);

	m1_outport[I] = (m1_outport[I] & 3) + socket * 4;
	m2_outport[I] = (m2_outport[I] & 3) + socket * 4;


	send_packet(lp, m1, m1_outport[I]);
	send_packet(lp, m2, m2_outport[I]);
}
