#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "oaht.h"

#define HTP1 173
#define HTP2 157
#define INIT_SIZE 13

static item ITEM_TOMBSTONE = {NULL, NULL};

static int is_prime(int n) {
    if(n < 2) return 0;
    if(n % 2 == 0 && n != 2) return 0;
    int limit = (int)sqrt(n);
    for(int i = 3; i <= limit; i += 2) {
        if(n % i == 0) return 0;
    }
    return 1;
}

static int next_prime(int n) {
    int np = n + 1;
    while(!is_prime(np)) np++;
    return np;
}

static int hash(const char *str, const int size, const int lp) {
    unsigned long hash = 0;
    while(*str) {
        hash = (hash * lp + *str) % size;
        str++;
    }
    return (int)hash;
}

static item *create_item(const char *key, const char *value) {
    item *i = malloc(sizeof(item));
    i->key = strdup(key);
    i->value = strdup(value);
    return i;
}

static hashtable *table_broker(const int size) {
    hashtable *ht = malloc(sizeof(hashtable));
    ht->base_size = size;
    ht->size = next_prime(size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(item*));
    return ht;
}

hashtable *create_ht() {
    return table_broker(INIT_SIZE);
}

static void resize_ht(hashtable *ht, int size) {
    if(size < INIT_SIZE) {
        return;
    }
    hashtable *new_ht = table_broker(size);
    for(int i = 0; i < ht->size; i++) {
        item *entry = ht->items[i];
        if(entry != NULL && entry != &ITEM_TOMBSTONE) {
            insert_ht(new_ht, entry->key, entry->value);
        }
    }
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int temp = ht->size;
    ht->size = new_ht->size;
    new_ht->size = temp;

    item **temp_entries = ht->items;
    ht->items = new_ht->items;
    new_ht->items = temp_entries;
    delete_ht(new_ht);
}

static void resize_down(hashtable *ht) {
    const int size = ht->base_size / 2;
    resize_ht(ht, size);
}

static void resize_up(hashtable *ht) {
    const int size = ht->base_size * 2;
    resize_ht(ht, size);
}

static void delete_item(item *i) {
	free(i->key);
	free(i->value);
	free(i);
}

void delete_ht(hashtable *ht) {
	for(int i = 0; i < ht->size; i++) {
		if(ht->items[i] != NULL && ht->items[i] != &ITEM_TOMBSTONE) {
			delete_item(ht->items[i]);
		}	
	}
	free(ht->items);
	free(ht);
}

static int get_hash(const char *str, const int size, const int attempt) {
    const int hash1 = hash(str, size, HTP1);
    int hash2 = hash(str, size, HTP2);
    if(hash2 % size == 0) {
        hash2 += 1;
    }
    return (hash1 + (attempt * hash2)) % size;
}

void insert_ht(hashtable *ht, const char *key, const char *value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        resize_up(ht);
    }
    item *entry = create_item(key, value);
    int hash = get_hash(entry->key, ht->size, 0);
    item *cur = ht->items[hash];
    int attempt = 1;
    while(cur != NULL) {
        if(cur != &ITEM_TOMBSTONE) {
            if(strcmp(key, cur->key) == 0) {
                delete_item(cur);
                ht->items[hash] = entry;
                return;
            }
        }
        hash = get_hash(entry->key, ht->size, attempt);
        cur = ht->items[hash];
        attempt++;
    }
    ht->items[hash] = entry;
    ht->count++;
}

char *search_ht(hashtable *ht, const char *key) {
    int hash = get_hash(key, ht->size, 0);
    item *entry = ht->items[hash];
    int attempt = 1;
    while(entry != NULL) {
        if(entry != &ITEM_TOMBSTONE) {
            if(strcmp(key, entry->key) == 0) {
                return entry->value;
            }
        }
        hash = get_hash(key, ht->size, attempt);
        entry = ht->items[hash];
        attempt++;
    }
    return NULL;
}

void dump_ht(hashtable *ht) {
    printf("========== HASH-TABLE (%d/%d) ==========\n", ht->count, ht->size);
    for(int i = 0; i < ht->size; i++) {
        if(ht->items[i] != NULL && ht->items[i] != &ITEM_TOMBSTONE) {
            printf("[%d] %s:%s\n", i, ht->items[i]->key, ht->items[i]->value);
        }
    }
    printf("========== END ==========\n\n");
}

void rmfrom_ht(hashtable *ht, const char *key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        resize_down(ht);
    }
    int hash = get_hash(key, ht->size, 0);
    item *entry = ht->items[hash];
    int attempt = 1;
    while(entry != NULL) {
        if(entry != &ITEM_TOMBSTONE) {
            if(strcmp(key, entry->key) == 0) {
                delete_item(entry);
                ht->items[hash] = &ITEM_TOMBSTONE;
                ht->count--;
                return;
            }
        }
        hash = get_hash(key, ht->size, attempt);
        entry = ht->items[hash];
        attempt++;
    }
}
