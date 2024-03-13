// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/*
 * Copyright (C) 2024  Braiins Systems s.r.o.
 *
 * This file is part of Braiins Open-Source Initiative (BOSI).
 *
 * BOSI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Please, keep in mind that we may also license BOSI or any part thereof
 * under a proprietary license. For more information on the terms and conditions
 * of such proprietary license or if you have any other questions, please
 * contact us at opensource@braiins.com.
 */

#include <common.h>
#include <command.h>
#include <env_internal.h>
#include <log.h>
#include "env_utils.h"

DECLARE_GLOBAL_DATA_PTR;

static bool env_prepare(struct hsearch_data *env_htab_prev)
{
	struct env_entry *ep, e = { .key = NULL, };

	if (!env_fix_bootcmd())
		return false;

	e.key = PRESERVE_KEY_NAME;
	hsearch_r(e, ENV_FIND, &ep, &env_htab, 0);
	if (ep != NULL) {
		if (!env_preserve(ep->data, env_htab_prev))
			return false;
		if (!hdelete_r(e.key, &env_htab, 0))
			return false;
	}

	return true;
}

int do_ii_saveenv(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	int res = 1;
	struct hsearch_data env_htab_prev = {
		.table = NULL,
	};

	env_swap(NULL, &env_htab_prev);
	if (!env_prepare(&env_htab_prev)) {
		pr_err("env_prepare failed");
	} else {
		env_save();
		res = 0;
	}

	env_swap(&env_htab_prev, &env_htab_prev);
	hdestroy_r(&env_htab_prev);
	return res;
}

U_BOOT_CMD(
	ii_saveenv, 1, 1, do_ii_saveenv,
	"save default environment only but preserve control variables",
	""
);
