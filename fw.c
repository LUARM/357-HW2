#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include "hash_quad.h"
#include "support.h"
#include <string.h>
#include <stdbool.h>

#define INITIAL_WORD_BUFFER_SIZE 50


void print_usage(void) 
{
     fprintf(stdout, "usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
}

int get_words(HashTable *ht, FILE *fp) 
{
     
     int   buffer_size = INITIAL_WORD_BUFFER_SIZE;
     char  *buffer = safe_malloc(buffer_size, __LINE__);
     int   c, words_read = 0;
     int bp = 0;

     if (fp)
          c = fgetc(fp);
     else
          c = getchar();
     

     while (c != EOF) {
          if (isalpha(c)) {
               buffer[bp] = tolower(c);
               bp += 1;

               if (bp > buffer_size - 1) 
               {
                    buffer_size *= 2;
                    buffer = safe_realloc(buffer, buffer_size, __LINE__);
               }
          }
          else {
               if (bp) {
                    buffer[bp] = 0;
                    words_read += 1;

                    add_update_item(ht, &buffer[0]);

                    bp = 0;
               }
          }
  
          if (fp)
               c = fgetc(fp);
          else
               c = getchar();
     }
     free(buffer);

     return words_read;
}

int read_words_from_files(HashTable *ht, int argc, char **argv, int files_list_start) {
    
     FILE *fp;
     int  i, words_read = 0, num_files = 0;


     for (i = files_list_start; i < argc; i++) {
          fp = fopen(argv[i], "r");
          if (fp == NULL) {
               fprintf(stderr, "%s: %s\n", argv[i], strerror(errno));
               continue;
          }

          words_read = get_words(ht, fp);

          fclose(fp);
          num_files += 1;
     }
     return words_read;
}

int read_words_from_stdin(HashTable *ht) 
{
    
     int words_read = 0;


     words_read = get_words(ht, NULL);

     return words_read;
}

int main(int argc, char **argv) 
{
     HashTable      *ht;
     int            num_top_words = 10;
     int            argv_spot = 1;
     if (argc < 2) 
     {
          argv_spot = 0;
     }

 
     if (argv[argv_spot][0] == '-') 
     {
          if (!STREQ("-n", argv[argv_spot])) 
          {
               
               fprintf(stdout, "%s: illegal option -- %s\n", argv[0], &argv[argv_spot][1]);
               print_usage();
               return EXIT_FAILURE;
          }
          else 
          {
              
               if (argc < 3) 
               {
                    print_usage();
                    return EXIT_FAILURE;
               }
               if (STREQ(argv[argv_spot + 1], "0")) 
               {
                    num_top_words = 0;
               }
               else 
               {
                    num_top_words = atoi(argv[argv_spot + 1]);
                    if (num_top_words < 0) {
                         fprintf(stderr, "%s: It is not possible to count fewer than zero words\n", argv[0]);
                         return EXIT_FAILURE;
                    }
                    if (!num_top_words) 
                    {
                         fprintf(stderr, "%s: not a well-formed number\n", argv[argv_spot + 1]);
                         return EXIT_FAILURE;
                    }
               }
               argv_spot += 2;
          }
     }

     ht = create_hash_table(1000, 0.5, 15);

     if (argv_spot == 0 || argv_spot >= argc) 
     {

          read_words_from_stdin(ht);
     }
     else 
     {
  
          read_words_from_files(ht, argc, argv, argv_spot);
     }

     print_top_words(ht, num_top_words);

     free_hash_table(ht);

     return EXIT_SUCCESS;
}
