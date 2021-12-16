#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>

#include "newsyscall2.h"

#define __NR_syscall 17


#define error(miss) { fprintf(stderr, "ERROR (linia %d): %s\n", __LINE__, miss); exit(1); }
#define TESTING(msg) fprintf(stderr, "\n\n%s...", msg);
#define TESTINGOK() fprintf(stderr, "OK\n");

#define MAX_FILLS 32

int
main ()
{
  int n, fills, i;
  pid_t pids[MAX_FILLS + 1], pid;

  TESTING ("Trivial cases");
  n = syscall (__NR_syscall, getpid(), NUM_CHILDREN);
  if (n != 0)
    error ("Must return 0, no child process yet");

  n = syscall (__NR_syscall, getpid(), 3);
  if ((n != -1) || (errno != EINVAL))
    error ("Must return -1, errno=EINVAL");

  switch (pid = fork())
      {
      case -1:
	error ("Child processes creation");
      case 0:
        exit(0);
   }
  wait(NULL);
  n = syscall (__NR_syscall, pid, NUM_CHILDREN);
  if ((n != -1) || (errno != ESRCH))
    error ("Must return -1, errno=ESRCH");
   
  TESTINGOK ();

  TESTING ("Checking number of child processes");
  srand (getpid ());
  fills = 1 + rand () % MAX_FILLS;
  printf ("\nNumber = %d\n", fills);
  for (i = 0; i < fills; i++)
    switch (pids[i] = fork ())
      {
      case -1:
	error ("Child processes creation");
      case 0:
	sleep (100);
	exit (0);
      }

  n = syscall (__NR_syscall, getpid(), NUM_CHILDREN);
  if (n != fills)
    {
      char s[80];

      sprintf (s, "Must return %d, but returns %d\n", fills, n);
      error (s);
    }

  for (i = 0; i < fills; i++)
    {
      kill (pids[i], 9);
      wait (NULL);
    }
  n = syscall (__NR_syscall, getpid(), NUM_CHILDREN);
  if (n != 0)
    error ("Must return 0");

  TESTINGOK ();

  TESTING ("Checking number of sibling processes");
  fills = 1 + rand () % MAX_FILLS;
  printf ("\nNumber= %d\n", fills);
  for (i = 0; i < fills; i++)
    switch (pids[i] = fork ())
      {
      case -1:
	error ("Process creation");
      case 0:
	sleep (2);		/* Enough time to allow the creation of the remaining siblings */
	if (syscall (__NR_syscall, getpid(), NUM_SIBLINGS) != fills)
	  error ("Incorrect number of siblings ");
/*				printf("%d\n", syscall(__NR_syscall, NUM_SIBLINGS));*/
	exit (0);
      }
  TESTINGOK ();

  TESTING ("Checking other processes");
  sleep (5);			/* Enough time to allow all child processes to check their number of siblings */
  for (i = 0; i < fills; i++)
    {
      if (syscall (__NR_syscall, pids[i], NUM_SIBLINGS) != (fills-i))
	  error ("Incorrect number of siblings ");
      if (syscall (__NR_syscall, pids[i], NUM_CHILDREN) != 0)
	  error ("Incorrect number of children");

      kill (pids[i], 9);
      wait (NULL);

      n = syscall (__NR_syscall, pids[i], NUM_SIBLINGS);
      if ((n!=-1) || (errno != ESRCH))
	  error ("Incorrect result");

    }
  TESTINGOK ();

  fprintf(stderr, "All tests seem to be OK\n");
}
