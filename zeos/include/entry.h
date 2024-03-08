/*
 * entry.h - Definició del punt d'entrada de les crides al sistema
 */

#ifndef __ENTRY_H__
#define __ENTRY_H__

void keyboard_handler();
void system_call_handler();
void clock_handler();
void writeMSR(int index, int valor);
void syscall_handler_sysenter();
void page_f_handler();

#endif  /* __ENTRY_H__ */
