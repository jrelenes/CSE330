#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/cred.h>
#include <linux/tty.h>
#include <linux/uidgid.h>
#include <asm/param.h>
#include <linux/string.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

int pid;
unsigned int present = 0;
unsigned int swapped = 0;

void walker_main(void);
void count_pages(struct task_struct*);
void counter(struct mm_struct *mm, unsigned long vpage);

void counter(struct mm_struct *mm, unsigned long vpage)
{

	pgd_t *pgd;
	p4d_t *p4d;
	pmd_t *pmd;
	pud_t *pud;
	pte_t *ptep, pte;

	pgd = pgd_offset(mm, vpage);           //you get the pgd from mm and the page address
	if (pgd_none(*pgd) || pgd_bad(*pgd)){        //if pgd does not exist or bad, return
		return;
	}

	p4d = p4d_offset(pgd, vpage);           //you get the p4d from pgd and the page address
	if (p4d_none(*p4d) || p4d_bad(*p4d)){            //if p4d does not exist or bad, return
		return;
	}

	pud = pud_offset(p4d, vpage);           //you get the pud from p4d and the page address
	if (pud_none(*pud) || pud_bad(*pud)){           //if pud does not exist or bad, return
		return;
	}

	pmd = pmd_offset(pud, vpage);        //you get the pmd from pud and the page address
	if (pmd_none(*pmd) || pmd_bad(*pmd)){	         //if pmd does not exist or bad, return
		return;
	} 

	ptep = pte_offset_map(pmd, vpage); //you get the pte from pmd and the page address
	if (!ptep) {
		return;
	}                                                  //if pte does not exit, return

	pte = *ptep;

	if(pte_present(pte) && !pte_none(pte))
		present++;

	if(!pte_present(pte) && !pte_none(pte))
		swapped++;
}

void count_pages(struct task_struct *task)
{
	struct vm_area_struct *vma;
	long unsigned vpage; 

	for (vma = task->mm->mmap; vma; vma=vma->vm_next) {
        for (vpage = vma->vm_start; vpage< vma->vm_end; vpage += PAGE_SIZE) {
			counter(task->mm, vpage);	
        }
	}

	printk("PRESENT: %d SWAPPED: %d", present, swapped);
}

void walker_main(void)
{
	struct task_struct *iter;	

	for_each_process(iter) {
		if(iter->pid == pid) {
			count_pages(iter);
			return;
		}
	}

	printk("process does not exist, please reload the kernel module with another pid.");
}

int walker_init(void)
{
	walker_main();
	return 0;
}

void walker_exit(void)
{
	printk("exiting walker\n");
}

module_init(walker_init);
module_exit(walker_exit);
module_param(pid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
