/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <io.h>

#include <zeos_interrupt.h>
#include <stddef.h>
extern int zeos_ticks;

Gate idt[IDT_ENTRIES];
Register    idtR;

char char_map[] =
{
  '\0','\0','1','2','3','4','5','6',
  '7','8','9','0','\'','�','\0','\0',
  'q','w','e','r','t','y','u','i',
  'o','p','`','+','\0','\0','a','s',
  'd','f','g','h','j','k','l','�',
  '\0','�','\0','�','z','x','c','v',
  'b','n','m',',','.','-','\0','*',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0','\0','\0','\0','\0','\0','7',
  '8','9','-','4','5','6','+','1',
  '2','3','0','\0','\0','\0','<','\0',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0'
};

void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void keyboard_handler(void);
void clock_handler(void);
void p_f_handler(unsigned long param, unsigned long eip);

//Fast syscall only
void syscall_handler_sysenter(); 
void system_call_handler();

void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;
  
  set_handlers();
  

  /* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */
  
  setInterruptHandler(32, clock_handler, 0);
  setInterruptHandler(33, keyboard_handler, 0);
  setInterruptHandler(14, p_f_handler, 0);

  //USED FOR FAST SYSCALLS
  writeMSR(0x174, __KERNEL_CS);
  writeMSR(0X175, INITIAL_ESP);
  writeMSR(0x176, (unsigned long)syscall_handler_sysenter);


  //setTrapHandler(0x80, system_call_handler, 3);

  set_idt_reg(&idtR);
}




void keyboard_routine() {
  unsigned char c = inb(0x60);
  if (c & 0x80) {
    unsigned char r = char_map[c & 0x7F];
    printc_xy(0,0,r);
  }
}

void clock_routine() {
  zeos_ticks++;
  zeos_show_clock();
}


void ulongToHex(unsigned long num, char *hexStr, size_t size) {
    size_t i = size - 1;
    hexStr[i] = '\0';  // Agregar el car�cter nulo al final de la cadena

    do {
        unsigned int remainder = num % 16;
        hexStr[--i] = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
        num /= 16;
    } while (num != 0 && i > 0);

    // Copiar la cadena resultante a la posici�n inicial
    size_t j = 0;
    while (hexStr[i] != '\0') {
        hexStr[j++] = hexStr[i++];
    }
}



void p_f_routine(unsigned long param, unsigned long eip){
  printk("Proces generates a PAGE FAULT exception, at EIP:0x");
  char strEip[20];
  ulongToHex(eip, strEip, sizeof(strEip));
  printk(strEip);
  
  while(1);
}


  
