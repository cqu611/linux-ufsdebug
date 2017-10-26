#include <linux/module.h>
#include <linux/moduleparam.h>

#include "ufs/ufshcd.h"


#ifdef CONFIG_SCSI_UFSHCD
static int null_ufshcd_register(void) 
{
	int sz = sizeof(ufs_hba);
	pr_err("------------------------------------------------------\nnull_dev: %d ------------------------------------------------\n-----------------------------------------------\n", sz);
	return -EINVAL;
}

#else
static int null_ufshcd_register(void) 
{
	pr_err("null_dev: CONFIG_SCSI_UFSHCD needs to be enabled for UFSHCD\n");
	return -EINVAL;
}
#endif /* CONFIG_SCSI_UFSHCD */



static int __init nulldev_init(void) 
{
	return null_ufshcd_register();
}


static void __exit nulldev_exit(void)
{

}

module_init(nulldev_init);
module_exit(nulldev_exit);

MODULE_AUTHOR("Xiong Xiong <xxiong@cqu.edu.cn>");
MODULE_LICENSE("GPL");
