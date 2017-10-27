#include <linux/module.h>
#include <linux/moduleparam.h>
#include <scsi/scsi_host.h>

#include "ufs/ufshcd.h"

#ifdef CONFIG_SCSI

struct ufs_hba *null_hba;

struct pcim_iomap_devres {
	void __iomem *table[6];
};

static int null_ufshcd_queuecommand(struct Scsi_Host *host, struct scsi_cmnd *cmd)
{
	return 0;
}

static int null_ufshcd_slave_alloc(struct scsi_device *sdev)
{
	return 0;
}

static int null_ufshcd_slave_configure(struct scsi_device *sdev)
{
	return 0;
}

static void null_ufshcd_slave_destroy(struct scsi_device *sdev)
{

}

static int null_ufshcd_change_queue_depth(struct scsi_device *sdev, int depth)
{
	return 0;
}

static int null_ufshcd_abort(struct scsi_cmnd *cmd)
{
	return 0;
}

static int null_ufshcd_eh_device_reset_handler(struct scsi_cmnd *cmd)
{
	return 0;
}

static int null_ufshcd_eh_host_reset_handler(struct scsi_cmnd *cmd)
{
	return 0;
}

static enum blk_eh_timer_return null_ufshcd_eh_timed_out(struct scsi_cmnd *scmd)
{
	return BLK_EH_NOT_HANDLED;
}

static struct scsi_host_template null_ufshcd_driver_template = {
	.module			= THIS_MODULE,
	.name			= UFSHCD,
	.proc_name		= UFSHCD,
	.queuecommand		= null_ufshcd_queuecommand,
	.slave_alloc		= null_ufshcd_slave_alloc,
	.slave_configure	= null_ufshcd_slave_configure,
	.slave_destroy		= null_ufshcd_slave_destroy,
	.change_queue_depth	= null_ufshcd_change_queue_depth,
	.eh_abort_handler	= null_ufshcd_abort,
	.eh_device_reset_handler = null_ufshcd_eh_device_reset_handler,
	.eh_host_reset_handler   = null_ufshcd_eh_host_reset_handler,
	.eh_timed_out		= null_ufshcd_eh_timed_out,
	.this_id		= -1,
	.sg_tablesize		= 128,
	.cmd_per_lun		= 32,
	.can_queue		= 32,
	.max_host_blocked	= 1,
	.track_queue_depth	= 1,
};

static int null_ufshcd_register(void) 
{
	struct ufs_hba *hba;
	struct Scsi_Host *host;
	struct device *dev;
	void __iomem *mmio_base;
	unsigned int irq = 15;
	struct pcim_iomap_devres *dr;
	int err = 0;

	/* alloc null device */
	dev = kzalloc(sizeof(struct device), GFP_KERNEL);
	if (!dev) {
		dev_err(dev,
		"Invalid memory reference for dev is NULL\n");
		err = -ENODEV;
		goto out_error;
	}

	/* alloc Scsi_Host */
	host = scsi_host_alloc(&null_ufshcd_driver_template,
				sizeof(struct ufs_hba));
	if (!host) {
		dev_err(dev, "scsi_host_alloc failed\n");
		err = -ENOMEM;
		goto out_error;
	}
	hba = shost_priv(host);
	hba->host = host;
	hba->dev = dev;
	
	INIT_LIST_HEAD(&hba->clk_list_head);

	/* mmio_base init */
	dr = devres_alloc_node(NULL, sizeof(*dr), GFP_KERNEL, NUMA_NO_NODE);
	mmio_base = dr->table;
	if (!mmio_base) {
		dev_err(hba->dev,
		"Invalid memory reference for mmio_base is NULL\n");
		err = -ENODEV;
		goto out_error;
	}
	hba->mmio_base = mmio_base;
	hba->irq = irq;

	/* Set descriptor lengths to specification defaults */
	hba->desc_size.dev_desc = QUERY_DESC_DEVICE_DEF_SIZE;
	hba->desc_size.pwr_desc = QUERY_DESC_POWER_DEF_SIZE;
	hba->desc_size.interc_desc = QUERY_DESC_INTERCONNECT_DEF_SIZE;
	hba->desc_size.conf_desc = QUERY_DESC_CONFIGURATION_DEF_SIZE;
	hba->desc_size.unit_desc = QUERY_DESC_UNIT_DEF_SIZE;
	hba->desc_size.geom_desc = QUERY_DESC_GEOMETRY_DEF_SIZE;

	/* Host bus adapter init */
	
	/* Read capabilities registers */

	/* Get UFS version supported by the controller */

	/* Get Interrupt bit mask per version */

	/* Set DMA mask */

	/* Allocate memory for host memory space */

	/* Configure LRB */

	/* Initailize wait queue for task management */

	/* Initialize work queues */

	/* Initialize UIC command mutex */

	/* Initialize mutex for device management commands */

	/* Initialize device management tag acquire wait queue */

	/* IRQ registration */

	/* Host controller enable */

	null_hba = hba;

out_error:
	return err;
}

static void null_ufshcd_unregister(void) 
{
	/* kfree null_hba */
}

#else
static int null_ufshcd_register(void) 
{
	pr_err("null_dev: CONFIG_SCSI needs to be enabled for UFSHCD\n");
	return -EINVAL;
}

static void null_ufshcd_unregister(void) {}
#endif /* CONFIG_SCSI */



static int __init nulldev_init(void) 
{
	return null_ufshcd_register();
}


static void __exit nulldev_exit(void)
{
	null_ufshcd_unregister();
}

module_init(nulldev_init);
module_exit(nulldev_exit);

MODULE_AUTHOR("Xiong Xiong <xxiong@cqu.edu.cn>");
MODULE_LICENSE("GPL");
