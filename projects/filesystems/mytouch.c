/*John Brady & Albert Owusu-Asare
Boxes 3119 & 4497
December 8, 2014
CSC213, Weinman
Lab 10: Filesystems
mytouch.c: updates date modified time to current system time
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


#define OUTPUT_MODE 0644

int main(int argc, char *argv[]);   /* ANSI prototype */


int main(int argc, char *argv[])
{
    int out_fd;

    if (argc != 2)
    {
        fprintf(stderr, "Syntax: mytouch <filename>\n");
        exit(EXIT_FAILURE);        /* syntax error if argc is not 2 */
    }
    int time_check;
    time_check = utime(argv[1], NULL);
    if (time_check < 0)
    {
        if (errno == ENOENT)
        {
            //create a file with system default permissions
            out_fd = creat(argv[1], OUTPUT_MODE ); //Create destination file
        }
        else
        {
            perror("Utime Error");
            exit(EXIT_FAILURE);
        }       /* if it cannot be created, exit */
    }
    //if utime returns that a file does not exist

    if (out_fd < 0)
    {
        perror("Create Error");
        exit(EXIT_FAILURE);        /* if it cannot be created, exit */
    }
    /* Close the file */
    close(out_fd);

}