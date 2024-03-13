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
#include <malloc.h>
#include "env_utils.h"

DECLARE_GLOBAL_DATA_PTR;

void env_swap(struct hsearch_data *env_htab_new,
	      struct hsearch_data *env_htab_prev)
{
	struct hsearch_data htab_prev = {
		.table = env_htab.table,
		.size = env_htab.size,
		.filled = env_htab.filled,
		.change_ok = env_htab.change_ok,
	};

	if (env_htab_new != NULL) {
		env_htab.table = env_htab_new->table;
		env_htab.size = env_htab_new->size;
		env_htab.filled = env_htab_new->filled;
		gd->flags &= ~GD_FLG_ENV_DEFAULT;
	} else {
		env_htab.table = NULL;
		env_htab.size = 0;
		env_htab.filled = 0;
		env_set_default(NULL, 0);
	}

	memcpy(env_htab_prev, &htab_prev, sizeof(htab_prev));
}

bool env_preserve(char *variables, struct hsearch_data *env_htab_prev)
{
	struct env_entry *ep, e = { .key = NULL, };
	char *tmp, *variable;

	variables = strdup(variables);
	if (!variables) {
		pr_err("strdup(variables) failed");
		return false;
	}

	tmp = variables;
	while (true) {
		variable = strsep(&tmp, " ");
		if (!variable)
			break;
		debug("%s: variable: %s\n", __func__, variable);

		e.key = variable;
		hsearch_r(e, ENV_FIND, &ep, env_htab_prev, 0);
		if (ep != NULL) {
			env_set(variable, ep->data);
		}
	}

	free(variables);
	return true;
}

bool env_fix_bootcmd(void)
{
	struct env_entry *ep, e = { .key = NULL, };

	e.key = "bootcmd_default";
	hsearch_r(e, ENV_FIND, &ep, &env_htab, 0);
	if (ep == NULL)
		return false;
	env_set("bootcmd", ep->data);
	if (!hdelete_r(e.key, &env_htab, 0))
		return false;
	if (!hdelete_r("bootcmd_bos", &env_htab, 0))
		return false;

	return true;
}
