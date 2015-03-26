/*************** YOU SHOULD NOT MODIFY ANYTHING IN THIS FILE ***************/

/*
Original author: Lawrence Angrave, UIUC
Adapted by Janet Davis, Grinnell College, May 2010
*/

#define _GNU_SOURCE
#include <stdio.h>
#undef _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "testrunner.h"
#include "c_review_tests.h"

#define quit_if(cond) do {if (cond) exit(EXIT_FAILURE);} while(0)

/* test of -h switch behavior (B3) */
int test_help_switch(int argc, char **argv)
{
    char *args[] = {"./main", "-h", NULL};
    FILE *out, *err, *tmp;
    char buffer[100];

    freopen("/dev/null", "r", stdin);
    freopen("c_review.out", "w", stdout);
    freopen("c_review.err", "w", stderr);
    quit_if(main(2, args) != EXIT_FAILURE);
    fclose(stdout);
    fclose(stderr);

    out = fopen("c_review.out", "r"); 
    err = fopen("c_review.err", "r"); 
    if (fgets(buffer, 100, out) != NULL && !strncmp(buffer, "usage:", 6)) {
	tmp = out;
    }
    else {
 	quit_if(fgets(buffer, 100, err) == NULL);
    	quit_if(strncmp(buffer, "usage:", 6));
	tmp = err;
    }
    if (fgets(buffer, 100, tmp) != NULL) {
    	quit_if(!strcmp(buffer, "./main: Invalid option -h. Use -h for help.\n"));
    }
    fclose(out);
    fclose(err);
    return EXIT_SUCCESS;
}

/* test of basic functionality (B4, B5) */
int test_basic_functionality(int argc, char **argv)
{
    char *args[] = {"./main", "cat", "dog", "nap", NULL};
    char *result[] = {"Looking for 3 words\n",
		      "Result:\n",
		      "cat:1\n",
		      "dog:0\n",
		      "nap:0\n"};
    FILE *out;
    int i;
    char buffer[100];

    out = fopen("c_review.in", "w");
    fprintf(out, "cat\n");
    fprintf(out, "doggerel\n");
    fclose(out);
    freopen("c_review.in", "r", stdin);
    freopen("c_review.out", "w", stdout);
    quit_if(main(4, args) != EXIT_SUCCESS);
    fclose(stdin);
    fclose(stdout);

    out = fopen("c_review.out", "r"); 
    for (i = 0; i < 5; i++) { 
 	quit_if(fgets(buffer, 100, out) == NULL);
    	quit_if(strcmp(buffer, result[i]));
    }
    fclose(out);
    return EXIT_SUCCESS;
}

/* test of stderr output support (C1) */
int test_stderr_output(int argc, char **argv)
{
    char *args[] = {"./main", "-wrong", NULL};
    char *result[] = {"./main: Invalid option -wrong. Use -h for help.\n",
		      "./main: Please supply at least one word. Use -h for help.\n"};
    FILE *err;
    int i;
    char buffer[100];

    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("c_review.err", "w", stderr);
    quit_if(main(2, args) != EXIT_FAILURE);
    fclose(stderr);

    err = fopen("c_review.err", "r"); 
    for (i = 0; i < 2; i++) { 
 	quit_if(fgets(buffer, 100, err) == NULL);
    	quit_if(strcmp(buffer, result[i]));
    }
    fclose(err);
    return EXIT_SUCCESS;
}

/* test of -fFILENAME switch behavior (C2) */
int test_file_output(int argc, char **argv)
{
    char *args[] = {"./main", "-fc_review.out", "cat", "dog", "nap", NULL};
    char *result[] = {"Looking for 3 words\n",
		      "Result:\n",
		      "cat:1\n",
		      "dog:0\n",
		      "nap:0\n"};
    FILE *out;
    int i;
    char buffer[100];

    out = fopen("c_review.in", "w");
    fprintf(out, "cat\n");
    fclose(out);
    freopen("/dev/null", "w", stdout);
    freopen("c_review.in", "r", stdin);
    quit_if(main(5, args) != EXIT_SUCCESS);
    fcloseall();

    quit_if((out = fopen("c_review.out", "r")) == NULL); 
    for (i = 0; i < 5; i++) { 
 	quit_if(fgets(buffer, 100, out) == NULL);
    	quit_if(strcmp(buffer, result[i]));
    }
    fclose(out);
    return EXIT_SUCCESS;
}

/* test of supporting an arbitrary number of words (C3) */
int test_malloc(int argc, char **argv)
{
    char *args[] = {"./main", "cat", "dog", "nap", "c", "a", "t", NULL};
    char *result[] = {"Looking for 6 words\n",
		      "Result:\n",
		      "cat:1\n",
		      "dog:0\n",
		      "nap:0\n",
		      "c:0\n", "a:0\n", "t:0\n"};
    FILE *out;
    int i;
    char buffer[100];
    quit_if(system("grep malloc main.c > /dev/null"));
    out = fopen("c_review.in", "w");
    fprintf(out, "cat\n");
    fclose(out);
    freopen("c_review.in", "r", stdin);
    freopen("c_review.out", "w", stdout);
    quit_if(main(7, args) != EXIT_SUCCESS);
    fclose(stdin);
    fclose(stdout);
    out = fopen("c_review.out", "r"); 
    for (i = 0; i < 8; i++) { 
 	quit_if(fgets(buffer, 100, out) == NULL);
    	quit_if(strcmp(buffer, result[i]));
    }
    fclose(out);
    return EXIT_SUCCESS;
}

/* test of fgets usage (C4) */
int test_fgets(int argc, char **argv)
{
    quit_if(system("grep fgets main.c > /dev/null"));
    return EXIT_SUCCESS;
}

/* test of multiple words per line support (C5) */
int test_strtok(int argc, char **argv)
{
    char *args[] = {"./main", "cow", "chicken", NULL};
    char *result[] = {"Looking for 2 words\n",
		      "Result:\n",
		      "cow:3\n",
		      "chicken:1\n"};
    FILE *out;
    int i;
    char buffer[100];
    out = fopen("c_review.in", "w");
    fprintf(out, "cow cow rat chicken\n");
    fprintf(out, "How now brown cow?\n");
    fclose(out);
    freopen("c_review.in", "r", stdin);
    freopen("c_review.out", "w", stdout);
    quit_if(main(3, args) != EXIT_SUCCESS);
    fclose(stdin);
    fclose(stdout);
    out = fopen("c_review.out", "r"); 
    for (i = 0; i < 4; i++) { 
 	quit_if(fgets(buffer, 100, out) == NULL);
    	quit_if(strcmp(buffer, result[i]));
    }
    fclose(out);
    return EXIT_SUCCESS;
}

void delete_temp_files()
{
	unlink("c_review.in");
	unlink("c_review.out");
	unlink("c_review.err");
}

/*
 * Main entry point for SMP0 test harness
 */
int run_c_review_tests(int argc, char **argv)
{
    /* Tests can be invoked by matching their name or their suite name 
       or 'all'*/
    testentry_t tests[] = {
			{"help_switch", "suite1", test_help_switch},
			{"basic_functionality", "suite1",
test_basic_functionality},
			{"stderr_output", "suite1", test_stderr_output},
			{"file_output", "suite1", test_file_output},
			{"malloc", "suite1", test_malloc},
			{"fgets", "suite1", test_fgets},
			{"strtok", "suite1", test_strtok}};
    atexit(delete_temp_files);
    return run_testrunner(argc, argv, tests, sizeof(tests) / sizeof (testentry_t));
}
