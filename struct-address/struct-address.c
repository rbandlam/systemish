#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

struct KV {
	long long key;
	long long value;
	long long location;
	long long req_num;
};

main()
{
	printf("Struct size: %d\n", (int) sizeof(struct KV));
	struct KV *kv = (struct KV*) memalign(4096, sizeof(struct KV));
	printf("Addresses: %p %p %p %p\n",
		&kv->key, &kv->value, &kv->location, &kv->req_num);
}

