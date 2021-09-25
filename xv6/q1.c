#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  int a = 12;
  printf(1, "Hello, Intialised an int with space %d\n", sizeof(a));
  printf(1, "The memory allocated to this process is %d\n", getProcessSize());
  exit();
}