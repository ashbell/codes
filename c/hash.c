#include <stdio.h>

int main(int argc, char *argv[])
{
    /* hash table for str1 */
    int i = 0;
    char hash_table[37];
    while (argv[1][i] != '\0') 
      {
        hash_table[argv[1][i] - '0'] = argv[1][i];
        i++;
      }
    i = 0;
    while (argv[2][i] != '\0')
      {
        if (hash_table[argv[2][i] - '0'] != argv[2][i])
          {
            printf ("%s is not a subs %s\n", argv[2], argv[1]);
            break;
          }
          i++;
      }
      return 0;
}







