#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct {
    char *key;
    char *value;
} item;

typedef struct {
    int base_size;
    int size;
    int count;
    item **items;
} hashtable;

hashtable *create_ht();
void delete_ht(hashtable *ht);

void insert_ht(hashtable *ht, const char *key, const char *value);
char *search_ht(hashtable *ht, const char *key);
void dump_ht(hashtable *ht);
void rmfrom_ht(hashtable *ht, const char *key);

#endif
