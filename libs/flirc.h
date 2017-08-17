/**
 * COPYRIGHT 2014 Flirc, Inc. All rights reserved.
 *
 * This copyright notice is Copyright Management Information under 17 USC 1202
 * and is included to protect this work and deter copyright infringement.
 * Removal or alteration of this Copyright Management Information without
 * the express written permission of Flirc, Inc. is prohibited, and any
 * such unauthorized removal or alteration will be a violation of federal law.
 *
 * @file
 * @brief   Flirc Library functions
 */

#include <stdint.h>
#include <stdio.h>
#include <ll.h>

#ifndef I__FLIRC_H__
	#define I__FLIRC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cpluspuls */

#ifdef BUILD_DLL
#define _DLL __declspec(dllexport)
#else
#define _DLL
#endif

#define BOOTLOADER 			1
#define FIRMWARE_FLIRC 			2
#define FIRMWARE 			2 	/* compatibility */
#define FIRMWARE_FLIRC_AMAZON		3
#define FL_UNKNOWN_DEV			4
#define MAX_TIMEOUT			100

struct fw_ver_hist {
	/* verison string */
	const char *v;
	/* summary string */
	const char *s;
	/* pointer to list node */
	struct list_head node;
};

/**
 * fl_open_device() searches for and opens flirc.
 *
 * @param VID  - USB Vendor ID
 * @param *mf  - manufacture string
 *
 * @return      BOOTLOADER   - Bootloader found and opend
 * @return      FIRMWARE     - Succesfully opened firmware
 * @return	-UNKNOWN_DEV - Device found, but unknown	
 * @return      -ENXIO       - No such device or address
 */
int _DLL fl_open_device(unsigned int VID, const char *mf);

/**
 * fl_open_device() searches for and opens flirc.
 *
 * @param VID  - USB Vendor ID
 * @param *mf  - manufacture string
 *
 * @return      BOOTLOADER
 * @return      FIRMWARE_FLIRC
 * @return      FIRMWARE_FLIRC_AMAZON
 * @return      -ENXIO       - No such device or address
 */
int _DLL fl_product_type(void);

/**
 * fl_close_device() closes flirc. Must be called before the program exits.
 *                   Erroneous behavior will be seen if this isn't called.
 *
 * This function requires no arguments.
 *
 * @return      None.
 */
void _DLL fl_close_device(void);

/**
 * fl_set_boot_flag() set bootflag so we don't stay in bootloader.
 *
 * This function requires no arguments.
 *
 * @return	EOK           - No errors
 * @return	-ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int _DLL fl_set_boot_flag(void);

/**
 * fl_device_set_dfu() puts the device into firmware upgrade mode. The product
 * 		       ID changes so the device will need to be close and
 * 		       re-opened.
 *
 * This function requires no arguments.
 *
 * @return	EOK           - No errors
 * @return	-ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int _DLL fl_device_set_dfu(void);

/**
 * fl_last_upgrade_attempt() gets the status of the last upgrade.
 *
 * This function requires no arguments.
 *
 * @return	EOK           - No errors
 * @return	ECRC          - CRC error
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int _DLL fl_last_upgrade_attempt(void);

/**
 * fl_version() returns the hardcoded firmware version of either the bootloader
 *              or main firmware
 *
 * This function requires no arguments.
 *
 * @return      VERSION       - Version number
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
float _DLL fl_version(void);

/**
 * fl_major_version() returns the mojor version of the firmware.
 *
 * This function requires no arguments.
 *
 * @return      VERSION       - Version number
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int _DLL fl_major_version(void);

/**
 * fl_minor_version() returns the minor version of the firmware.
 *
 * This function requires no arguments.
 *
 * @return      VERSION       - Version number
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int _DLL fl_minor_version(void);

/**
 * fl_patch_version() returns the patch version of the firmware.
 *
 * This function requires no arguments.
 *
 * @return      VERSION       - Version number
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int _DLL fl_patch_version(void);

/**
 * fl_version_str() returns the firmware version of either the bootloader
 *                  or the main firmware in string format
 *
 * This function requires no arguments.
 *
 * @return      VERSION       - Version number
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
char _DLL *fl_version_str(void);

/**
 * fl_fw_scm_hash() returns the hardcoded firmware git hash of the bootloader
 * or the main firmware image, if available.
 *
 * This function requires no arguments.
 *
 * @return      VERSION       - Version number
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
uint32_t _DLL fl_fw_scm_hash(void);

/**
 * fl_leave_bootloader() forces the bootloader to jump to the main image.
 * 			 Normally, when a firmware image is uploaded, the device
 * 			 will jump to the newly upgraded image automatically,
 * 			 however, should an image not be available, or DFU was
 * 			 entered accidentally, this function can be used to
 * 			 return the firmware to the normal functional state.
 *
 * This function requires no arguments.
 *
 * @return      EOK	      - Success
 * @return      -ENODEV       - Flirc not present
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int _DLL fl_leave_bootloader(void);

/**
 * fl_reboot() sends a command down that issues a reset. This is the
 *             equivalent as unplugging and plugging in the device. No data
 *             is lost with this command and this command should never need
 *             to be called.
 *
 * This function requires no arguments.
 *
 * @return      EOK	      - Success
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - Bad mode selected.
 */
