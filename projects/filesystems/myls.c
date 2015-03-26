/*John Brady & Albert Owusu-Asare
Boxes 3119 & 4497
December 8, 2014
CSC213, Weinman
Lab 10: Filesystems
myls.c: prints information about the current or listed directory
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
#include <dirent.h>


int main(int argc, char *argv[]);   // ANSI prototype */


int main(int argc, char *argv[])
{


    if (argc > 2)
    {
        fprintf(stderr, "Syntax: myls <directory>/<>\n");
        exit(EXIT_FAILURE);    // syntax error if argc is more than 2 */
    }

    //now directory is the directory we want to print. pass to opendir

    DIR *directory;
    struct dirent *ent;
    //if we have an argument, open the listed directory
    if (argc == 2)
        directory = opendir(argv[1]);
    else
        //otherwise open the current directory
        directory = opendir(".");
    //if nothing in directory
    if (directory == NULL)
    {
        perror("Open Error");
        exit(EXIT_FAILURE);         // if it cannot be opened, exit
    }
    else
    {
        //iterate through directory
        while ((ent = readdir(directory)) != NULL)
        {
            //if the file is not hidden (marked by a leading .)
            if ('.' != ent->d_name[0])
            {
                struct stat buf;
                int error;
                //create path; argv's length plus max d_name size

                char *filepath = malloc(sizeof(argv[1]) + 255 * sizeof(char));

                if (argc == 2)
                {
                    strcpy(filepath, argv[1]);
                }
                else
                {
                    strcpy(filepath, ".");
                }
                strcat(filepath, "/");
                strcat(filepath, ent->d_name);
                //store statistics in buf
                error = stat(filepath, &buf);
                if (error < 0)
                {
                    perror("Stat");
                    exit(EXIT_FAILURE); // if it cannot be opened, exit */
                }

                printf("%s\t%d\n", ent->d_name, buf.st_size);
            }

        }//while
        //close directory
        closedir(directory);

    }//else
    exit(EXIT_SUCCESS);

}