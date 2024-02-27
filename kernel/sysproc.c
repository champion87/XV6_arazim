#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL



#define LM_MAX_PAGES (8 * sizeof(uint64) )
typedef uint64 bitmask;

#define IS_ACC(pte) ((pte & PTE_A) != 0)
#define BUF_LEN(n) (n % 8 ? n/8 +1 : n/8)


void write_to_bitmask(bitmask *dest, int bit_num, int value)
{
  if (value)
  {
    *dest = *dest | (1 << bit_num);
  }
  else
  {
    *dest = *dest & ~(1 << bit_num);
  }
}

void clear_acc(pte_t *pte)
{
  *pte &= ~PTE_A;
}

int // assumes the buffer is of size ceil(n / 8) here n is the number of pages to check
sys_pgaccess(void)
{
  bitmask res;
  int is_acc;
  pte_t *pointer_to_current_pte;
  pagetable_t pt = myproc()->pagetable;

  uint64 current_va;
  argaddr(0, &current_va);

  int num_pages_to_check;
  argint(1, &num_pages_to_check);

  uint64 user_buffer;
  argaddr(2, &user_buffer);

  if (!(0 <= num_pages_to_check && num_pages_to_check <= LM_MAX_PAGES))
  {
    printf("Too many pages for pgaccess!\n");
    return -1;
  }

  for (int i = 0; i < num_pages_to_check; i++)
  {
    pointer_to_current_pte = walk(pt, current_va + i*PGSIZE, 0);// TODO given a virtual address, get pte
    // TODO should I take walk earlier and index the result?

    printf("%p\n", pointer_to_current_pte);

    is_acc = IS_ACC(*pointer_to_current_pte);
    clear_acc(pointer_to_current_pte); // TODO args
    write_to_bitmask(&res, i, is_acc);
  }

  copyout(pt, user_buffer, (char *) (&res), BUF_LEN(num_pages_to_check));

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