int _DLL fl_reboot(void);

/**
 * fl_wait_for_device() waits for flirc to be present in the system, and then
 * 			opens the device.
 *
 * @param VID    - USB Vendor ID
 * @param char * - manufacture string
 *
 * @return      BOOTLOADER   - Bootloader found and opend
 * @return      FIRMWARE     - Succesfully opened firmware
 * @return      -ENXIO       - No such device or address
 */
int _DLL fl_wait_for_device(unsigned int VID, const char *manufacturer);

/**
 * fl_upgrade_fw() all in one upgrader function. Will wait for a device to be
 *                 present, if present, this will automatically set the device
 *                 in DFU mode (if not alraedy), wait for the device to
 *                 reappear and upload the file sent to the function.
 *
 * @param user_file - location of firmware image
 * @param VID	    - USB Vendor ID
 * @param *man	    - manufacturer string
 * @param *priv	    - container for C++ class, progress bar
 * @param *p_bar    - function pointer to progress bar
 *
 * @return      EOK	      - Success
 * @return      -EUPGRADE_FAIL - Upgrade failed
 */
int _DLL fl_upgrade_fw(const char *user_file, unsigned int VID,
		const char *manufacturer, void *priv,
		void (*p_bar)(int perc, void *priv));

/**
 * fl_upgrade_fw_ext() all in one upgrader function. Uses a buffer instead of a
 *                     file, but otherwise, identical to above.
 *
 * @param buff 	 - image buffer
 * @param VID  	 - USB Vendor ID
 * @param *man 	 - manufacturer string
 * @param *priv	 - container for C++ class, progress bar
 * @param *p_bar - function pointer to progress bar
 *
 * @return      EOK	      - Success
 * @return      -EUPGRADE_FAIL - Upgrade failed
 */
int _DLL fl_upgrade_fw_ext(unsigned char *buf, size_t len, int VID,
		const char *manufacturer, void *priv,
		void (*p_bar)(int perc, void *priv));

/**
 * fl_set_delete() puts flirc into delete mode. When calling this function, the
 * 		   next remote button that flirc 'see's' will be deleted from
 * 		   the flirc. If the button is not found in flirc's memory,
 * 		   flirc will ignore the request and go back into normal
 * 		   operation. If the button is found in memory, flirc will
 * 		   delete the button, and go back into normal operation.
 *
 * @param WAIT 	    - specify blocking functionality
 * @param DONT_WAIT - specify non-blocking functionality
 *
 * @return      EOK		- Completed Succesfully
 * @return      -ENODEV		- Flirc not present
 * @return      -EWRONGDEV	- Unsupported, device in bootloader
 * @return      -KEY_NOT_FOUND	- Delete did not succeed, button must not exist
 * @return      -LIBUSBERR	- LIBUSB error code
 */
int _DLL fl_set_delete(int wait);

