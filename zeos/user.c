#include <libc.h>

char buff[24];

int pid;

int add(int par1, int par2) {
  return par1 + par2;
}

int addASM(int, int);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

	//int result1 = add(0x42, 0x666);
 	//int result2 = addASM(0x42, 0x666);

	//Test de la syscall feta amb int
	write(1, "\nLa syscall write va correctament\n", 34);
	


	//Test de la syscall gettime feta amb sysenter
	char *buffer = "\0\0\0\0\0\n";

    	write(1, "Gettime 1: ", 11);
    	itoa(gettime(), buffer);
	write(1, buffer, 6);

	while(gettime() < 500);

    	write(1, "Gettime 2: ", 11);
    	itoa(gettime(), buffer);
	write(1, buffer, 6);

	while(gettime() < 1000);

    	write(1, "Gettime 3: ", 11);
    	itoa(gettime(), buffer);
	write(1, buffer, 6);

	while(gettime() < 1501);

        write(1, "Gettime 4: ", 11);
        itoa(gettime(), buffer);
        write(1, buffer, 6);


	//Descomentar per testejar page fault exception
	//char *p = 0;
	//*p = 'x';

	while(1) {}
}
