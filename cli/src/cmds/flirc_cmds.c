/**
 * COPYRIGHT 2023 Flirc, Inc. All rights reserved.
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

#include <flirc/flirc.h>
#include <cmds.h>
#include <logging.h>

#include <timelib.h>

static inline int enough_args(int arguments, int amount_expected)
{
	if (arguments < amount_expected) {
		logerror("Not Enough Arguments\n");
		return -1;
	}

	return 0;
}

CMDHANDLER(version)
{
	int state;
	uint8_t major;
	uint8_t minor;
	int patch;
	uint32_t fw_scm_hash;
	const char *dirty;
	const char *branch;
	const char *config;
	const char *skuu;


	printf("flirc_util version %s [%s]\n",
			SCMVER,
			SCMVERSION);

	if ((state = fl_fw_state()) < 0)  {
		return argc;
	}

	major = fl_major_version();
	minor = fl_minor_version();
	patch = fl_patch_version();

	dirty = fl_fw_dirty();
	branch = fl_fw_branch();
	config = fl_fw_config();

	if ((dirty = fl_fw_dirty()) == NULL) {
		dirty = "";
	}

	if (patch < 0) {
		/* print patch if we have hardware that supports it */
		patch = 0;
	}

	fw_scm_hash = fl_fw_scm_hash();

	if (state == FIRMWARE) {
		printf(" Firmware Detected\n");
	} else if (state == BOOTLOADER) {
		printf(" Booloader Detected\n");
	}

	printf(" FW Version: v%d.%d.%d%s\n", major, minor, patch, dirty);

	if ((skuu = fl_get_sku()) != NULL) {
		printf("    SKU:     %s\n", skuu);
	}

	if ((branch = fl_fw_branch()) != NULL) {
		printf("    Branch:  %s\n", branch);
	}

	if ((config = fl_fw_config()) != NULL) {
		printf("    Config:  %s\n", config);
	}

	if ((fw_scm_hash = fl_fw_scm_hash()) > 0) {
		printf("    Hash:    0x%08X\n", fw_scm_hash);
	}

	return 0;
}

APPCMD(version, &version,
		"Print the application version and device version if connected",
		"usage: version [history]\n"
		" history     shows the history log",
		NULL);

CMDHANDLER(waet)
{
	fl_wait_for_device(0x20A0, "flirc.tv");

	return 0;
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
		return 0;
	}

	printf(" Press any button on the remote to link it with '%s'\n\n",
			argv[0]);

	if (strcmp(argv[0], "toggle") == 0) {

		if (fl_major_version() >= 4 && fl_minor_version() >= 6) {
			if (fl_record_toggle(100) == EOK) {
				printf("  Succesfully recorded button\n\n");
			} else {
				printf("Error, button exists\n");
			}
		} else {
			printf("  Supported on gen2, v4.6 or higher\n");
		}

		return 1;
	}


	if (fl_set_record(argv[0], 100) == EOK)
		printf("  Succesfully recorded button\n\n");
	else
		printf("Error, button exists\n");

	return 1;
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
		"  space, F[1-12], print_screen, scroll, pause, insert, \n"
		"  home, pageup, pagedown, end, right, left, down, up, \n"
		"Media Keys: \n"
		"  wake, eject, vol_up, vol_down, mute, play/pause, stop \n"
		"  rewind, fast_forward, next_track, prev_track \n"
		"System Keys: \n"
		"  suspend\n"
		"Other: \n"
		"  toggle [enables/disables] flirc\n",
		NULL);

CMDHANDLER(record_lp)
{
	int rq;

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help record_lp", NULL);
		return argc;
	}

	printf(" Press any button on the remote to link it with '%s'\n\n",
			argv[0]);

	rq = fl_set_record_lp(argv[0], 100);

	switch (rq) {
	case (-ERR_KEY_NOT_FOUND):
		printf("Error: You must first record this remote button with"
				" the regular flirc_util record command.\n"
				"Example:\n"
				" flirc_util record `<first function>`\n"
				" flirc_util record_lp %s\n", argv[0]);
		break;
	case (-ERR_BUTTON_EXISTS):
		printf("Error: Long press already assigned to that button\n");
		break;
	case (-EWRONGDEV):
		printf("Error: Current device does not support this feature\n");
		break;
	case (EOK):
		printf("  Succesfully recorded button\n\n");
		break;
	default:
		printf("Error: Unable to pair key\n");
		break;
	}

	return argc;
}

