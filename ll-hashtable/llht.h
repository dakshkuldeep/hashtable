#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct item {
    char *key;
    char *value;
    struct item *next;
} item;

typedef struct {
    int size;
    item **items;
} hashtable;

hashtable *create_ht(const int size);
hashtable *delete_ht(hashtable *ht);

void insert_ht(hashtable *ht, const char *key, const char *value);
char *search_ht(hashtable *ht, const char *key);
void dump_ht(hashtable *ht);
void rmfrom_ht(hashtable *ht, const char *key);

#endif
