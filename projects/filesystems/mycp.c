/*John Brady & Albert Owusu-Asare
Boxes 3119 & 4497
December 8, 2014
CSC213, Weinman
Lab 10: Filesystems
mycp.c - error checking and reporting version of cp
*/
/* File copy program.  */
/* From Andrew S. Tanenbaum, _Modern_Operating_Systems 3/e, p. 266.
 * Transcribed by Janet Davis, October 8, 2010.
 *Modified by Albert & John for Jerod Weinman's Lab 10
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

#define BUF_SIZE 4096           /* use a buffer size of 4096 bytes */
#define OUTPUT_MODE 0700        /* protection bits for output file */

int main(int argc, char *argv[])
{
    int in_fd, out_fd, rd_count, wt_count;
    char buffer[BUF_SIZE];

    if (argc != 3)
    {
        fprintf(stderr, "Syntax: mycp <source> <destination>\n");
        exit(EXIT_FAILURE);        /* syntax error if argc is not 3 */
    }

    /* Open the input file and create the output file */
    in_fd = open(argv[1], O_RDONLY);             /* open the source file */

    if (in_fd < 0)
    {
        perror("Open Error");
        exit(EXIT_FAILURE);         /* if it cannot be opened, exit */
    }
    //Copy Permissions to struct
    struct stat orig_stats;
    fstat(in_fd, &orig_stats);

    out_fd = creat(argv[2], orig_stats.st_mode ); //Create  destination file
    //based on testing, EISDIR should be 21
    if (errno == EISDIR)
    {
        char *orig_name = basename(argv[1]); //gives us source name
        char *dest_path = malloc(sizeof(argv[1]) +
                                 sizeof(argv[2] + 3)); //declares destination
        strcpy(dest_path, argv[2]); //add directory name
        strcat(dest_path, "/"); //add slash into directory
        strcat(dest_path, orig_name); //add output file name
        out_fd = creat(dest_path, orig_stats.st_mode); //retry copy


    }
    if (out_fd < 0)
    {
        perror("Create Error");
        exit(EXIT_FAILURE);        /* if it cannot be created, exit */
    }
    /* Copy loop */
    while (1)
    {
        rd_count = read(in_fd, buffer, BUF_SIZE); /* read a block of data */
        if (rd_count <= 0)
        {
            //if rd_count other than EOF, err
            if (rd_count < 0) perror("Read Error");
            break;      /* if end of file or error, exit loop */
        }
        wt_count = write(out_fd, buffer, rd_count);         /* write data */
        if (wt_count <= 0)
        {
            perror("Write Error");
            exit(EXIT_FAILURE);           /* wt_count <= 0 is an error */
        }
    }




    /* Close the files */
    close(in_fd);
    close(out_fd);
    if (rd_count == 0)               /* no error on last read */
        exit(EXIT_SUCCESS);
    else                                /* error on last read */
    {
        perror("Close Error");
        exit(EXIT_FAILURE);
    }
}
