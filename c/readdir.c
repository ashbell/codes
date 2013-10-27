#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAXLEN  1024

char fullpath[MAXLEN];
char *save_list[MAXLEN];

void list_dir(const char *start_path, const char *index)
{
    DIR *dp;
    struct dirent *dirp;
    struct stat   statbuf;
    char   *ptr, *ap, *bp;
    int    rp;

    strncpy(fullpath, start_path, strlen(start_path));
    printf("first fullpath: %s\n", fullpath);
    if (lstat(start_path, &statbuf) < 0)
    {
        printf("error stat: %s\n", start_path);
        exit(0);
    }
    if (S_ISDIR(statbuf.st_mode) == 0)
    {
        printf("%s is not a dir.\n", start_path);
        exit(0);
    }
    if (!(dp = opendir(start_path)))
    {
        printf("error opendir: %s\n", start_path);
        exit(0); 
    }
    ap = ptr = fullpath + strlen(fullpath);
    while ((dirp = readdir(dp)) != NULL)  {
        if ( strcmp(dirp->d_name, ".") == 0 || 
             strcmp(dirp->d_name, "..") == 0 ||
             (dirp->d_name[0] == '.')
            )
            continue;
        strncpy(ptr, dirp->d_name, strlen(dirp->d_name));
        ptr = ptr + strlen(dirp->d_name);
        *ptr = 0;
        if (strcmp(dirp->d_name, index) == 0)
        {
            printf("Find at: %s\n", fullpath);

        }
        if (lstat(fullpath, &statbuf) < 0)
        {
            printf("error lstat at: %s\n", fullpath);
            exit(0);
        }
        switch (statbuf.st_mode & S_IFMT) {
            case S_IFDIR: {
                          ptr  = fullpath + strlen(fullpath);
                          rp = strlen(fullpath);
                          *ptr++ = '/';
                          *ptr = 0;
                          printf("Dir: %s\n", fullpath);
                          list_dir(fullpath, index);
                          fullpath[rp] = '\0';
                          printf("\n\n");

                          break;
            }
            case S_IFREG: 
                            printf("RegFile: %s \n", fullpath); break;
            case S_IFLNK: 
                            printf("LinkFile: %s\n", fullpath); break;
            default:      printf("Unknow filetype!\n"); break;
        }
        ptr = ap;
    }
    closedir(dp);
}

int main(int argc, char *argv[])
{
    list_dir(argv[1], argv[2]);
    return 0;



}
