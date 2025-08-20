#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

#define HTP1 173
#define HTP2 157

static item ITEM_TOMBSTONE = {NULL, NULL};

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

hashtable *create_ht() {
    hashtable *ht = malloc(sizeof(hashtable));
    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(item*));
    return ht;
}

static void delete_item(item *i) {
	free(i->key);
	free(i->value);
	free(i);
}

void delete_ht(hashtable *ht) {
	for(int i = 0; i < ht->size; i++) {
		if(ht->items[i] != NULL) {
			delete_item(ht->items[i]);
		}	
	}
	free(ht->items);
	free(ht);
}

static int get_hash(const char *str, const int size, const int attempt) {
    const int hash1 = hash(str, size, HTP1);
    const int hash2 = hash(str, size, HTP2);
    return (hash1 + (attempt * (hash2 + 1))) % size;
}

void insert_ht(hashtable *ht, const char *key, const char *value) {
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