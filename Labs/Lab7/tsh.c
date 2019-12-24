/* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and login ID here>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>

/* Misc manifest constants */
#define MAXLINE 1024   /* max line size */
#define MAXARGS 128	/* max args on a command line */
#define MAXJOBS 16	 /* max jobs at any point in time */
#define MAXJID 1 << 16 /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1	/* running in foreground */
#define BG 2	/* running in background */
#define ST 3	/* stopped */

//Macro
#define streql(a, b) !strcmp(a, b)

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : background command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;   /* defined in libc */
char prompt[] = "tsh> "; /* command line prompt (DO NOT CHANGE) */
int verbose = 0;		 /* if true, print additional output */
int nextjid = 1;		 /* next job ID to allocate */
char sbuf[MAXLINE];		 /* for composing sprintf messages */

typedef struct
{							   /* The job struct */
	pid_t pid;				   /* job PID */
	int jid;				   /* job ID [1, 2, ...] */
	int state;				   /* UNDEF, BG, FG, or ST */
	char cmdjobsline[MAXLINE]; /* command line */
} job_t;
job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */

/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
bool builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
bool parseline(const char *cmdline, char **argv);
void sigquit_handler(int sig);

void clearjob(job_t *job);
void initjobs(job_t *jobs);
int maxjid(job_t *jobs);
int addjob(job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(job_t *jobs, pid_t pid);
pid_t fgpid(job_t *jobs);
job_t *getjobpid(job_t *jobs, pid_t pid);
job_t *getjobjid(job_t *jobs, int jid);
int pid2jid(pid_t pid);
void listjobs(job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv)
{
	char c;
	char cmdline[MAXLINE];
	int emit_prompt = 1; /* emit prompt (default) */

	/* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
	dup2(1, 2);

	/* Parse the command line */
	while ((c = getopt(argc, argv, "hvp")) != EOF)
	{
		switch (c)
		{
		case 'h': /* print help message */
			usage();
			break;
		case 'v': /* emit additional diagnostic info */
			verbose = 1;
			break;
		case 'p':			 /* don't print a prompt */
			emit_prompt = 0; /* handy for automatic testing */
			break;
		default:
			usage();
		}
	}

	/* Install the signal handlers */

	/* These are the ones you will need to implement */
	Signal(SIGINT, sigint_handler);   /* ctrl-c */
	Signal(SIGTSTP, sigtstp_handler); /* ctrl-z */
	Signal(SIGCHLD, sigchld_handler); /* Terminated or stopped child */

	/* This one provides a clean way to kill the shell */
	Signal(SIGQUIT, sigquit_handler);

	/* Initialize the job list */
	initjobs(jobs);

	/* Execute the shell's read/eval loop */
	while (1)
	{

		/* Read command line */
		if (emit_prompt)
		{
			printf("%s", prompt);
			fflush(stdout);
		}
		if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
			app_error("fgets error");
		if (feof(stdin))
		{ /* End of file (ctrl-d) */
			fflush(stdout);
			exit(0);
		}

		/* Evaluate the command line */
		eval(cmdline);
		fflush(stdout);
		fflush(stdout);
	}

	exit(0); /* control never reaches here */
}

/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, background or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline)
{
	/* $begin handout */
	char *argv[MAXARGS]; /* argv for execve() */
	bool background;	 /* should the job run in background or fg? */
	pid_t pid;			 /* process id */
	sigset_t mask;		 /* signal mask */

	/* Parse command line */
	background = parseline(cmdline, argv);
	if (argv[0] == NULL)
		return; /* ignore empty lines */

	if (!builtin_cmd(argv))
	{

		/* 
	 * This is a little tricky. Block SIGCHLD, SIGINT, and SIGTSTP
	 * signals until we can add the job to the job list. This
	 * eliminates some nasty races between adding a job to the job
	 * list and the arrival of SIGCHLD, SIGINT, and SIGTSTP signals.  
	 */

		if (sigemptyset(&mask) < 0)
			unix_error("sigemptyset error");
		if (sigaddset(&mask, SIGCHLD))
			unix_error("sigaddset error");
		if (sigaddset(&mask, SIGINT))
			unix_error("sigaddset error");
		if (sigaddset(&mask, SIGTSTP))
			unix_error("sigaddset error");
		if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
			unix_error("sigprocmask error");

		/* Create a child process */
		if ((pid = fork()) < 0)
			unix_error("fork error");

		/* 
	 * Child  process 
	 */

		if (pid == 0)
		{
			/* Child unblocks signals */
			if (sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0)
			{
				unix_error("sigprocmask error");
			}

			/* Each new job must get a new process group ID 
	       so that the kernel doesn't send ctrl-c and ctrl-z
	       signals to all of the shell's jobs */
			if (setpgid(0, 0) < 0)
				unix_error("setpgid error");

			/* Now load and run the program in the new job */
			if (execve(argv[0], argv, environ) < 0)
			{
				printf("%s: Command not found\n", argv[0]);
				exit(0);
			}
		}

		/* 
	 * Parent process
	 */

		/* Parent adds the job, and then unblocks signals so that
	   the signals handlers can run again */
		addjob(jobs, pid, (background == 1 ? BG : FG), cmdline);
		if (sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0)
		{
			unix_error("sigprocmask error");
		}

		if (!background)
			waitfg(pid);
		else
			printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
	}
	/* $end handout */
	return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
bool parseline(const char *cmdline, char **argv)
{
	static char array[MAXLINE]; /* holds local copy of command line */
	char *buf = array;			/* ptr that traverses command line */
	char *delim;				/* points to first space delimiter */
	int argc;					/* number of args */
	bool background;			/* background job? */

	strcpy(buf, cmdline);
	buf[strlen(buf) - 1] = ' ';   /* replace trailing '\n' with space */
	while (*buf && (*buf == ' ')) /* ignore leading spaces */
		buf++;

	/* Build the argv list */
	argc = 0;
	if (*buf == '\'')
	{
		buf++;
		delim = strchr(buf, '\'');
	}
	else
	{
		delim = strchr(buf, ' ');
	}

	while (delim)
	{
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' ')) /* ignore spaces */
			buf++;

		if (*buf == '\'')
		{
			buf++;
			delim = strchr(buf, '\'');
		}
		else
		{
			delim = strchr(buf, ' ');
		}
	}
	argv[argc] = NULL;

	if (argc == 0) /* ignore blank line */
		return 1;

	/* should the job run in the background? */
	if ((background = (*argv[argc - 1] == '&')) != 0)
	{
		argv[--argc] = NULL;
	}
	return background;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */

