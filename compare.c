#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define ZTE_VS_FILE_NUM 10000
#define ZTE_VS_FILENAME_MAX_LEN 100
char zte_vs_filename_array[ZTE_VS_FILE_NUM][ZTE_VS_FILENAME_MAX_LEN] = {
	"a.txt",
	"b.txt",
	"c.txt",
	"e.txt",
	"d.txt",
	"e.txt",
};

#define ZTE_VS_FILENAME_HASH_TABLE_SIZE  (ZTE_VS_FILE_NUM * 10)
void *zte_vs_filename_hash_table[ZTE_VS_FILENAME_HASH_TABLE_SIZE] = {NULL,};

struct zte_vs_element {
	struct zte_vs_element *next;
	char *data;
};

static struct zte_vs_element vs_elements[ZTE_VS_FILE_NUM];

unsigned long  BKDHash(const char *str)
{
	unsigned long hash = 0;
	while(*str)
		hash = hash * 131 + *str++;
	return hash;
}

void zte_vs_filename_hash_table_init()
{
	int i;
	for(i = 0; i < ZTE_VS_FILE_NUM; i++) {
		int index = BKDHash(zte_vs_filename_array[i]) % ZTE_VS_FILENAME_HASH_TABLE_SIZE;
		vs_elements[i].next = NULL;
		vs_elements[i].data = zte_vs_filename_array[i];
		if(zte_vs_filename_hash_table[index] == NULL) {
			zte_vs_filename_hash_table[index] = &vs_elements[i];
			assert(zte_vs_filename_hash_table[index] != NULL);
		} else {
			struct zte_vs_element *p;
			p = zte_vs_filename_hash_table[index];
			while(p->next)
				p = p->next;
			p->next = &vs_elements[i];
		}
	}
}

char *zte_search_vs_filename(const char *name)
{
	int index = BKDHash(name) % ZTE_VS_FILENAME_HASH_TABLE_SIZE;
	if(zte_vs_filename_hash_table[index] == NULL)
		return NULL;
	struct zte_vs_element *p = zte_vs_filename_hash_table[index];
	struct zte_vs_element *pre = &zte_vs_filename_hash_table[index];
	while(p) {
		if(strcmp(name, p->data) == 0) {
            if(p == pre)
                pre = NULL;
            else
                pre->next = p->next;
            return p->data;
		}
        pre = p;
		p = p->next;
	}
	return NULL;
}

void set_rand_vs_filename()
{
	int i;
	char validchars[] = "abcdefghijklmnopqrstuvwxyz0123456789_.ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int len = strlen(validchars);
	for(i = 0; i < ZTE_VS_FILE_NUM; i++) {
		int j;
		int rlen = rand() % (ZTE_VS_FILENAME_MAX_LEN -1)+ 1;
		if (rlen < 20)
			rlen = 20;
		for(j = 0; j < rlen; j++) {
			zte_vs_filename_array[i][j] = validchars[rand()%len];
		}
		zte_vs_filename_array[i][j] = '\0';
	}
}

int main(int argc, char *argv[])
{
	int i;
	srand(time(NULL));
	set_rand_vs_filename();
	zte_vs_filename_hash_table_init();
	printf("zte_vs_filename_hash_table_init end\n");
	for(i =0; i < ZTE_VS_FILE_NUM; i++) {
		char *p = zte_search_vs_filename(zte_vs_filename_array[i]);
        assert(p == zte_vs_filename_array[i]);
	}

	int j = 0;
	for(i = 0; i < ZTE_VS_FILENAME_HASH_TABLE_SIZE; i++) {
		if(zte_vs_filename_hash_table[i] != NULL)
			j++;
	}
	printf("%d\n", j);
	return 0;
}