/**
 * fl_set_record() puts flirc into record mode. The next button pressed will be
 *                 saved in flirc and linked with the keyboard key user_input.
 *                 This function can be passed either a character or a word. Any
 *                 character will be translated into the USB HID equivalent
 *                 value automatically before being sent down to flirc.
 *
 * @param key  - Either a character or string.
 * @param wait - Timeout in seconds. If zero is given (DONT_WAIT), the function
 * 		 becomes non-blocking, and will not put the device back in normal
 * 		 playback mode.
 *
 * Example Keys:
 * 			Example:  'h'
 *
 * Example Key Strings:
 *  			escape, return, enter, backspace, delete, tab,
 *  			space, F[1-12], printscreen, scroll, pause, insert
 *  			home, pageup, pagedown, end, right, left, down, up, wake
 *
 * @return      EOK		   - Completed Succesfully
 * @return      -ETIMEOUT          - Time out reached
 * @return      -ENODEV		   - Flirc not present
 * @return      -EWRONGDEV	   - Unsupported, device in bootloader
 * @return      -ERR_NO_SPACE	   - No space is left on the device
 * @return	-ERR_BUTTON_EXISTS - Button already exists
 * @return      -LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_record(const char *key, int wait);

/**
 * fl_set_record_api() this function calls the same USB command as setRecord,
 * 		       but there is no translation layer. For example, in both
 * 		       examples below, lower case 'a' will be paired with the
 * 		       next remote button seen by flirc:
 *
 * setRecord("a", WAIT); vs fl_set_record_api(0, 0x4, WAIT); or
 * fl_set_record_api(0x, HID_a, WAIT);
 *
 * 0x04 is the HID value for a. HID_a is defined for you in the included
 * translate.h file.
 *
 * The modifier is also defined in the translate.h file. To pass multiple
 * modifiers, 'or' them together. Example:
 *
 *  fl_set_record_api((MOD_CTRL_LEFT | MOD_ALT_LEFT), HID_a, WATI);
 *
 * @param mod  - USB Modifier (1 << X)
 * @param key  - unsigned number representing HID value
 * @param wait - Timeout in seconds. If zero is given (DONT_WAIT), the function
 * 		 becomes non-blocking, and will not put the device back in normal
 * 		 playback mode.
 *
 * @return      EOK		   - Completed Succesfully
 * @return      -ETIMEOUT          - Time out reached
 * @return      -ENODEV		   - Flirc not present
 * @return      -EWRONGDEV	   - Unsupported, device in bootloader
 * @return	-ERR_NO_SPACE	   - Record Failed, no space left
 * @return	-ERR_BUTTON_EXISTS - Record Failed, button exists
 * @return      -LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_record_api(int modifier, int key, int wait);

/**
 * fl_set_normal() Puts flirc into normal mode. In normal operation, flirc sends
 *                 keyboard keys to the PC when recorded infrared signals are
 *                 seen. Can be called to take the device out of record mode.
 *
 * This function requires no arguments.
 *
 * @return      EOK		   - Completed Succesfully
 * @return      -ENODEV		   - Flirc not present
 * @return      -EWRONGDEV	   - Unsupported, device in bootloader
 * @return      -LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_normal(void);

/**
 * fl_set_interrupt() Waits for an interrupt from flirc to be created. An
 * 		      interrupt is only generated after recording or deleting a
 * 		      button.
 *
 * @param - timeout in seconds. If zero is given (DONT_WAIT), the function
 *          becomes non-blocking, and will not put the device back in normal
 *          playback mode.
 *
 * @return	 EOK		   - Successfully (add or delete)
 * @return	-ENODEV		   - Flirc not present
 * @return	-EWRONGDEV	   - Unsupported, device in bootloader
 * @return	-LIBUSBERR	   - LIBUSB error code
 * @return	-ERR_NO_SPACE	   - EEPROM out of space
 * @return	-ERR_BUTTON_EXISTS - Botton already exists
 * @return	-ERR_KEY_NOT_FOUND - Can't Delete, key not found
 */
int _DLL fl_set_interrupt(int timeout);

