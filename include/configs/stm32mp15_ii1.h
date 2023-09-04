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

#define UUID_GPT_EMMC_DISK	"b5604747-268c-43d1-b828-de0cb3fb47e0"
#define UUID_GPT_SD_DISK	"be7fb69a-5d53-4500-bd3b-2e53a7e6eeeb"

#define PARTS_UUID_GPT_EMMC \
	"uuid_gpt_emmc_disk=b5604747-268c-43d1-b828-de0cb3fb47e0\0" \
	"uuid_gpt_emmc_fip=d4b9b338-c25a-4e7d-b7a7-a6ddb9baeb70\0" \
	"uuid_gpt_emmc_rootfs1=83c998dd-9f28-455e-86b3-1b379174ff28\0" \
	"uuid_gpt_emmc_rootfs2=b7f7cbe9-e4f5-430f-bc4c-4ca78e8336a0\0" \
	"uuid_gpt_emmc_nvdata=9e51faf6-4968-4f73-b47f-d7f08708bfcb\0"

#define PARTS_UUID_GPT_SD \
	"uuid_gpt_sd_disk=be7fb69a-5d53-4500-bd3b-2e53a7e6eeeb\0" \
	"uuid_gpt_sd_fsbl1=c9d5d7e5-39a2-4d0e-b809-a5b055aad7bb\0" \
	"uuid_gpt_sd_fsbl2=f410f718-415d-4bcb-9140-347f9d0fdf59\0" \
	"uuid_gpt_sd_fip=98616d47-3eba-4222-a9fb-02205041e029\0" \
	"uuid_gpt_sd_vendor=950bfac3-e6d6-4241-a2b2-b96ae7d3b3fb\0" \
	"uuid_gpt_sd_rootfs=aa9471f7-eea9-45d4-89d2-9c2dd99ef464\0" \
	"uuid_gpt_sd_nvdata=29988c36-5ae4-48e8-afe9-52dbc535abe7\0"

#define EMMC_HWPART_USER	"0"
#define EMMC_HWPART_BOOT0	"1"
#define EMMC_HWPART_BOOT1	"2"
#define EMMC_HWPART_RPMB	"3"
#define EMMC_HWPART_GP1		"4"

#define PRESERVE_DYNAMIC_VARIABLES \
	"board_name=$board_name && " \
	"boot_device=$boot_device && " \
	"boot_instance=$boot_instance && " \
	"bootcount=$bootcount && " \
	"dev_emmc=$dev_emmc && " \
	"dev_sd=$dev_sd && " \
	"ethaddr=$ethaddr && " \
	"fdtcontroladdr=$fdtcontroladdr && " \
	"fdtfile=$fdtfile && "

#ifdef CONFIG_BOOT_INIT_EMMC
/* eMMC initialization */
#define UUID_GPT_DISK UUID_GPT_EMMC_DISK

#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=fip,start=1MiB,size=15MiB,uuid=${uuid_gpt_emmc_fip};" \
	"name=rootfs1,size=160MiB,uuid=${uuid_gpt_emmc_rootfs1};" \
	"name=rootfs2,size=160MiB,uuid=${uuid_gpt_emmc_rootfs2};" \
	"name=nvdata,size=-,uuid=${uuid_gpt_emmc_nvdata}"

#define ROOTFS_INDEX "1"

// TODO: BOS-1084 - Remove 'recovery_counter' from env not to trigger the recovery process
#define DOWNGRADE_ENV "downgrade_env=" \
	PRESERVE_DYNAMIC_VARIABLES \
	"rootfs_index=$rootfs_index && " \
	"env default -a && " \
	"setenv bootcmd \"$bootcmd_default\" && " \
	"env delete bootcmd_default bootcmd_bos rootfs_index && " \
	"setenv rootfs_index $rootfs_index && " \
	"saveenv && " \
	"setenv set_ethaddr 'test -z \"$ethaddr\" || setenv ethaddr $ethaddr' && " \
	"run set_ethaddr" \
	"\0"