bool builtin_cmd(char **argv)
{
	const char *command = *argv;

	//builtin_cmd:quit,jobs,bg,fg

	if (streql(command, "quit"))
	{
		_exit(0);
	}
	else if (streql(command, "jobs"))
	{
		sigset_t full_mask, mask;
		sigfillset(&full_mask);
		sigprocmask(SIG_BLOCK, &full_mask, &mask);
		listjobs(jobs);
		sigprocmask(SIG_SETMASK, &mask, NULL);
		return true;
	}
	else if (streql(command, "bg") || streql(command, "fg"))
	{
		do_bgfg(argv);
		return true;
	}
	else
	{
		return false;
	}
}

/* 
 * do_bgfg - Execute the builtin background and fg commands
 */
void do_bgfg(char **argv)
{
	/* $begin handout */

	/* Ignore command if no argument */
	if (argv[1] == NULL)
	{
		printf("%s command requires PID or %%jobid argument\n", argv[0]);
		return;
	}
	job_t *jobp = NULL;

	/* Parse the required PID or %JID arg */
	if (isdigit(argv[1][0]))
	{
		pid_t pid = atoi(argv[1]);
		if (!(jobp = getjobpid(jobs, pid)))
		{
			printf("(%d): No such process\n", pid);
			return;
		}
	}
	else if (argv[1][0] == '%')
	{
		int jid = atoi(&argv[1][1]);
		if (!(jobp = getjobjid(jobs, jid)))
		{
			printf("%s: No such job\n", argv[1]);
			return;
		}
	}
	else
	{
		printf("%s: argument must be a PID or %%jobid\n", argv[0]);
		return;
	}

	/* background command */
	if (streql(argv[0], "bg"))
	{
		if (kill(-(jobp->pid), SIGCONT) < 0)
			unix_error("kill (background) error");
		jobp->state = BG;
		printf("[%d] (%d) %s", jobp->jid, jobp->pid, jobp->cmdjobsline);
	}

	/* fg command */
	else if (streql(argv[0], "fg"))
	{
		if (kill(-(jobp->pid), SIGCONT) < 0)
			unix_error("kill (fg) error");
		jobp->state = FG;
		waitfg(jobp->pid);
	}
	else
	{
		printf("do_bgfg: Internal error\n");
		exit(0);
	}
	/* $end handout */
	return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
	/*get jot by pid*/
	job_t *job = getjobpid(jobs, pid);

	/*busy loop to wait process status change*/
	while (job->state == FG)
		sleep(1);

	if (verbose)
	{
		printf("waitfg: Process (%d) no longer the fg process\n", pid);
	}
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig)
{
	int old_errno = errno; //Save old errno
	int status;
	pid_t pid;
	int jid;
	job_t *job;
	sigset_t full_mask;
	sigset_t mask;
	sigfillset(&full_mask); //generate fullmask
	if (verbose)
		printf("sigchld_handler: entering");

	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
	{
		sigprocmask(SIG_BLOCK, &full_mask, &mask); //block signals
		job = getjobpid(jobs, pid);				   //get child
		jid = job->jid;
		if (WIFSTOPPED(status))
		{
			/*Update Status && Notify user*/
			job->state = ST;
			printf("Job [%d] (%d) stopped by signal %d\n", jid, pid, WSTOPSIG(status));
		}
		else
		{

			if (WIFSIGNALED(status))
				printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, WTERMSIG(status)); //if Signaled
			else
			{
				if (WTERMSIG(status))
				{
					unix_error("waitpid error");
				}
				if (verbose)
					printf("sigchld_handler: Job [%d] (%d) terminates OK (status %d)\n", jid, pid, WTERMSIG(status));
			}
			if (deletejob(jobs, pid) && verbose)
			{
				printf("sigchld_handler: Job [%d] (%d) deleted\n", jid, pid);
			}
		}

		sigprocmask(SIG_SETMASK, &mask, NULL);
	}
	if (pid && (pid != -1 || errno != ECHILD))
	{
		unix_error("sigchld_handler wait error");
	}
	if (verbose)
		printf("sigchld_handler: exiting");
	errno = old_errno;
	return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig)
{
	int old_errno = errno; //Save old errno
	int status;
	pid_t pid;
	int jid;
	job_t *job;
	sigset_t full_mask;
	sigset_t mask;
	sigfillset(&full_mask); //generate fullmask

	if (verbose)
		printf("sigint_handler: entering");

	sigprocmask(SIG_BLOCK, &full_mask, &mask);
	pid = fgpid(jobs);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	if (pid)
	{
		if (kill(-pid, SIGINT) < 0)
			unix_error("kill (sigint) error");
		if (verbose)
			printf("sigint_handler: Job (%d) killed\n", pid);
	}

	errno = old_errno;

	if (verbose)
		printf("sigint_handler: exiting");
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig)
{
	int old_errno = errno; //Save old errno
	int status;
	pid_t pid;
	int jid;
	job_t *job;
	sigset_t full_mask;
	sigset_t mask;
	sigfillset(&full_mask); //generate fullmask

	if (verbose)
		printf("sigtstp_handler: entering");

	sigprocmask(SIG_BLOCK, &full_mask, &mask); //block
	pid = fgpid(jobs);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	if (pid)
	{
		if (kill(-pid, SIGTSTP) < 0)
			unix_error("kill (tstp) error");
		if (verbose)
			printf("sigstp_handler: Job [%d] (%d) stopped\n", pid2jid(pid), pid);
	}

	errno = old_errno;

	if (verbose)
		printf("sigtstp_handler: exiting");
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(job_t *job)
{
	job->pid = 0;
	job->jid = 0;
	job->state = UNDEF;
	job->cmdjobsline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(job_t *jobs)
{
	int i;

	for (i = 0; i < MAXJOBS; i++)
		clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(job_t *jobs)
{
	int i, max = 0;

	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].jid > max)
			max = jobs[i].jid;
	return max;
}

/* addjob - Add a job to the job list */
int addjob(job_t *jobs, pid_t pid, int state, char *cmdline)
{
	int i;

	if (pid < 1)
		return 0;

	for (i = 0; i < MAXJOBS; i++)
	{
		if (jobs[i].pid == 0)
		{
			jobs[i].pid = pid;
			jobs[i].state = state;
			jobs[i].jid = nextjid++;
			if (nextjid > MAXJOBS)
				nextjid = 1;
			strcpy(jobs[i].cmdjobsline, cmdline);
			if (verbose)
			{
				printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdjobsline);
			}
			return 1;
		}
	}
	printf("Tried to create too many jobs\n");
	return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(job_t *jobs, pid_t pid)
{
	int i;

	if (pid < 1)
		return 0;

	for (i = 0; i < MAXJOBS; i++)
	{
		if (jobs[i].pid == pid)
		{
			clearjob(&jobs[i]);
			nextjid = maxjid(jobs) + 1;
			return 1;
		}
	}
	return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(job_t *jobs)
{
	int i;

	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].state == FG)
			return jobs[i].pid;
	return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
job_t *getjobpid(job_t *jobs, pid_t pid)
{
	int i;

	if (pid < 1)
		return NULL;
	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].pid == pid)
			return &jobs[i];
	return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
