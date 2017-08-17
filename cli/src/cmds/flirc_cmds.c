/**
 * COPYRIGHT 2012 Flirc, Inc. All rights reserved.
 *
 * This copyright notice is Copyright Management Information under 17 USC 1202
 * and is included to protect this work and deter copyright infringement.
 * Removal or alteration of this Copyright Management Information without
 * the express written permission of Flirc, Inc. is prohibited, and any
 * such unauthorized removal or alteration will be a violation of federal law.
 *
 * @file
 * @brief   Flirc command file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>

#include <ll.h>
#include <flirc.h>
#include <prjutil.h>
#include <cmds.h>
#include <logging.h>

#include <timelib.h>

static inline int
enough_args(int arguments, int amount_expected)
{
	if (!((arguments) >= amount_expected)) {
		logerror("Not Enough Arguments\n");
		return -1;
	}

	return 0;
}

CMDHANDLER(waet)
{
	fl_wait_for_device(0x20A0, "flirc.tv");

	return argc;
}

APPCMD(wait, &waet,
		"Waits for the device to be plugged in (used for scripting)",
		"usage: \n"
		"  flirc wait",
		NULL);

CMDHANDLER(record)
{
	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help record", NULL);
		return argc;
	}

	printf(" Press any button on the remote to link it with '%s'\n\n",
			argv[0]);

	if (fl_set_record(argv[0], 100) == EOK)
		printf("  Succesfully recorded button\n\n");
	else
		printf("Error, button exists\n");

	return argc;
}

APPCMD(record, &record,
		"Record infrared buttons and link them to HID keys",
		"usage: \n"
		"  record '<keyboard key>'\n"
		"examples:		\n"
		"  flirc record a   \n"
		"  flirc record space\n"
		"valid commands:\n"
		"  a-z <any single letter>\n"
		"  return, enter, escape, backspace, delete, tab, \n"
		"  space, F[1-12], printscreen, scroll, pause, insert, \n"
		"  home, pageup, pagedown, end, right, left, down, up, \n"
		"Media Keys: \n"
		"  wake, eject, vol_up, vol_down, mute, play/pause, stop \n"
		"  rewind, fast_forward, next_track, prev_track \n"
		"System Keys: \n"
		"  suspend\n",
		NULL);

CMDHANDLER(record_api)
{
	if (enough_args(argc, 2) < 0) {
		run_cmd_line("help record_api", NULL);
		return argc;
	}

	printf("hit a key on the remote to be paired with %d %d\n",
			atoi(argv[0]), atoi(argv[1]));

	if (fl_set_record_api(atoi(argv[0]), atoi(argv[1]), 100) == EOK)
		printf("  Succesfully recorded button\n\n");

	return argc;
}

APPCMD(record_api, &record_api,
	"Advanced button recording",
	"Send the raw HID value down to flirc to be linked with button recorded"
	"\nusage:\n"
	"  record_api 'arg1 arg2'   arg1 is key-modifier \n"
	"                           arg2 is HID key\n"
	"example:\n"
	"  flirc record_api 136 4   '136' represents right cmd + left cmd\n"
	"                           '4' represents 'a' in HID\n"
	"\n"
	"Key modifiers are defined in the IEEE HID Spec as follows:\n"
	"LEFT  CONTROL          1\n"
	"LEFT  SHIFT            2\n"
	"LEFT  ALT              4\n"
	"LEFT  CMD|WIN          8\n"
	"RIGHT CONTROL          16\n"
	"RIGHT SHIFT            32\n"
	"RIGHT ALT              64\n"
	"RIGHT CMD|WIN          128\n"
	"\n"
	"To record Control + Shift, logically or 1 & 2 to make 3",
	NULL)

CMDHANDLER(delete_index)
{
	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help delete_index", NULL);
		return argc;
	}

	fl_delete_index(atoi(argv[0]));

	return 1;
}

APPCMD(delete_index, &delete_index,
		"Delete button at index displayed in `flirc_util settings`",
		"usage: \n"
		"  delete_index <index> \n"
		"example: \n"
		"  flirc delete_index 0",
		NULL);


CMDHANDLER(delete)
{
	printf(" The Next button pressed will be deleted,"
		" please press a button\n\n");

	if (fl_set_delete(WAIT) == EOK)
		printf("  Button Deleted Successfully\n\n");

	return argc;
}

APPCMD(delete, &delete,
		"Delete next remote button flirc sees from saved database",
		"usage: \n"
		"  delete \n"
		"example: \n"
		"  flirc delete",
		NULL);

CMDHANDLER(normal)
{
	if (fl_set_normal() == EOK)
		printf("Succesfully set device to normal operation\n");

	return argc;
}

APPCMD(normal, &normal,
		"Put flirc in normal user mode",
		"usage: \n"
		"  normal \n"
		"example: \n"
		"  flirc normal",
		NULL);

CMDHANDLER(saveconfig)
{
	char file_name[300];

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help saveconfig", NULL);
		return argc;
	}

	sprintf(file_name, "%s.fcfg", argv[0]);

	printf("\nSaving Configuration File '%s' to Disk\n", file_name);

	if (fl_save_config(file_name) == EOK)
		printf("\n\nConfiguration File saved\n\n");

	return argc;
}

APPCMD(saveconfig, &saveconfig,
		"Save configuration file to disk",
		"usage: \n"
		"  saveconfig '<filname>'\n"
		"example:\n"
		"  flirc saveconfig ~/Desktop/boxee",
		NULL);

CMDHANDLER(loadconfig)
{
	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help loadconfig", NULL);
		return argc;
	}

	printf("\nLoading Configuration File '%s' to Device\n", argv[0]);

	if (fl_load_config(argv[0]) == EOK)
		printf("\n\nConfiguration File Loaded Successfully\n\n");

	return argc;
}

APPCMD(loadconfig, &loadconfig,
		"Load configuration file from disk to flirc",
		"usage: \n"
		"  loadconfig '<filename>'\n"
		"example:\n"
		"  flirc loadconfig ~/Desktop/boxee.fcfg",
		NULL);

CMDHANDLER(format)
{
	printf("Formatting Device, please wait...");
	fflush(stdout);

	if (fl_format_config() == EOK)
		printf("  Done!\n");

	return 1;
}

APPCMD(format, &format,
		"Remove all saved buttons from flirc",
		"usage: \n"
		"  format \n"
		"example: \n"
		"  flirc format",
		NULL);

CMDHANDLER(dfu)
{
	int result;

	if (argc < 1) {
		printf("Putting device in Firmware Upgrade Mode...");
		fflush(stdout);
		result = fl_device_set_dfu();

		if (result < EOK) {
			printf("\nError\n");
		} else {
			printf("  Done!\n");
		}

		return argc;
	}

	if (strcmp(argv[0], "leave") == 0) {
		printf("Kicking device out of bootloader...");
		fflush(stdout);
		result = fl_leave_bootloader();

		if (result < EOK)
			printf("Error, could not leave bootloader\n");
		else
			printf("Done!\n");

	} else {
		printf("Error: invalid dfu option\n");
		run_cmd_line("help dfu", NULL);
	}

	return argc;
}

APPCMD(dfu, &dfu,
		"Kick in or out of Device Firmware Upgrade mode",
		"usage: \n"
		"  dfu\n"
		"example: \n"
		"  flirc dfu \n"
		"  flirc dfu --leave",
		NULL);

CMDHANDLER(upgrade)
{
	const char *imageLocation = NULL;
	int VID = 0;
	const char *manufacturer = NULL;

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help upgrade", NULL);
		return argc;
	}

	if (argc == 1) {
		imageLocation = argv[0];
		VID = 0x20A0;
		manufacturer = "flirc.tv";

		printf("Uploading image: %s to \"%s\" 0x%X\n",
			imageLocation,
			manufacturer,
			VID);

		/* Send image to device */
		if (fl_upgrade_fw(imageLocation, VID, manufacturer,
					NULL, NULL) < 0) {
			printf("Error, upload failed\n");
		}

		return argc;

	} else if (argc == 3) {
		imageLocation = argv[0];
		manufacturer = argv[1];
		VID = strtol(argv[2], NULL, 16);

		printf("Uploading image: %s to \"%s\" 0x%X\n",
			imageLocation,
			manufacturer,
			VID);

		if (fl_upgrade_fw(imageLocation, VID, manufacturer,
					NULL, NULL) < 0) {

			fl_leave_bootloader();
		}

		return argc;
	}

	return argc;
}