/**
 * fl_delete_buttons() deletes multiple buttons if they exist from the device.
 *
 * @param  key	- the key to remove
 *
 * @return	Count		   - Number of buttons deleted
 * @return	-ENODEV		   - Flirc not present
 * @return	-EWRONGDEV	   - Unsupported, device in bootloader
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_delete_buttons(const char *user_input);

/**
 * fl_set_noise_cancel() enables the noise canceler. This is used when phantom
 *                       keys are detected on flirc.
 *
 * @param	1		   - Enable Suspend Detection
 * @param	0		   - Disable Suspend Detection
 *
 * @return	-EINVAL		   - Invalid value
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_noise_cancel(int val);

/**
 * fl_get_noise_cancel() get currently state of noise cancel
 *
 * This function requires no arguments.
 *
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_get_noise_cancel(void);

/**
 * fl_set_sensitivity() Set's the sensitivity of flirc. Changing the sensitivity
 *                      will require all buttons to be re-recorded. This
 *                      function is only for advanced users and has not been
 *                      brought to the GUI yet.
 *
 * @param  val	- sensitivity, 0-3
 *
 * @return	-EINVAL		   - Invalid value
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_sensitivity(int val);

/**
 * fl_get_sensitivity() get currently set sensitivity.
 *
 * This function requires no arguments.
 *
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_get_sensitivity(void);

/**
 * fl_set_interkey_delay() Sets the interkey delay. This will help with button
 *                         repeat issues. This function is only for advanced
 *                         users.
 *
 * @param  val - 0-7 (translates to ms delay in firmware)
 *
 * @return	-EINVAL		   - Invalid value
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_interkey_delay(int val);

/**
 * fl_get_interkey_delay() gets the interkey delay.
 *
 * This function requires no arguments.
 *
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_get_interkey_delay(void);

/**
 * fl_enable_rom_table() Enable or disables a particular rom table
 *
 * @param	table		   - 0 - FireTV
 *				   - 1 - Kodi
 *				   - 2 - Streacom (Microsoft WMCE)
 *				   - 3 - Flirc WMCE
 *				   - 4 - NVIDIA Shield
 *
 * @param	en		   - 0 - Disable
 *				   - 1 - Enable
 *
 * @return	-EINVAL		   - Invalid value
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_enable_rom_table(uint32_t table, uint8_t en);

/**
 * fl_get_rom_table() gets the passed table state
 *
 * This function requires no arguments.
 *
 * @return      0		   - Table is Disabled
 * @return      1		   - Table is Enabled
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_get_rom_table(uint32_t table);

/**
 * fl_set_sequence_modifiers() enable or disable sequencing modifiers and keys
 * This should be enabled for windows media center users.
 *
 * @param  val - 0:1 (disable/enable)
 *
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_sequence_modifiers(int val);

/**
 * fl_get_sequence_modifiers() gets the current state of the sequence modifier
 * and keys settings.
 *
 * This function requires no arguments.
 *
 * @return	0:1		   (disable/enable)
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_get_sequence_modifiers(void);

/**
 * fl_set_builtin_profile_support() enable or disable built in profile support
 * like the harmony xbmc profile. If disabled, the keys can be remapped at the
 * users will.
 *
 * @param  val - 0:1 (Disable:Enable)
 *
 * @return	-EINVAL		   - Invalid value
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_builtin_profile_support(int val);

/**
 * fl_get_builtin_profile_support() gets the setting for builtin profile support
 *
 * This function requires no arguments.
 *
 * @return	1: Feature enabled
 * @return	0: Feature disabled
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_get_builtin_profile_support(void);

/**
 * fl_get_sleep_detect() returns a value if suspend is enabled
 *
 * This function requires no arguments.
 *
 * @return	1		   - Suspend is enabled
 * @return	0		   - Suspend is not enabled
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_get_sleep_detect(void);

/**
 * fl_set_sleep_detect() sets the interkey delay.
 *
 * @param	1		   - Enable Suspend Detection
 * @param	0		   - Disable Suspend Detection
 *
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_sleep_detect(int val);

/**
 * fl_fw_state() gets the state of flirc
 *
 * This function requires no arguments.
 *
 * @return	BOOTLOADER
 * @return	FIRMWARE
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_fw_state(void);

/**
 * fl_eeprom_peek() returns data in given address
 *
 * @param  add	- offset in the EEPROM
 *
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_eeprom_peek(int address);

/**
 * fl_eeprom_poke() pokes the non volatile eeprom
 *
 * @param  add	- offset in the EEPROM
 * @param  data	- data to put in offset
 *
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_eeprom_poke(uint16_t address, uint16_t data);

/**
 * fl_get_state() returns the status of the flirc state machine.
 *
 * This function requires no arguments.
 *
 * @return	-ENODEV		- Flirc not present
 * @return	-LIBUSBERR	- LIBUSB error code
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 */
int _DLL fl_get_state(void);

