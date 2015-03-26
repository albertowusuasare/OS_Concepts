/*John Brady & Albert Owusu-Asare
Boxes 3119 & 4497
December 8, 2014
CSC213, Weinman
Lab 10: Filesystems
mycp.c - error checking and reporting version of cp
NOTE: adapted from mycp.c to use the includes, ANSI thing, and open/close
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


int main(int argc, char *argv[]);   /* ANSI prototype */


int main(int argc, char *argv[])
{
    int in_fd;

    if (argc != 2)
    {
        fprintf(stderr, "Syntax: myrm <filename>\n");
        exit(EXIT_FAILURE);        /* syntax error if argc is not 2 */
    }

    /* Open the input file  */
    in_fd = open(argv[1], O_RDONLY);             // open the source file 
    struct stat stats;
    fstat(in_fd, &stats);

    if (S_ISDIR(stats.st_mode))
    {
        fprintf(stderr, "%s refers to a directory.\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if (in_fd < 0)
    {
        perror("Open Error");
        exit(EXIT_FAILURE);         /* if it cannot be opened, exit */
    }

    /* Close the file */
    close(in_fd);

    //delete the file
    unlink(argv[1]);

}