APPCMD(upgrade, &upgrade,
		"Uploads new firmware image to flirc hardware",
		"usage: \n"
		"  flirc upgrade <path to firmware file>"
		"example:\n"
		"  flirc loadconfig ~/Desktop/flirc.bin",
		NULL);

CMDHANDLER(noise_canceler)
{
	int enabled;

	if (argc < 1) {
		if ((enabled = fl_get_noise_cancel()) >= 0) {
			printf("  noise canceler:   %s\n", enabled ?
				"Enabled" : "Disabled");
		} else {
			printf("  noise canceler:   NA\n");
		}
		return 0;
	}

	if (strcmp(argv[0], "enable") == 0) {
		printf("enabling noise canceler\n");
		fl_set_noise_cancel(1);
		return 2;
	} else if (strcmp(argv[0], "en") == 0) {
		printf("enabling noise canceler\n");
		fl_set_noise_cancel(1);
		return 2;
	} else if (strcmp(argv[0], "disable") == 0) {
		printf("enabling noise canceler\n");
		fl_set_noise_cancel(0);
		return 2;
	} else if (strcmp(argv[0], "dis") == 0) {
		printf("enabling noise canceler\n");
		fl_set_noise_cancel(0);
		return 2;
	}

	return 1;
}

APPCMD(noise_canceler, &noise_canceler,
		"Noise canceler to prevent phantom presses",
		"usage: enable|disable",
		NULL);

