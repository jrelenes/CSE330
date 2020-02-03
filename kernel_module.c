#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
MODULE_LICENSE ("GPL");

int team_names_init(void)
{
pr_alert("Jose Elenes, Elizabeth Ramirez, Mei Lu\n");
return 0;
}
void team_names_exit(void)
{
pr_alert("Jose Elenes, Elizabeth Ramirez, Mei Lu\n");
}
 
module_init(team_names_init);
module_exit(team_names_exit);