job_t *getjobjid(job_t *jobs, int jid)
{
	int i;

	if (jid < 1)
		return NULL;
	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].jid == jid)
			return &jobs[i];
	return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid)
{
	int i;

	if (pid < 1)
		return 0;
	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].pid == pid)
		{
			return jobs[i].jid;
		}
	return 0;
}

/* listjobs - Print the job list */
void listjobs(job_t *jobs)
{
	int i;

	for (i = 0; i < MAXJOBS; i++)
	{
		if (jobs[i].pid != 0)
		{
			printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
			switch (jobs[i].state)
			{
			case BG:
				printf("Running ");
				break;
			case FG:
				printf("Foreground ");
				break;
			case ST:
				printf("Stopped ");
				break;
			default:
				printf("listjobs: Internal error: job[%d].state=%d ",
					   i, jobs[i].state);
			}
			printf("%s", jobs[i].cmdjobsline);
		}
	}
}
/******************************
 * end job list helper routines
 ******************************/

/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void)
{
	printf("Usage: shell [-hvp]\n");
	printf("   -h   print this message\n");
	printf("   -v   print additional diagnostic information\n");
	printf("   -p   do not emit a command prompt\n");
	exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
	fprintf(stdout, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
	fprintf(stdout, "%s\n", msg);
	exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler)
{
	struct sigaction action, old_action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask); /* block sigs of type being handled */
	action.sa_flags = SA_RESTART; /* restart syscalls if possible */

	if (sigaction(signum, &action, &old_action) < 0)
		unix_error("Signal error");
	return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig)
{
	printf("Terminating after receipt of SIGQUIT signal\n");
	exit(1);
}