CMDHANDLER(builtin_profiles)
{
	int enabled;

	if (argc < 1) {
		if ((enabled = fl_get_builtin_profile_support()) >= 0) {
			printf("  builtin profiles: %s\n", enabled ?
				"Enable" : "Disable");
		} else {
			printf("  builtin profiles: NA\n");
		}
		return 0;
	}

	if (strcmp(argv[0], "enable") == 0) {
		printf("enabling built in profiles\n");
		fl_set_builtin_profile_support(1);
		return 1;
	} else if (strcmp(argv[0], "en") == 0) {
		printf("enabling built in profiles\n");
		fl_set_builtin_profile_support(1);
		return 1;
	} else if (strcmp(argv[0], "disable") == 0) {
		printf("disabling built in profiles\n");
		fl_set_builtin_profile_support(0);
		return 1;
	} else if (strcmp(argv[0], "dis") == 0) {
		printf("disabling built in profiles\n");
		fl_set_builtin_profile_support(0);
		return 1;
	}
	return 1;
}

APPCMD(profiles, &builtin_profiles,
		"enable or disable built in profiles",
		"usage: enable|disable (no args get the current setting)",
		NULL);

CMDHANDLER(interkey_delay)
{
	int enabled;

	if (argc < 1) {
		if ((enabled = fl_get_interkey_delay()) >= 0) {
			printf("  inter-key delay:  %d\n", enabled);
		} else {
			printf("  inter-key delay:  NA\n");
		}

		return 0;
	}

	if (fl_set_interkey_delay(atoi(argv[0])) == EOK)
		printf("interkey delay set: %d\n", atoi(argv[0]));


	return 1;
}

APPCMD(interkey_delay, &interkey_delay,
		"set the interkey delay",
		"This adjusts the sensitivity of botton pressing. If keys\n"
		"feel sticky, try decreasing this. If keys are to responsive,\n"
		"try increasing this. A sweet spot is 3\n"
		"usage:\n"
		"  interkey_delay 0-6\n"
		"examples:\n"
		"  interkey_delay 0\n"
		"  interkey_delay 3\n",
		NULL);


CMDHANDLER(seq_modifiers)
{
	int enabled;

	if (argc < 1) {
		if ((enabled = fl_get_sequence_modifiers()) >= 0) {
			printf("  seq. modifiers:   %s\n", enabled ?
				"Enabled" : "Disabled");
		} else {
			printf("  seq. modifiers:   NA\n");
		}

		return 0;
	}

	if (strcmp(argv[0], "enable") == 0) {
		printf("enabling sequencing modifiers\n");
		fl_set_sequence_modifiers(1);
		return 2;
	} else if (strcmp(argv[0], "en") == 0) {
		printf("enabling sequencing modifiers\n");
		fl_set_sequence_modifiers(1);
		return 2;
	} else if (strcmp(argv[0], "disable") == 0) {
		printf("disabling sequencing modifiers\n");
		fl_set_sequence_modifiers(0);
		return 2;
	} else if (strcmp(argv[0], "dis") == 0) {
		printf("disabling sequencing modifiers\n");
		fl_set_sequence_modifiers(0);
		return 2;
	} else {
		printf("unrecognized option\n");
		run_cmd_line("help sequence_modifiers", NULL);
	}


	return argc;
}

