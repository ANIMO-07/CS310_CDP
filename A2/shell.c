#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <pwd.h>
#include "tokenizer.h"
#include <grp.h>

/* Convenience macro to silence compiler warnings about unused function parameters. */
#define unused __attribute__((unused))

extern int errno;

/* Whether the shell is connected to an actual terminal or not. */
bool shell_is_interactive;

/* File descriptor for the shell input */
int shell_terminal;

/* Terminal mode settings for the shell */
struct termios shell_tmodes;

/* Process group id for the shell */
pid_t shell_pgid;

int cmd_exit(struct tokens *tokens);
int cmd_help(struct tokens *tokens);
int cmd_uid(struct tokens *tokens);
int cmd_gid(struct tokens *tokens);
int cmd_groups(struct tokens *tokens);


/* Built-in command functions take token array and return int */
typedef int cmd_fun_t(struct tokens *tokens);

/* Built-in command struct and lookup table */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_exit, "exit", "exit the command shell"},
  {cmd_uid, "uid", "find the id of the current user"},
  {cmd_gid, "gid", "find the group id of the current user"},
  {cmd_groups, "groups", "find the groups for the current user"}
};

/* Prints a helpful description for the given command */
int cmd_help(unused struct tokens *tokens) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    printf("%s - %s\n", cmd_table[i].cmd, cmd_table[i].doc);
  return 1;
}

/* Exits this shell */
int cmd_exit(unused struct tokens *tokens) {
  exit(0);
}

/* Prints the user id(real) of the current user */
int cmd_uid(unused struct tokens *tokens){
  unsigned int id = getuid();
  struct passwd *pws;
  pws = getpwuid(id);
  if ( errno != 0){
    int errnum = errno;
    fprintf(stderr,"An error occured while resolving the name for the id %u with the ERROR CODE: %s\n", id,strerror(errnum));
    return -1;
  }
  printf("%s\n", pws->pw_name);
  return 1;
}


/* Prints the user id(real) of the current user */
int cmd_gid(unused struct tokens *tokens){
  unsigned int id = getgid();
  struct passwd *pws;
  pws = getpwuid(id);
  if ( errno != 0){
    int errnum = errno;
    fprintf(stderr, "An error occured while resolving the name for the id %u with the ERROR CODE: %s\n", id,strerror(errnum));
    return -1;
  }
  
  printf("%s\n", pws->pw_name);
  return 1;
}

/* Prints the groups of the current user */
int cmd_groups(unused struct tokens *tokens){
  
  /* A psuedo call to get the number of groups for the current user */
  int num_of_groups = getgroups(0, NULL);
  
  if ( num_of_groups == -1){
    int errnum = errno;
    fprintf(stderr, "Error finding the number of groups for the user: %s\n", strerror(errnum));
    return -1;
  }

  gid_t group_ids[num_of_groups];
  num_of_groups = getgroups(num_of_groups, group_ids);

  if ( num_of_groups == -1){
    int errnum = errno;
    fprintf(stderr, "Error finding the groups for the user: %s\n", strerror(errnum));
    return -1;
  }
 
  
  printf("The groups of the current user are:\n");
  for( int i = 0;i<num_of_groups;i++){
    struct group *grpname = getgrgid(group_ids[i]);
    printf("%s ", grpname->gr_name); 
  }
  printf("\n");
  return 1;
}

int file_check(char* loc){
  if ( access(loc, F_OK) == 0)
    return 1;
  else{
    return -1;
  }
}

int run_bin_file(struct tokens *tokens){
  char* loc = tokens_get_token(tokens, 0);
  if ( file_check(loc) == -1 ){
    fprintf(stderr, "Executable file not found at the given location!\n");
    return -1;
  }

  /* Making a copy of the tokenized arguments */
  unsigned int num_of_args = tokens_get_length(tokens);
  
  char* args[num_of_args+1];
  for ( int i = 0;i<num_of_args;i++){
    args[i] = tokens_get_token(tokens, i);
  } 
  args[num_of_args] = NULL;
  
  pid_t pid = fork();
  if ( pid == 0){
    execv(args[0], args);
    }
  else{
    wait(&pid);
  }
  return 1;
}


/* Looks up the built-in command, if it exists. */
int lookup(char cmd[]) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0))
      return i;
  return -1;
}



/* Intialization procedures for this shell */
void init_shell() {
  /* Our shell is connected to standard input. */
  shell_terminal = STDIN_FILENO;

  /* Check if we are running interactively */
  shell_is_interactive = isatty(shell_terminal);

  if (shell_is_interactive) {
    /* If the shell is not currently in the foreground, we must pause the shell until it becomes a
     * foreground process. We use SIGTTIN to pause the shell. When the shell gets moved to the
     * foreground, we'll receive a SIGCONT. */
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
      kill(-shell_pgid, SIGTTIN);

    /* Saves the shell's process id */
    shell_pgid = getpid();

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);

    /* Save the current termios to a variable, so it can be restored later. */
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}

int main(unused int argc, unused char *argv[]) {
  init_shell();

  static char line[4096];
  int line_num = 0;

  /* Please only print shell prompts when standard input is not a tty */
  if (shell_is_interactive)
    fprintf(stdout, "%d: ", line_num);

  while (fgets(line, 4096, stdin)) {
    /* Split our line into words. */
    struct tokens *tokens = tokenize(line);

    /* Find which built-in function to run. */
    int fundex = lookup(tokens_get_token(tokens, 0));

    if (fundex >= 0) {
      cmd_table[fundex].fun(tokens);
    } else {
      run_bin_file(tokens);
    }

    if (shell_is_interactive)
      /* Please only print shell prompts when standard input is not a tty */
      fprintf(stdout, "%d: ", ++line_num);

    /* Clean up memory */
    tokens_destroy(tokens);
  }

  return 0;
}