#define FACTORY_ENV \
	"emmc_factory_env_hwpart="EMMC_HWPART_BOOT1"\0" \
	"emmc_factory_env_blk=0x00001800\0" \
	"emmc_factory_env_cnt=0x00000040\0" \
	"emmc_factory_env_size=0x8000\0" \
	DOWNGRADE_ENV

#define FACTORY_ENV_IMPORT \
	"mmc dev $dev_emmc $emmc_factory_env_hwpart && " \
	"mmc read $scriptaddr $emmc_factory_env_blk $emmc_factory_env_cnt && " \
	"env import -c $scriptaddr $emmc_factory_env_size && "

#define FACTORY_INIT \
	"part number mmc $dev_emmc rootfs${rootfs_index} rootfs_part && " \
	"part start mmc $dev_emmc $rootfs_part rootfs_start && " \
	"mmc dev $dev_emmc $emmc_factory_rootfs_hwpart && " \
	"mmc read $loadaddr $emmc_factory_rootfs_blk $emmc_factory_rootfs_cnt && " \
	"mmc dev $dev_emmc && " \
	"mmc erase $emmc_erase_blk $emmc_erase_cnt && " \
	"mmc write $loadaddr $rootfs_start $emmc_factory_rootfs_cnt && "
#endif /* ifdef CONFIG_BOOT_INIT_EMMC */

#ifdef CONFIG_BOOT_INIT_SD
/* SD card initialization */
#define UUID_GPT_DISK UUID_GPT_SD_DISK

#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=fsbl1,start=4MiB,size=1MiB,uuid=${uuid_gpt_sd_fsbl1};" \
	"name=fsbl2,size=1MiB,uuid=${uuid_gpt_sd_fsbl2};" \
	"name=fip,size=6MiB,uuid=${uuid_gpt_sd_fip};" \
	"name=rootfs,size=160MiB,uuid=${uuid_gpt_sd_rootfs};" \
	"name=nvdata,size=-,uuid=${uuid_gpt_sd_nvdata}"

#define ROOTFS_INDEX ""

#define FACTORY_ENV \
	"partitions="PARTS_DEFAULT"\0" \

#define FACTORY_ENV_IMPORT ""

#define FACTORY_INIT \
	""
#endif /* ifdef CONFIG_BOOT_INIT_SD */

#define SET_ROOTFS_PART "set_rootfs_part=" \
	"part number mmc $boot_instance rootfs${rootfs_index} rootfs_part && " \
	"setexpr rootfs_part dec $rootfs_part" \
	"\0"

#define BOOTCMD_DEFAULT "bootcmd_default=" \
	"run set_rootfs_part && " \
	"echo Loading U-Boot script... && " \
	"load mmc ${boot_instance}:${rootfs_part} $scriptaddr boot/$script && " \
	"source $scriptaddr" \
	"\0"

#define BOOTCMD_BOS "bootcmd_bos=" \
	"echo Factory initialization...; " \
	FACTORY_ENV_IMPORT \
	"gpt write mmc $boot_instance $partitions && " \
	FACTORY_INIT \
	PRESERVE_DYNAMIC_VARIABLES \
	"env default -a && " \
	"setenv bootcmd \"$bootcmd_default\" && " \
	"env delete bootcmd_default bootcmd_bos && " \
	"saveenv && " \
	"setenv set_ethaddr 'test -z \"$ethaddr\" || setenv ethaddr $ethaddr' && " \
	"run set_ethaddr && " \
	"boot" \
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
	PARTS_UUID_GPT_EMMC \
	PARTS_UUID_GPT_SD \
	"uuid_gpt_disk="UUID_GPT_DISK"\0" \
	"rootfs_index="ROOTFS_INDEX"\0" \
	FACTORY_ENV \
	SET_ROOTFS_PART \
	BOOTCMD_DEFAULT \
	BOOTCMD_BOS

#endif /* ifndef CONFIG_SPL_BUILD */
#endif /* ifdef CONFIG_DISTRO_DEFAULTS*/

#endif /* __CONFIG_H */