APPCMD(seq_modifiers, &seq_modifiers,
		"enable or disable sequencing the modifiers",
		"Enable this for windows media center\n"
		"usage: seq_modifiers enable|disable\n"
		"example: \n"
		"  seq_modifiers enable\n"
		"  seq_modifiers disable",
		NULL);

CMDHANDLER(status)
{
	/* useless, need a parser for last record status for correct version */
//	printf("Current record status: %d\n", fl_set_interrupt(0));
	printf("Last upgrade attempt: %s\n", fl_last_upgrade_attempt() ?
			"Error" : "OK");
	return 0;
}

APPCMD(status, &status,
		"Last Flirc Status",
		"Last Flirc Status",
		NULL);

CMDHANDLER(peek)
{
	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help peek", NULL);
		return argc;
	}

	printf("Reading:\n");
	printf("    Address: %X Data: %X\n", atoi(argv[0]),
			fl_eeprom_peek(atoi(argv[0])));

	return 1;
}

APPCMD(peek, &peek,
		"Peek EEPROM address",
		"usage: \n"
		"  peek 'location'\n"
		"example: \n"
		"  peek 5\n"
		"  Address: 0x05 Data: 0x05",
		NULL);

CMDHANDLER(space)
{
	float remain;
	float recorded;

	if (fl_fw_state() != FIRMWARE) {
		printf("  Memory Info:      NA\n");
		return 0;
	}

	if ((recorded = fl_keys_recorded()) < 0) {
		printf("  Memory Info:      NA\n");
		return 0;
	}

	if ((remain = fl_keys_remaining()) < 0) {
		printf("  Memory Info:      NA\n");
		return 0;
	}

	printf("\n");
	printf("Memory Info:\n");
	printf("  keys recorded:  %d\n", fl_keys_recorded());
	printf("  keys remaining: %d\n", fl_keys_remaining());
	printf("  memory used:  %3.0f%%\n", 100*(recorded / remain));

	return argc;
}

APPCMD(space, &space,
		"Displays information about the space used and remaining",
		"usage: \n"
		"  space\n",
		NULL);

CMDHANDLER(keys)
{
	if (fl_display_config() < 0) {
		printf("  Records:          NA\n");
	}
	return argc;
}

APPCMD(keys, &keys,
		"Shows the recorded remote keys and their pairings",
		"usage: \n"
		"  keys\n",
		NULL);

CMDHANDLER(dump)
{
	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help dump", NULL);
		return argc;
	}

	printf("Reading:\n");
	int read_data;
	int i;

	printf("%04X  ", 0);
	for (i = 0; i < atoi(argv[0]) + 1; i++) {
		if (i%16 == 0 && i != 0)
			printf("\n%04X  ", i);
		if (i%8 == 0)
			printf(" ");
		read_data = fl_eeprom_peek(i);

		if (read_data < 0)
			goto exit;

		printf("%02X ", fl_eeprom_peek(i));
	}
	printf("\n");


	return argc;
exit:
	printf("Error\n");
	return argc;
}

APPCMD(dump, &dump,
		"Dumps contents of eeprom to console",
		"usage: \n"
		"  dump 'size'\n"
		"example: \n"
		"  flirc dump 100",
		NULL);

CMDHANDLER(sleep_detect)
{
	int enabled;

	if (argc < 1) {
		if ((enabled = fl_get_sleep_detect()) >= 0) {
			printf("  sleep detection:  %s\n", enabled ?
				"Enabled" : "Disabled");
		} else {
			printf("  sleep detection:  NA\n");
		}

		return 0;
	}

	if (strcmp(argv[0], "enable") == 0) {
		printf("enabling sleep/suspend detection\n");
		fl_set_sleep_detect(1);
		return 2;
	} else if (strcmp(argv[0], "en") == 0) {
		printf("enabling sleep/suspend detection\n");
		fl_set_sleep_detect(1);
		return 2;
	} else if (strcmp(argv[0], "disable") == 0) {
		printf("disabling sleep/suspend detection\n");
		fl_set_sleep_detect(0);
		return 2;
	} else if (strcmp(argv[0], "dis") == 0) {
		printf("disabling sleep/suspend detection\n");
		fl_set_sleep_detect(0);
		return 2;
	} else {
		printf("unrecognized option\n");
		run_cmd_line("help suspend_detect", NULL);
	}


	return argc;
}

