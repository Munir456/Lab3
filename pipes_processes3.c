#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{
  int pipefd1[2];
  int pipefd2[2];
  int pid;
  int pid2;
  int pid3; 

  int i;

  int curr; 

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  grep_args[1] = argv[1]; 

  if (argc <1){
    printf("Enter grep: ");
    exit(1);
  }

  pipe(pipefd1); 
  pipe(pipefd2);

  pid = fork();

  if (pid == 0){
    dup2(pipefd1[1], 1);
    //make it close all unused pipes
    close(pipefd1[1]);
    close(pipefd1[0]);
    close(pipefd2[0]);
    close(pipefd2[1]);

    //how to make it execute?????
    execvp(*cat_args, cat_args);
    }

  else if (pid <0){
    printf("Error with cat fork");
  }
  //basically the same as cat
  else{
    pid2 = fork();
    if (pid2 == 0){
      dup2(pipefd1[0],0);
      dup2(pipefd2[1],1);

      close(pipefd1[1]);
      close(pipefd1[0]);
      close(pipefd2[0]);
      close(pipefd2[1]);

      execvp(*grep_args, grep_args);

    }

    else if(pid2 <0){
      printf("Error with forking grep");
    }

    else{
      pid3 = fork();

    pid3 = fork();
    if (pid3 == 0){
      dup2(pipefd2[0],0);

      close(pipefd1[1]);
      close(pipefd1[0]);
      close(pipefd2[0]);
      close(pipefd2[1]);

      execvp(*sort_args, sort_args);
      }

      else if (pid3<0)
      {
        printf("Error with sorting fork");
      }
    }
  }
  //close all the other pipes just in case 
  close(pipefd1[1]);
  close(pipefd1[0]);
  close(pipefd2[0]);
  close(pipefd2[1]);
  for(i=0; i<3; i++){
    wait(&curr);
  }
}