/**
 * fl_clear_state() resets the state machine, this must be called after
 * 		    recording button.
 *
 * @return	-ENODEV		- Flirc not present
 * @return	-LIBUSBER	- LIBUSB error code
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 */
int _DLL fl_clear_state(void);

/**
 * fl_display_config() resets the state machine, this must be called after
 * 		       recording a button.
 *
 * @return	-ENODEV		- Flirc not present
 * @return	-LIBUSBER	- LIBUSB error code
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 * @return      -ENOMEM     	- Malloc fail
 */
int fl_display_config(void);

/**
 * fl_format_config() clears all saved configuration data in the device. All
 * 		      recorded buttons will be erased.
 *
 * This function requires no arguments.
 *
 * @return      EOK		   - Completed Succesfully
 * @return      -ENODEV		   - Flirc not present
 * @return      -EWRONGDEV	   - Unsupported, device in bootloader
 * @return      -LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_format_config(void);

/**
 * fl_save_config() this function downloads the configuration file from a flirc
 *                  device and saves it to the location specified by the user.
 *                  This is a raw binary file in a proprietary format.
 *
 * @param file - location to save the file
 *
 * @return      EOK	    - Completed Succesfully
 * @return      -ENODEV     - Flirc not present
 * @return      -EWRONGDEV  - Unsupported, device in bootloader
 * @return      -EFAULT     - Invalid file address
 * @return      -EBADF      - File could not be created/opened
 * @return      -ENOMEM     - Malloc fail
 * @return      -LIBUSBERR  - LIBUSB error code
 */
int _DLL fl_save_config(const char *user_file);

/**
 * fl_load_config() uploads a .fcfg file to a flirc device that was previously
 * 		    saved with fl_save_config().
 *
 * @param file - location of the file to load
 *
 * @return      EOK	    - Completed Succesfully
 * @return      -ENODEV     - Flirc not present
 * @return      -EWRONGDEV  - Unsupported, device in bootloader
 * @return      -EFAULT     - Invalid file address
 * @return      -EBADF      - File could not be created/opened
 * @return      -ENOMEM     - Malloc fail
 * @return      -LIBUSBERR  - LIBUSB error code
 */
int _DLL fl_load_config(const char *user_file);

/**
 * fl_keys_recorded() gets the used space, in keys
 *
 * @return	space_used	- space free on device
 * @return	-ENODEV		- Flirc not present
 * @return	-LIBUSBER	- LIBUSB error code
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 * @return      -ENOMEM     	- Malloc fail
 */
int fl_keys_recorded(void);

/**
 * fl_keys_remaining() gets the space free, in keys
 *
 * @return	space_free	- space free on device
 * @return	-ENODEV		- Flirc not present
 * @return	-LIBUSBER	- LIBUSB error code
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 * @return      -ENOMEM     	- Malloc fail
 */
int fl_keys_remaining(void);

/**
 * fl_delete_index() specify the index of the key to delete
 *
 * @param index - index to delete.
 *
 * @return	-EINVAL		   - Invalid value
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
int fl_delete_index(int index);

/**
 * fl_libusb_logging() enable/disable libusb logging
 *
 * @param index - 1 enable, 0 - disable
 */
void fl_libusb_logging(int enable);

/**
 * fl_log() get usb event log
 *
 * @param buf - empty buf pointer, that will be filled
 */
int fl_log(char *buf);

/**
 * fl_get_sku() parses header and gets the sku so you don't have to
 *
 * This function requries no arguments.
 *
 * @return	-NULL		- Probably no device
 * @return	string		- 100 bytes of header
 */
char * _DLL fl_get_sku(void);

/**
 * fl_print_ver_diff() prints a log of the history given a comparison string
 * assuming this string is a valid semantic string
 *
 * @param comp - string to compare against firmware version
 */
int fl_print_ver_diff(const char *comp);

