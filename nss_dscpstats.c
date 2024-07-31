/*
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/module.h>
#include <linux/kernel.h>

#include "nss_dscpstats.h"
#include "exports/nss_dscpstats_nl_public.h"
#include "nss_dscpstats_nl.h"

/*
 * qca_nss_dscp_stat_init()
 *	register netlink ops and initialize hash table
 *
 * returns 0 on successful initialization else return -1
 */
int __init nss_dscpstats_init(void)
{
	int error = 0;

	nss_dscpstats_info("Init NSS client dscp stats handler\n");

	if (num_clients < NSS_DSCPSTATS_CLIENT_HASH_RATIO) {
		num_clients = NSS_DSCPSTATS_CLIENT_HASH_RATIO;
	}

	error = nss_dscpstats_db_init_table();
	if (error != 0) {
		return error;
	}

	error = nss_dscpstats_nl_init();
	if (error != 0) {
		return error;
	}

	nss_dscpstats_info("Init dscpstats complete\n");
	return 0;
}

/*
 * qca_nss_dscp_stat_init()
 *	unregister netlink ops and free the hash table
 *
 */
void __exit nss_dscpstats_exit(void)
{
	nss_dscpstats_info("Exit dscpstats complete\n");

	nss_dscpstats_nl_exit();

	/*
	 * free memory assigned to hashtable
	 */
	nss_dscpstats_db_free_table();
}

module_init(nss_dscpstats_init);
module_exit(nss_dscpstats_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("DSCP STAT");

module_param(num_clients, int, 0644);
MODULE_PARM_DESC(num_clients, "Number of clients supported");