APPCMD(record_lp, &record_lp,
"Record a long pres key",
"\n"
"This will record a secondary function to an already recorded remote control \n"
"button. Once a button is converted to a long press button, a short press of \n"
"that button will send out the first function, and holding down the button \n"
"for half a second will send out the second function.\n"
"\n"
"You will need to record a regular key with the 'flirc_util record' "
"command first.\n"
"The examples below assume you are pressing the same button on your remote.\n"
"\n"
"Example 1; Send out 'up' arrrow on short press, 'vol up' when held down for\n"
"half a second: \n"
"  flirc_util record up\n"
"  flirc_util record_lp vol_up\n"
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

CMDHANDLER(record_macro)
{
	int rq;

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help record_macro", NULL);
		return argc;
	}

	printf(" Press any button on the remote to link it with '%s'\n\n",
			argv[0]);

	rq = fl_set_record_macro(argv[0], 100);

	switch (rq) {
	case (-ERR_KEY_NOT_FOUND):
		printf("Error: You must first record this remote button with"
				" the regular flirc_util record command.\n"
				"Example:\n"
				" flirc_util record `<first function>`\n"
				" flirc_util record_macro %s\n", argv[0]);
		break;
	case (-EWRONGDEV):
		printf("Error: Current device does not support this feature\n");
		break;
	case (EOK):
		printf("  Succesfully recorded button\n\n");
		break;
	default:
		printf("Error: Unable to pair key\n");
		break;
	}

	return 1;
}

APPCMD(record_macro, &record_macro,
"Record a macro key",
"\n"
"This will record a secondary function to an already recorded remote control \n"
"button. Keep using this command to keep adding functions to the same remote \n"
"control button. For example, to send out a series of commands on a single \n"
"button press, first use the flirc_util record command with the first key \n"
"then convert the button to a macro with this command. You can also combine \n"
"long presses and macros together by first converting a key to a long \n"
"press, and then recording macros. See the below for examples:\n"
"Example 1; Create a macro that sounds out the text 'hello':\n"
"  flirc_util record h\n"
"  flirc_util record_macro e\n"
"  flirc_util record_macro l\n"
"  flirc_util record_macro l\n"
"  flirc_util record_macro o\n"
"Example 2; Create a long press. Sends out 'up arrow' on short press, 'hello'\n"
"if held down for half a second:\n"
"  flirc_util record h\n"
"  flirc_util record_lp e\n"
"  flirc_util record_macro l\n"
"  flirc_util record_macro l\n"
"  flirc_util record_macro o\n"
"  a-z <any single letter>\n"
"  return, enter, escape, backspace, delete, tab, \n"
"  space, F[1-12], print_screen, scroll, pause, insert, \n"
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

	if (strtol(argv[0], NULL, 16) == 2) {
		printf("Hit any remote button to be paired with Consumer Usage "
				"Table, Key 0x%X\n",
				(int)strtol(argv[1], NULL, 16));
		if (fl_set_record_api_new(
				strtol(argv[0], NULL, 16),
				strtol(argv[1], NULL, 16) & 0xFF,
				strtol(argv[1], NULL, 16) >> 8,
				100) == EOK) {
			printf("  Succesfully recorded button\n\n");
			return 2;
		}
	}

	if (argc > 2) {
		printf("Press any remote button to be paired with key: "
				"%d %d %d\n", atoi(argv[0]), atoi(argv[1]),
				atoi(argv[2]));

		if (fl_set_record_api_new(atoi(argv[0]), atoi(argv[1]),
					atoi(argv[2]), 100) == EOK) {
			printf("  Succesfully recorded button\n\n");
			return 3;
		}
	}

	printf("hit a key on the remote to be paired with %d %d\n",
			atoi(argv[0]), atoi(argv[1]));

	if (fl_set_record_api(RM_NORMAL,
				atoi(argv[0]), atoi(argv[1]), 100) == EOK)
		printf("  Succesfully recorded button\n\n");

	return 2;
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
	int index;

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help delete_index", NULL);
		return argc;
	}

	index = atoi(argv[0]);

	printf("Before Index Deletion\n");

	if (fl_display_config() < 0)
		log_err("failed to load configuration\n");

	fl_delete_index(index);

	fflush(stdout);
	printf("\n\n");

	printf("After Index Deletion\n");

	if (fl_display_config() < 0)
		log_err("failed to save configuration\n");

	printf("\n");

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

	return 0;
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

	return 0;
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
#ifdef CONFIG_DEBUG
	struct timeval s;
