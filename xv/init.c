// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "param.h"


char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid, wpid;

  setpriority(getpid(), 1);
  settickets(getpid(), MAX_TICKETS);

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  #ifdef DEFAULT
    printf(1, "Scheduler policy: DEFAULT\n");
  #else
  #ifdef LOTTERY
    printf(1, "Scheduler policy: LOTTERY\n");
  #endif
  #endif
  

  for(;;){
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}
