/* SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause */
/*
 * Copyright (C) 2023  Braiins Systems s.r.o.
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
/*
 * Copyright (C) 2018, STMicroelectronics - All Rights Reserved
 */

#ifndef __CONFIG_H
#define __CONFIG_H
#include <linux/sizes.h>
#include <asm/arch/stm32.h>

#ifndef CONFIG_TFABOOT
/* PSCI support */
#define CONFIG_ARMV7_SECURE_BASE		STM32_SYSRAM_BASE
#define CONFIG_ARMV7_SECURE_MAX_SIZE		STM32_SYSRAM_SIZE
#endif

/*
 * Configuration of the external SRAM memory used by U-Boot
 */
#define CONFIG_SYS_SDRAM_BASE			STM32_DDR_BASE
#define CONFIG_SYS_INIT_SP_ADDR			CONFIG_SYS_TEXT_BASE

/*
 * Console I/O buffer size
 */
#define CONFIG_SYS_CBSIZE			SZ_1K

/*
 * default load address used for command tftp,  bootm , loadb, ...
 */
#define CONFIG_LOADADDR			0xc2000000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/* ATAGs */
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

/*
 * For booting Linux, use the first 128 MB of memory.
 */
#define CONFIG_SYS_BOOTMAPSZ		SZ_128M

/* Extend size of kernel image for uncompression */
#define CONFIG_SYS_BOOTM_LEN		SZ_32M

/*MMC SD*/
#define CONFIG_SYS_MMC_MAX_DEVICE	3

/* Ethernet need */
#ifdef CONFIG_DWC_ETH_QOS
#define CONFIG_SYS_NONCACHED_MEMORY	(1 * SZ_1M)	/* 1M */
#define CONFIG_SERVERIP                 192.168.1.1
#define CONFIG_BOOTP_SERVERIP
#define CONFIG_SYS_AUTOLOAD		"no"
#endif

/*****************************************************************************/
#ifdef CONFIG_DISTRO_DEFAULTS
/*****************************************************************************/

#if !defined(CONFIG_SPL_BUILD)

#define SET_ROOTFS_PART "set_rootfs_part=" \
	"part number mmc $boot_instance rootfs${rootfs_index} rootfs_part && " \
	"setexpr rootfs_part dec $rootfs_part && " \
	"\0"

#define BOOTCMD_BOS "bootcmd_bos=" \
	"run set_rootfs_part && " \
	"echo Loading U-Boot script... && " \
	"load mmc ${boot_instance}:${rootfs_part} $scriptaddr boot/$script && " \
	"source $scriptaddr" \
	"\0"

/*
 * memory layout for 32M uncompressed/compressed kernel,
 * 1M fdt, 1M script, 1M pxe and the ramdisk at the end.
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"kernel_addr_r=0xc2000000\0" \
	"fdt_addr_r=0xc4000000\0" \
	"fdtoverlay_addr_r=0xc4100000\0" \
	"scriptaddr=0xc4100000\0" \
	"pxefile_addr_r=0xc4200000\0" \
	"ramdisk_addr_r=0xc4400000\0" \
	"script=boot.scr\0" \
	SET_ROOTFS_PART \
	BOOTCMD_BOS

#endif /* ifndef CONFIG_SPL_BUILD */
#endif /* ifdef CONFIG_DISTRO_DEFAULTS*/

#endif /* __CONFIG_H */