#endif
	char file_name[300];

	if (enough_args(argc, 1) < 0) {
		run_cmd_line("help saveconfig", NULL);
		return argc;
	}

	sprintf(file_name, "%s.fcfg", argv[0]);

	printf("\nSaving Configuration File '%s' to Disk\n", file_name);

#ifdef CONFIG_DEBUG
	gettimeofday(&s, NULL);
#endif
	if (fl_save_config(file_name) == EOK) {
		printf("\n\nConfiguration File saved\n\n");
#ifdef CONFIG_DEBUG
		printf("\n\nTook %d ms\n\n",
				(int)time_elapsed_us(&s)/(1000));
#endif
	}

	return 1;
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

	return 1;
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

		/* Delay to fix race condition and close devices */
		delay_ms(500);
		fl_close_device();
		/* Uploaded Firmware, now wait for device */
		if (fl_wait_for_device(0x20A0, "flirc.tv") == FIRMWARE) {
			fl_set_boot_flag();
			printf("[DEVICE]        EOK\n");
			return 1;
		} else {
			printf("[DEVICE]        Failed\n");
			return 1;
		}
	} else if (strcmp(argv[0], "flag") == 0) {
		printf("clearing boot flag\n");
		fl_set_boot_flag();
		return 1;
	} else {
		printf("%s\n", argv[0]);
		printf("Error: invalid dfu option\n");
		run_cmd_line("help dfu", NULL);
		return 1;
	}
}

APPCMD(dfu, &dfu,
		"Kick in or out of Device Firmware Upgrade mode",
		"usage: \n"
		"  dfu\n"
		"example: \n"
		"  flirc dfu \n"
		"  flirc dfu leave",
		NULL);

CMDHANDLER(upgrade)
{
	const char *imageLocation = NULL;
	int VID = 0;
	const char *manufacturer = NULL;
	int timeout = -1; // optional timeout

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

		return 1;

	} else if (argc == 3) {
		if (strcmp(argv[1], "timeout") == 0) {
			timeout = atoi(argv[2]);

			imageLocation = argv[0];
			VID = 0x20A0;
			manufacturer = "flirc.tv";
		} else {
			imageLocation = argv[0];
			manufacturer = argv[1];
			VID = strtol(argv[2], NULL, 16);
		}

		printf("Uploading image: %s to \"%s\" 0x%X\n",
			imageLocation,
			manufacturer,
			VID);

		if (fl_upgrade_fw_timeout(imageLocation, VID, manufacturer,
					NULL, NULL, timeout) < 0) {

			fl_leave_bootloader();
		}

		return 3;
	}

	return argc;
}

APPCMD(upgrade, &upgrade,
		"Uploads new firmware image to flirc hardware",
		"usage: \n"
		"  flirc upgrade <path to firmware file> [timeout <seconds>]\n"
		"example:\n"
		"  flirc upgrade ~/Desktop/flirc.bin\n"
		"  flirc upgrade ~/Desktop/flirc.bin timeout 3",
		NULL);

