

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <math.h>
#include "hash_quad.h"
#include "support.h"


HashTable * create_hash_table(int init_size, float load_factor, int hash_max) 
{
   
    HashTable *ht = safe_calloc(1, sizeof(HashTable), __LINE__);
    ht->table_size = init_size;
    ht->length = 0;
    ht->max_load_factor = load_factor;
    /* max char num*/
    ht->hash_max = hash_max;
    ht->items = safe_calloc(ht->table_size, sizeof(HashItem), __LINE__);
    /* pointer to*/
    return ht;
}
void print_top_words(HashTable *ht, int top_words) 
{
    /* prints top words */

    int i;
    int actual_count = top_words;   
    if (top_words)
    {
        sort_items(ht);
    }   
    if (actual_count > ht->length)
    {
        actual_count = ht->length;
    }    
    printf("The top %d words (out of %d) are:\n", top_words, ht->length);
    for (i = 0; i < actual_count; i++) 
    {
        if (ht->items[i].count)
        {
            printf("%9d %s\n", ht->items[i].count, ht->items[i].key);
        }
    }
}

void print_hash_table(HashTable *ht, int print_items) 
{

    int i;
    printf("\n");

    if (print_items) 
    {
        for (i = 0; i < ht->table_size; i++) 
        {
            if (ht->items[i].count)
                printf("key: %s - count: %d\n", ht->items[i].key, ht->items[i].count);
        }
    }
    printf("\nTable Size is: %d  -  Length is: %d\n", ht->table_size, ht->length);
    printf("The table was resized %d times\n", ht->resized);
    printf("\n");
}


void free_hash_table(HashTable *ht) 
{

    int i;
    /* free keys */
    for (i = 0; i < ht->table_size; i++) 
    {
        
        if (ht->items[i].count)
        {
            free(ht->items[i].key);
        }
    }
    /* free hash table*/
    free(ht->items);
    free(ht);
}

int horner_hash(HashTable *ht, char *key) 
{
    
    int max_len;
    int i, key_len;
    unsigned long hash_val = 0;

    key_len = strlen(key);
    if (key_len < ht->hash_max)
    {
        max_len = key_len;
    }
    else
    {
        max_len = ht->hash_max;
    }

    for (i = 0; i < max_len; i++) {
        hash_val = 31 * hash_val + key[i];
    }

    return hash_val % ht->table_size;
}

int next_perfect_square(float n) 
{
    /*finds square root*/
    while (sqrt(n) != floor(sqrt(n)))
    {
        n += 1;
    }
    return (int)n;
}

int get_hash(HashTable *ht, char *key) 
{
    /*compute hash value*/
    long org_hash, curr_hash, hash_add;
    org_hash = horner_hash(ht, key);
    
    curr_hash = org_hash;
    
    hash_add = -1;

    while (ht->items[curr_hash].count != 0 && !STREQ(ht->items[curr_hash].key, key)) 
    {
        hash_add = next_perfect_square(hash_add + 1);
        curr_hash = (org_hash + hash_add) % ht->table_size;
    }

    return curr_hash;
}

void add_update_item(HashTable *ht, char *key) 
{
    
    int hash;    
    hash = get_hash(ht, key);

    if (ht->items[hash].count) {
        ht->items[hash].count += 1;
    }
    else 
    {
        /* puts key into the hash table */
        ht->items[hash].count = 1;
        ht->items[hash].key = safe_strdup(key, __LINE__);
        ht->length += 1;

        /* looks if we need to resize based on the load factor*/
        if ((float)ht->length / (float)ht->table_size > ht->max_load_factor) 
        {
            resize(ht);
        }
    }
}
int cmp_func(const void * a, const void * b) 
{
       
    int count;
    count = ( ((HashItem*)b)->count - ((HashItem*)a)->count );
    if (!count) 
    {
        return strcmp(((HashItem*)b)->key, ((HashItem*)a)->key);
    }

    return count;
}

void sort_items(HashTable *ht) 
{
    /* sort hash table*/
    compact(ht);
    qsort(ht->items, ht->length, sizeof(HashItem), cmp_func);
}



void add_item(HashTable *ht, char *key, int count) 
{
    /* add to empty hash table so less checks are needed*/
    int hash = get_hash(ht, key);
    ht->items[hash].count = count;
    ht->items[hash].key = key;
    ht->length += 1;
}


void resize(HashTable *ht) 
{
    /*rezise hash table and reinsert keys in new table*/

    HashItem *old_table = ht->items;
    int i, old_table_size = ht->table_size;


    ht->table_size = (ht->table_size * 2) + 1;

    ht->items = safe_calloc(ht->table_size, sizeof(HashItem), __LINE__);
    ht->length = 0;

    for (i = 0; i < old_table_size; i++) 
    {
        if (old_table[i].count) {
            add_item(ht, old_table[i].key, old_table[i].count);
        }
    }

    free(old_table);
    ht->resized += 1;
}

void compact(HashTable *ht) 
{
    
    int i, curr_slot = 0, last_slot = 0;

    /* finds empty slot in hash table*/
    for (i = 0; i < ht->table_size; i++) 
    {
        
        if (!ht->items[i].count) 
        {
            curr_slot = i;
            last_slot = i+1;
            break;
        }
    }
    if (last_slot > ht->length) 
    {
        return;
    }
    for (i = last_slot; i < ht->table_size; i++) 
    {
        if (ht->items[i].count) 
        {
            ht->items[curr_slot].key = ht->items[i].key;
            ht->items[curr_slot].count = ht->items[i].count;
            curr_slot += 1;
        }
    }
    for (i = curr_slot; i < ht->table_size; i++) 
    {
        ht->items[i].count = 0;
        ht->items[i].key = NULL;
    }
}