APPCMD(sleep_detect, &sleep_detect,
		"Turns on sleep/suspend detection",
		"usage: \n"
		"  sleep_detect enable|disable\n"
		"example: \n"
		"  sleep_detect enable\n"
		"  sleep_detect disable",
		NULL);

static void print_pid(void)
{
	int pid = fl_product_type();

	if (pid == FIRMWARE_FLIRC_AMAZON) {
		printf("  variant:          Amazon\n");
	} else if (pid == FIRMWARE_FLIRC) {
		printf("  variant:          Flirc\n");
	} else {
		printf("  variant:          Unknown\n");
	}
}

CMDHANDLER(settings)
{
	run_cmd_line("version", NULL);
	printf("\n");
	printf("Settings:\n");
	run_cmd_line("sleep_detect", NULL);
	run_cmd_line("noise_canceler", NULL);
	run_cmd_line("interkey_delay", NULL);
	run_cmd_line("seq_modifiers", NULL);
	print_pid();
	run_cmd_line("profiles", NULL);
	run_cmd_line("space", NULL);
	run_cmd_line("sku", NULL);
	run_cmd_line("keys", NULL);
	return argc;
}

APPCMD(settings, &settings,
		"Displays all the devices current settings",
		"usage: \n"
		"  settings\n",
		NULL);

CMDHANDLER(reboot_flirc)
{
	printf("Rebooting flirc...");
	fl_reboot();
	printf("done\n");
	return argc;
}

APPCMD(reboot, &reboot_flirc,
		"Displays all the devices current settings",
		"usage: \n"
		"  flirc_util reboot\n",
		NULL);

static void dump_log(void)
{
	char buf[2048];

	while (fl_log(buf) > 0) {
		printf("%s",buf);
	}
}


CMDHANDLER(normalize_cfg)
{
	fl_normalize_config();
	return argc;
}

APPCMD(normalize_cfg, &normalize_cfg,
		"Normalize configuration on device",
		"This will go through the configuration and fix interkey\n"
		"If a device is too sensitive, try this. If you recorded\n"
		"more than one remote onto flirc, this command will not work\n"
		"usage: \n"
		"  normalize_cfg \n",
		NULL);


CMDHANDLER(device_log)
{
	struct timeval s;
	gettimeofday(&s, NULL);
	uint8_t persist = 0;


	if (argc > 0) {
		if (strcmp(argv[0], "persistance") == 0) {
			persist = 1;
		}
	}

	do {
		gettimeofday(&s, NULL);
		dump_log();
		while (!time_has_elapsed_us(&s, 100));
	} while (persist);

	return argc;
}

APPCMD(device_log, &device_log,
		"Displays the log on the device",
		"usage: \n"
		"  device_log <persistance>\n"
		"  - persistance mode optional\n"
		"  - this command clears the log on the device\n",
		NULL);
#if 0
CMDHANDLER(rom_table)
{
	uint8_t table_id;
	uint8_t enable;

	if (argc < 2) {
		printf("Possible Tables:\n");
		printf("   firetv\n");
		printf("   kodi\n");
		printf("   streacom\n");
		run_cmd_line("help rom_table", NULL);
		return 0;
	}

	if (strcmp(argv[0], "firetv") == 0) {
		table_id = 0;
	} else if (strcmp(argv[0], "kodi") == 0) {
		table_id = 1;
	} else if (strcmp(argv[0], "streacom") == 0) {
		table_id = 2;
	} else {
		printf("invalid argument\n");
		run_cmd_line("help rom_table", NULL);
		return 2;
	}

	if (strcmp(argv[1], "en") == 0) {
		enable = 1;
	} else if (strcmp(argv[1], "dis") == 0) {
		enable = 0;
	} else {
		printf("invalid argument\n");
		run_cmd_line("help rom_table", NULL);
		return 2;
	}

	fl_enable_rom_table(atoi(argv[0]), atoi(argv[1]));
	return argc;
}

APPCMD(rom_table, &rom_table,
		"enable or disable a give rom table",
		"usage: \n"
		"  rom_table <name> <en/dis>\n"
		"possible tables: \n"
		"  firetv\n"
		"  kodi\n"
		"  streacom\n",
		NULL);
#endif
