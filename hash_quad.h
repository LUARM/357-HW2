#ifndef __HASH_QUAD_H__
#define __HASH_QUAD_H__

typedef struct 
{

  char *key;            
  int  count;        
} HashItem;

typedef struct 
{
  int  table_size;       
  int  length;           
  HashItem      *items;           
  float  max_load_factor;  
  int   hash_max;         
  int    resized;          
} HashTable;



HashTable * create_hash_table(int init_size, float load_factor, int hash_max);

void free_hash_table(HashTable *ht);
int horner_hash(HashTable *ht, char *key);
void sort_items(HashTable *ht);
void print_top_words(HashTable *ht, int top_words);
void print_hash_table(HashTable *ht, int print_items);

void add_update_item(HashTable *ht, char *key);
void add_item(HashTable *ht, char *key, int count);
void resize(HashTable *ht);
void compact(HashTable *ht);
int next_perfect_square(float n);
int get_hash(HashTable *ht, char *key);

int cmp_count_func(const void * a, const void * b);

#endif