CMDHANDLER(noise_canceler)
{
	int enabled;

	if (fl_major_version() >= 4) {
		printf("  noise canceler:   %s\n", "always enabled");
		return argc;
	}

	if (argc < 1) {
		if ((enabled = fl_get_noise_cancel()) >= 0) {
			printf("  noise canceler:    %s\n", enabled ?
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

	if (fl_major_version() >= 4) {
		printf("  inter-key delay:  %s\n", "N/A for current firmware");
		return argc;
	}

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

CMDHANDLER(sleep_detect)
{
	int enabled;

	if (fl_major_version() >= 4) {
		printf("  sleep detection:  %s\n", "always enabled");
		return argc;
	}

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

static void show_space(void)
{
	float remain;
	float recorded;

	if (fl_fw_state() != FIRMWARE) {
		printf("  Memory Info:      NA\n");
		return;
	}

	if ((recorded = fl_keys_recorded()) < 0) {
		printf("  Memory Info:      NA\n");
		return;
	}

	if ((remain = fl_keys_remaining()) < 0) {
		printf("  Memory Info:      NA\n");
		return;
	}

	printf("\n");
	printf("Memory Info:\n");
	printf("  keys recorded:  %d\n", fl_keys_recorded());
	printf("  keys remaining: %d\n", fl_keys_remaining());
	printf("  memory used:  %3.0f%%\n", 100*(recorded / remain));
}

static void show_sku(void)
{
	char *skuu;
	if ((skuu = fl_get_sku()) == NULL) {
		printf("  product sku:      NA\n");
	} else {
		printf("  product sku:      %s\n", skuu);
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
	print_pid();
	run_cmd_line("profiles", NULL);

	show_space();
	show_sku();

	if (fl_display_config() < 0) {
		printf("  Records:          NA\n");
	}

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

CMDHANDLER(mode)
{
	if (argc < 1) {
		printf("not enough args\n");
		printf("argc:%d\n", argc);
	}

	if (dict_has_key(opts, "iospirit")) {
		if (strcmp(argv[0], "en") == 0) {
			printf("enabling iospirit, disabling hid\n");
			fl_usb_iface_en(IOSPIRIT, 1);
		} else if (strcmp(argv[0], "dis") == 0) {
			fl_usb_iface_en(IOSPIRIT, 0);
			printf("disabling iospirit, enabling hid\n");
		}
	} else {
		printf("Error, button exists\n");
	}

	return argc;
}

START_CMD_OPTS(mode_opts)
	CMD_OPT(iospirit, 'i', "iospirit", "enable/disable iospirit interface")
END_CMD_OPTS;

APPCMD_OPT(mode, &mode,
		"Enable or disable a usb mode",
		"usage: \n"
		"  flirc_util mode --iospirit <en/dis>\n"
		"  - enabling iospirit interface, disables generic interface",
		NULL, mode_opts);

CMDHANDLER(device_log)
{
	struct timeval s;
	gettimeofday(&s, NULL);
	uint8_t persist = 0;

	if (dict_has_key(opts, "ir")) {
		printf("Note - Enabling IR debugging\n");
		fl_set_debug_pipe(0, 1);
	} else {
		/* disable debug logging */
		fl_set_debug_pipe(0, 0);
	}

	if (dict_has_key(opts, "persistance")) {
		printf("Note - Entering persistance mode\n");
		printf("Press Ctrl+C to exit\n");
		persist = 1;
	}

	do {
		gettimeofday(&s, NULL);
		dump_log();
		while (!time_has_elapsed_us(&s, 100));
	} while (persist);

	return argc;
}

START_CMD_OPTS(device_log_opts)
	CMD_OPT(ir, 'i', "ir", "enable ir debugging")
	CMD_OPT(persistance, 'p', "persistance", "keep polling device")
END_CMD_OPTS;

APPCMD_OPT(device_log, &device_log,
		"Displays the log on the device",
		"usage: \n"
		"  device_log <persistance>\n"
		"  - persistance mode optional\n"
		"  - this command clears the log on the device\n",
		NULL, device_log_opts);

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

	fl_enable_rom_table(table_id, enable);
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

CMDHANDLER(unit_test)
{
	if (fl_major_version() < 4) {
		printf("Self test not supported\n");
	}

	if (fl_format_config() < 0) {
		printf("Flirc Fucked\n");
	}

	if (fl_unit_test() == EOK) {
		printf("Flirc Okay\n");
	} else {
		printf("Flirc Not Okay\n");
	}

	if (fl_format_config() < 0) {
		printf("Flirc Not Okay\n");
	}

	return 0;
}

APPCMD(unit_test, &unit_test,
		"Performs a self test",
		"Options:\n"
		"  none\n",
		NULL);
