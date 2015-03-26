/*John Brady & Albert Owusu-Asare
Boxes 3119 & 4497
December 8, 2014
CSC213, Weinman
Lab 10: Filesystems
mymv.c: moves/renames a file
NOTE: Skeleton (with includes and ANSI prototype) copied from myrm.c
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include <utime.h>


int main(int argc, char *argv[]);   // ANSI prototype */


int main(int argc, char *argv[])
{
    //check for correct number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Syntax: mymv <oldpath> <newpath>\n");
        exit(EXIT_FAILURE);        /* syntax error if argc is not 3*/
    }

    int error;
    error = rename(argv[1], argv[2]);
    //if our source file was a directory
    if (errno == EISDIR)
    {
        //allocate for an array to store the splits
        const char slash[2] = "/";
        char *token;

        char *input = malloc(sizeof(argv[1]));
        char *output = malloc(sizeof(argv[1]));
        strcpy(input, argv[1]);

        token = strtok(input, slash);

        while (token != NULL)
        {
            strcpy(output, token);
            token = strtok(NULL, slash);

        }

        char *updated_path = malloc(sizeof(argv[1]) + sizeof(argv[2]));
        strcpy(updated_path, argv[2]);
        strcat(updated_path, "/");
        strcat(updated_path, output);

        error = rename(argv[1], updated_path);

    }

    if (error < 0)
    {
        perror("rename Error");
        exit(EXIT_FAILURE);
    }
}