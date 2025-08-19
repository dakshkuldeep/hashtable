#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define TABLE_SIZE 7
#define LARGE_PRIME 173

static int hash(const char *str) {
    unsigned long hash = 0;
    while(*str) {
        hash = (hash * LARGE_PRIME + *str) % TABLE_SIZE;
        str++;
    }
    return (int)hash;
}

hashtable *create_ht() {
    hashtable *ht = malloc(sizeof(hashtable));
    ht->items = malloc(sizeof(item*) * TABLE_SIZE);
    for(int i = 0; i < TABLE_SIZE; i++) {
        ht->items[i] = NULL;
    }
    return ht;
}

static item *create_item(const char *key, const char *value) {
    item *i = malloc(sizeof(item));
    i->key = strdup(key);
    i->value = strdup(value);
    i->next = NULL;
    return i;
}

void insert_ht(hashtable *ht, const char *key, const char *value) {
	if(ht == NULL) return;
	
    const int index = hash(key);
    item *entry = ht->items[index];
    if(entry == NULL) {
        ht->items[index] = create_item(key, value);
        return;
    }

    item *prev = NULL;
    while(entry != NULL) {
        if(strcmp(key, entry->key) == 0) {
            free(entry->value);
            entry->value = strdup(value);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
    prev->next = create_item(key, value);
}

char *search_ht(hashtable *ht, const char *key) {
	if(ht == NULL) return "!";
	
    const int index = hash(key);
    item *entry = ht->items[index];

    while(entry != NULL) {
        if(strcmp(key, entry->key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return "!";
}

void dump_ht(hashtable *ht) {
	if(ht == NULL) return;
    for(int i = 0; i < TABLE_SIZE; i++) {
        printf("[%d] ", i);
        item *entry = ht->items[i];
        while(entry != NULL) {
            printf("[%s:%s] -> ", entry->key, entry->value);
            entry = entry->next;
        }
        printf("\n");
    }
    printf("==========END==========\n");
}

static void delete_item(item *i) {
	free(i->key);
	free(i->value);
	free(i);
}

void remove_ht(hashtable *ht, const char *key) {
	if(key == NULL) return;
	
    const int index = hash(key);
    item *entry = ht->items[index];
    item *prev = NULL;

    while(entry != NULL) {
        if(strcmp(key, entry->key) == 0) {
     		if(prev == NULL) {
     			ht->items[index] = entry->next; 
     		} else {
     			prev->next = entry->next;
     		}
     		delete_item(entry);
     		return;
        }
        prev = entry;
        entry = entry->next;
    }
}

static void delete_item_list(item *head_item) {
	item *cur = head_item;
	while(cur != NULL) {
		item *next = cur->next;
		delete_item(cur);
		cur = next; 
	}
}

hashtable *delete_ht(hashtable *ht) {
	if(ht == NULL) return NULL;
	for(int i = 0; i < TABLE_SIZE; i++) {
		if(ht->items[i] != NULL) {
			delete_item_list(ht->items[i]);
		}	
	}
	
	free(ht->items);
	free(ht);
	return NULL;
}