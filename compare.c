#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10000
#define ZTE_VS_FILENAME_MAX_LEN 100
char zte_vs_filename_array[N][ZTE_VS_FILENAME_MAX_LEN] = {
	"a.txt",
	"b.txt",
	"c.txt",
	"e.txt",
	"d.txt",
	"e.txt",
};

#define ZTE_VS_FILENAME_HASH_TABLE_SIZE sizeof(zte_vs_filename_array)/sizeof(zte_vs_filename_array[0]) * 10
void *zte_vs_filename_hash_table[ZTE_VS_FILENAME_HASH_TABLE_SIZE] = {NULL,};

unsigned long  BKDHash(const char *str)
{
	unsigned long hash = 0;
	while(*str)
		hash = hash * 131313 + *str++;
	return hash;
}

void zte_vs_filename_hash_table_init()
{
	int i;
	for(i = 0; i < sizeof(zte_vs_filename_array)/sizeof(zte_vs_filename_array[0]); i++) {
		int index = BKDHash(zte_vs_filename_array[i]) % ZTE_VS_FILENAME_HASH_TABLE_SIZE;
		while(zte_vs_filename_hash_table[index] != NULL) {
			index++;
			if(index == ZTE_VS_FILENAME_HASH_TABLE_SIZE)
				index = 0;
		}
		zte_vs_filename_hash_table[index] = zte_vs_filename_array[i];
	}
}

char *zte_search_vs_filename(const char *name)
{
	int index = BKDHash(name) % ZTE_VS_FILENAME_HASH_TABLE_SIZE;
	while(zte_vs_filename_hash_table[index] != NULL) {
		if(strcmp(name, zte_vs_filename_hash_table[index]) == 0)
			return zte_vs_filename_hash_table[index];
		index++;
		if(index == ZTE_VS_FILENAME_HASH_TABLE_SIZE)
			index = 0;
	}
	return NULL;
}

void set_rand_vs_filename()
{
	int i;
	char validchars[] = "abcdefghijklmnopqrstuvwxyz0123456789_.ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int len = strlen(validchars);
	for(i = 0; i < N; i++) {
		int j;
		int rlen = rand() % (ZTE_VS_FILENAME_MAX_LEN -1)+ 1;
		for(j = 0; j < rlen; j++) {
			zte_vs_filename_array[i][j] = validchars[rand()%len];
		}
		zte_vs_filename_array[i][j] = '\0';
	}
}

int main(int argc, char *argv[])
{
	set_rand_vs_filename();
	zte_vs_filename_hash_table_init();
	for(int i =0; i < N; i++) {
		char *p = zte_search_vs_filename(zte_vs_filename_array[i]);
		//printf("%p\n", p);
		
	}
	return 0;
}
