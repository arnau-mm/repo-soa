/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <interrupt.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));


struct task_struct *list_head_to_task_struct(struct list_head *l)
{
	struct task_struct * ts = (struct task_struct *) ((unsigned long)l & 0xfffff000);
  	return ts;
}

extern struct list_head blocked;
struct list_head freequeue;
struct list_head readyqueue;
struct task_struct * idle_task;

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{	
	struct list_head *first = list_first(&freequeue);
	idle_task = list_head_to_task_struct(first);
	union task_union *tu = (union task_union *) idle_task;
	list_del(first); 
	tu -> task.PID = 0;
	allocate_DIR((struct task_struct*) tu);
	tu -> stack[1022] = 0;
	tu -> stack[1023] =  (unsigned long) cpu_idle;
	tu -> task.kernel_esp = tu -> stack[1022];

}

void init_task1(void)
{
	struct list_head *first = list_first(&freequeue);
	union task_union *tu = (union task_union *) list_head_to_task_struct(first);
	list_del(first); 
	tu -> task.PID = 1;
	allocate_DIR((struct task_struct*) tu);
	set_user_pages((struct task_struct *)tu);
	writeMSR(0x175, tu -> stack);
}


void init_sched()
{
	INIT_LIST_HEAD(&readyqueue);
	INIT_LIST_HEAD(&freequeue);
	for (int i = 0; i < NR_TASKS; ++i) {
		list_add_tail(&task[i].task.list, &freequeue);
	}
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

