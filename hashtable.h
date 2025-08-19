#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct item {
    char *key;
    char *value;
    struct item *next;
} item;

typedef struct {
    item **items;
} hashtable;

hashtable *create_ht();
hashtable *delete_ht(hashtable *ht);

void insert_ht(hashtable *ht, const char *key, const char *value);
char *search_ht(hashtable *ht, const char *key);
void dump_ht(hashtable *ht);
void remove_ht(hashtable *ht, const char *key);

#endif