/**
 * fl_get_ver_hist() gets a list head of log information in the form
 * of fw_ver_hist.
 *
 * @param comp - string to compare against firmware version
 */
struct list_head *fl_get_ver_hist(const char *comp);

/**
 * fl_set_debug_pipe() enables a specific log pipe
 *
 * @param index - 0 RAW_IR
 * @param en    - 1 Enable, 0 Disable
 *
 * @return	EOK
 */
int _DLL fl_set_debug_pipe(uint32_t pipe, uint8_t en);

/**
 * fl_get_debug_pipe() enables a specific log pipe
 *
 * @param 	- none
 *
 * @return	32 bit value (1<<RAW_IR) etc
 * @return	-1 error
 */
int _DLL fl_get_debug_pipe(uint32_t pipe);

/**
 * fl_normalize_config() tries to figure out the interkey delay
 * of the configuration and normalize all recorded keys. Don't
 * use this if you have multiple remotes. Only supported on gen2 hardware.
 * enables a specific log pipe
 *
 * @param 	- none
 *
 * @return	EOK
 * @return	-1 error
 */
int _DLL fl_normalize_config(void);

/**
 * fl_transmit_raw() sends a raw packet. Only supported on flirc gen2 hardware
 *
 * @param 	buf 	data to transmit, please be responsible, numbers are in
 * 			micro seconds
 * @param	len	length of buffer, don't send more than 100 bits, must be
 * 			even number of bits (edges).
 *
 * @return	EOK
 * @return	-1 error
 */
int _DLL fl_transmit_raw(uint16_t *buf, uint16_t len);

enum sensitivity {
	low_sens=0,
	med_sens=1,
	def_sens=2,
	hig_sens=3,
};

/** \ingroup desc
 * Erorr codes.
 */

#undef ENOMEM
#undef EBADF
#undef EFAULT
#undef EINVAL
#undef ENODEV
#undef ENOSYS
#undef ECANCELED
#undef EWRONGDEV
#undef EIDXRANGE
#undef ENXIO

/* Can't Record, no space */
#ifndef FUNK_SUCCESS
#define FUNK_SUCCESS		3
#endif

/* No space left on device */
#ifndef ERR_NO_SPACE
#define ERR_NO_SPACE		4
#endif

/* Can't Record, button exists */
#ifndef ERR_BUTTON_EXISTS
#define ERR_BUTTON_EXISTS	5
#endif

/* Can't Delete, button not found */
#ifndef ERR_KEY_NOT_FOUND
#define ERR_KEY_NOT_FOUND	6
#endif

typedef enum error_t{
        EOK = 0,
	STATE_PLAYBACK = 0,
	STATE_RECORD = 1,
	STATE_DELETE = 2,
	NO_INTERRUPT=4,		/** Returned from setRecord */
        ENOMEM,   		/** Out of memory */
	EBADF,	  		/** Bad File Descriptor */
        EFAULT,   		/** Bad Address */
        EINVAL,   		/** Invalid argument */
	ENODEV,			/** No Device */
        ENOSYS,   		/** Function not implemented */
        ECANCELED,		/** Operation Canceled */
	EWRONGDEV,		/** Wrong Device */
	EUPGRADE_FAILED,	/** Upgrade Failed */
        EIDXRANGE,		/** Index Out Of Range */
	ENXIO, 			/** No such device or address */
	LIBUSBERR,         	/** LIBUSB Error Code */
	ETIMEOUT,		/** Error, time out */
	_ERROR_T_COUNT
} error_type;

/** \ingroup desc
 * Parameter symbols
 */
typedef enum flirc_parms_t{
	WAIT=1,			/* Used to send to setRecord */
	DONT_WAIT=0,		/* Used to send to setRecord */
} flirc_parms_t;

/**
 * strerr() error code to string
 *
 * @param  err  - error code
 *
 * @return	err_string - const string
 */
const char _DLL *strerr(int err);

#if (defined(WIN32) || defined(WINDOWS) || defined(__WINDOWS__))

#include <windows.h>
#define delay_ms(msec) Sleep(msec)

#else
#include <unistd.h>
#define delay_ms(msec) usleep(msec * 1000)

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* I__FLIRC_H__ */
