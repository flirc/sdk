/**
 * COPYRIGHT 2019 Flirc, Inc. All rights reserved.
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

#include <stddef.h>
#include <stdint.h>
#include <ll.h>

#ifndef I__FLIRC_H__
	#define I__FLIRC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cpluspuls */

#ifdef BUILD_DLL
#ifdef _DLL
#undef _DLL
#endif
#define _DLL __declspec(dllexport)
#else
#define _DLL
#endif

#define LIBFLIRC_VERSION		"3.27.7"
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
	ll_t node;
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
 * fl_open_device_alt() searches for and opens flirc's alternate
 * interface used for ir raw capturing.
 *
 * @param VID  - USB Vendor ID
 * @param *mf  - manufacture string
 *
 * @return      BOOTLOADER   - Bootloader found and opend
 * @return      FIRMWARE     - Succesfully opened firmware
 * @return	-UNKNOWN_DEV - Device found, but unknown
 * @return      -ENXIO       - No such device or address
 */
int _DLL fl_open_device_alt(unsigned int VID, const char *mf);

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
 * fl_fw_branch() returns the firmware build branch
 *
 * This function requires no arguments.
 *
 * @return      char *        - build branch
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
char _DLL *fl_fw_branch(void);

/**
 * fl_fw_config() returns the firmware configuration string
 *
 * This function requires no arguments.
 *
 * @return      char *        - build configuration
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
char _DLL *fl_fw_config(void);

/**
 * fl_fw_dirty() returns a string that represents the extended version dirty tag
 *
 * This function requires no arguments.
 *
 * @return      char *        - tag string
 * @return      -ENODEV       - Flirc not present
 * @return	-EWRONGDEV    - Unsupported, device in bootloader
 * @return	-LIBUSB_ERROR - bad mode selected.
 */
char _DLL *fl_fw_dirty(void);

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
 * fl_wait_for_device_timout() waits for flirc to be present in the system, and then
 * 			opens the device. Will fail after timeout has passed.
 * 			Otherwise, is identical to above.
 *
 * @param VID     - USB Vendor ID
 * @param char *  - manufacture string
 * @param timeout - timeout in seconds
 *
 * @return      BOOTLOADER   - Bootloader found and opend
 * @return      FIRMWARE     - Succesfully opened firmware
 * @return      -ENXIO       - No such device or address
 * @return      -ETIMEOUT    - Time out reached
 */
int _DLL fl_wait_for_device_timeout(unsigned int VID,
		const char *manufacturer, int timeout);

/**
 * fl_upgrade_fw() all in one upgrader function. Will wait for a device to be
 *                 present, if present, this will automatically set the device
 *                 in DFU mode (if not alraedy), wait for the device to
 *                 reappear and upload the file sent to the function.
 *
 * @param user_file - location of firmware image
 * @param VID       - USB Vendor ID
 * @param *man      - manufacturer string
 * @param *priv     - container for C++ class, progress bar
 * @param *p_bar    - function pointer to progress bar
 *
 * @return      EOK	      - Success
 * @return      -EUPGRADE_FAIL - Upgrade failed
 */
int _DLL fl_upgrade_fw(const char *user_file, unsigned int VID,
		const char *manufacturer, void *priv,
		void (*p_bar)(int perc, void *priv));

/**
 * fl_upgrade_fw_timeout() all in one upgrader function. When waiting for device
 *                 to appear, will fail after timeout has passed. Otherwise, is
 *                 identical to above.
 *
 * @param user_file - location of firmware image
 * @param VID       - USB Vendor ID
 * @param *man      - manufacturer string
 * @param *priv     - container for C++ class, progress bar
 * @param *p_bar    - function pointer to progress bar
 * @param timeout   - timeout in seconds
 *
 * @return      EOK	      - Success
 * @return      -EUPGRADE_FAIL - Upgrade failed
 */
int _DLL fl_upgrade_fw_timeout(const char *user_file, unsigned int VID,
		const char *manufacturer, void *priv,
		void (*p_bar)(int perc, void *priv), int timeout);

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
 * fl_upgrade_fw_ext_timeout() all in one upgrader function. When waiting for
 *                 device to appear, will fail after timeout has passed.
 *                 Otherwise, is identical to above.
 *
 * @param buff    - image buffer
 * @param VID     - USB Vendor ID
 * @param *man    - manufacturer string
 * @param *priv   - container for C++ class, progress bar
 * @param *p_bar  - function pointer to progress bar
 * @param timeout - timeout in seconds
 *
 * @return      EOK	      - Success
 * @return      -EUPGRADE_FAIL - Upgrade failed
 */
int _DLL fl_upgrade_fw_ext_timeout(unsigned char *buf, size_t len, int VID,
		const char *manufacturer, void *priv,
		void (*p_bar)(int perc, void *priv), int timeout);

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

#define RM_NORMAL	0
#define RM_MACRO	1
#define RM_LONG_PRESS	2
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
int _DLL fl_set_record_api(int mode, int modifier, int key, int wait);

/**
 * fl_set_record_api_new() this function calls the same USB command as
 * setRecord. Raw record api for new version of the hardware. No table
 * translation.
 *
 * @param rep  - Report ID
 * @param mod  - KEY0 - Usually Modifier
 * @param key  - KEY1 -
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
int _DLL fl_set_record_api_new(int report_id, int modifier, int key, int wait);

/**
 * fl_set_record_lp() will assign a second hid function to the same remote
 * control button press. The second key will be sent out after half a second
 * of holding down the button.
 *
 * @param key  - Either a character or string.
 * @param wait - Timeout in seconds. If zero is given (DONT_WAIT), the function
 * 		 becomes non-blocking, and will not put the device back in
 * 		 normal playback mode.
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
 * @return      -ERR_KEY_NOT_FOUND - First record the key using the std record
 * @return      -LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_record_lp(const char *key, int wait);

/**
 * fl_set_record_macro() will record another hid key to the same remote control
 * button as a macro. All keys assigned to a single remote control press
 * will be sent out sequentially.
 *
 * @param key  - Either a character or string.
 * @param wait - Timeout in seconds. If zero is given (DONT_WAIT), the function
 * 		 becomes non-blocking, and will not put the device back in
 * 		 normal playback mode.
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
 * @return      -ERR_KEY_NOT_FOUND - First record the key using the std record
 * @return      -LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_set_record_macro(const char *key, int wait);

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
 * fl_record_toggle() will record the toggle button. Will allow flirc to be
 * disabled or enabled by recorded key.
 *
 * @param wait - Timeout in seconds. If zero is given (DONT_WAIT), the function
 * 		 becomes non-blocking, and will not put the device back in
 * 		 normal playback mode.
 *
 *
 * @return      EOK		   - Completed Succesfully
 * @return      -ETIMEOUT          - Time out reached
 * @return      -ENODEV		   - Flirc not present
 * @return      -EWRONGDEV	   - Unsupported, device in bootloader
 * @return      -ERR_NO_SPACE	   - No space is left on the device
 * @return      -ERR_KEY_NOT_FOUND - First record the key using the std record
 * @return      -LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_record_toggle(int wait);

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

enum sensitivity {
	low_sens=0,
	med_sens=1,
	def_sens=2,
	hig_sens=3,
};

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
 * @param	ik	delay to use inbetween packets. On firmware < 4.10.0,
 *			use microseconds, on >= 4.10.0, use ms. Will
 *			automatically detect number less than 1000 as ms and
 *			convert accordingly for lder devices. Similarly, any
 *			number greater than 1000 will be converted to ms for
 *			newer devices. An error will be displayed in both cases.
 *			convert
 * 			Defaults to 40ms if to low.
 * @param	repeat	repeat count, generally should be 3, 0 is infinit until
 * 			stopped, currently not supported.
 *
 * @return	EOK
 * @return	-1 error
 */
int _DLL fl_transmit_raw(uint16_t *buf, uint16_t len, uint16_t ik, uint8_t repeat);

/**
 * flirc_send_pronto() transmit a pronto encoded buffer. Only supported on gen2
 * hardware.
 *
 * @param	buf	Pointer to the buffer holding the Pronto codes.
 * @param	length 	The number of elements in the buffer.
 * @param	rep  The number of times to repeat the transmission.
 * 
 * @return An integer representing the status of the operation. A value 
 * of zero  indicates success, while non-zero values represent 
 * specific error codes.
 */
int _DLL flirc_send_pronto(uint16_t *buf, uint32_t length, uint8_t rep);

/**
 * fl_unit_test() perform a closed loop test
 *
 * @param 	- none
 *
 * @return	EOK
 * @return	-1 error
 */
int _DLL fl_unit_test(void);

/**
 * fl_last_hash() grab the last hash seen, doesn't have to be in the database
 *
 * @param 	- none
 *
 * @return	hash		- 32 bit unique button hash
 * @return	-ENODEV		- Flirc not present
 * @return	-LIBUSBERR	- LIBUSB error code
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 */
int _DLL fl_last_hash(void);

typedef enum {
    IOSPIRIT = 0,
} usb_iface_type;

/**
 * fl_usb_iface_en() enable or disable a specific usb interface
 *
 * @param 	usb_iface_type	- See enums below
 * @param	en		- enable or disable
 *
 * @return	EOK
 * @return	-ENODEV		   - Flirc not present
 * @return	-LIBUSBERR	   - LIBUSB error code
 */
int _DLL fl_usb_iface_en(usb_iface_type type, uint8_t en);

/**
 * struct ir_packet - Represents a single captured IR packet.
 *
 * @param buf      - Timing buffer containing the timings (in microseconds) 
 *                   of the captured IR signal.
 * @param len      - The actual number of entries populated in 'buf'. 
 * @param elapsed  - Time (in microseconds) elapsed since the last edge 
 *                   was captured.
 */
struct ir_packet {
	uint16_t buf[256];
	uint16_t len;
	uint16_t elapsed;
};

/**
 * fl_ir_packet_poll() - Polls a flirc USB device for a complete IR packet.
 *
 * This function checks if the flirc USB device has a complete IR packet ready.
 * If a frame is available, the 'ir' structure is filled with timing data and 
 * relevant details. Useful for real-time systems or applications that need to 
 * regularly check for IR input.
 *
 * @param *ir      - Pointer to the ir_packet structure where details of the 
 *                   captured IR signal will be stored.
 *
 * @return         - Returns 0 if no frame is available yet.
 *                   Returns 1 if a frame is ready and 'ir' has been populated.
 */
int _DLL fl_ir_packet_poll(struct ir_packet *ir);

#if 0
#include <ir/ir.h>
/**
 * fl_transmit_prot() - Transmits an IR signal based on the provided protocol 
 *                      and scancode.
 *
 * This function initiates the transmission of an IR signal corresponding to 
 * the provided protocol and scancode. The signal can be repeated a specified 
 * number of times.
 *
 * @param protocol   - The IR protocol to use for the transmission. Defined 
 *                     as an enumeration of type rc_proto elsewhere.
 * @param scancode   - The value or scancode to be transmitted.
 * @param repeat     - The number of times the signal should repeat. If set to 
 *                     zero, the function will use the protocol's default repeat 
 *                     count, typically once.
 *
 * @return           - An integer indicating the success or failure of the 
 *                     transmission. 0 for success and -1 for failure.
 */
int fl_transmit_prot(enum rc_proto protocol, uint32_t scancode, uint8_t repeat);
#endif

/**
 * fl_ir_transmit_kill() will stop the transmitter from transmission
 *
 * @param 	none
 *
 * @return	EOK
 * @return	-ENODEV		   - Flirc not present
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 */
int _DLL fl_ir_transmit_kill(void);

/**
 * fl_rb_settings_set() settings for remote buddy configuration. To enable
 * remote wakeup, bit0 in wake_en must be cleared. For an apple remote to
 * wake the machine, the wake_en feature must be enabled. The high_add and
 * low_add are apple remote address filters. These address are used to determine
 * which apple remote can send the remote wake command. For example, if low_add
 * is set to 0, and high_add is set to 1, then apple remotes with addresses 0
 * and 1 will wake up the computer. If high_add is equal to 0, and low_add is
 * equal to 0, then all buttons from apple remote address 0 will wake up the
 * machine.
 *
 * Flirc can wake the machine from a single button within an apple remote as
 * well. In order to use this feature, use the api xxxx which allows the
 * specificiation of any IR signal, which is not limited to apple remotes.
 *
 * If the timing of a button is specified, this will override the address
 * filter. To remove the specified button, see the xxxx api.
 *
 * @param 	wake_en		- enable or disable, Bit: 0
 * @param	high_add	- Apple Remote ADDR high
 * @param	low_add 	- Apple Remote ADDR low
 *
 * @return	EOK
 * @return	-ENODEV		- Flirc not present
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 */
int _DLL fl_rb_settings_set(uint32_t wake_en,
		uint8_t high_add, uint8_t low_add);

/**
 * fl_rb_settings_get() grab the settings on the device. Variables passed
 * by reference.
 *
 * @param 	wake_en		- enable or disable, Bit: 0
 * @param	high_add	- Apple Remote ADDR high
 * @param	low_add 	- Apple Remote ADDR low
 *
 * @return	EOK
 * @return	-ENODEV		- Flirc not present
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 */
int _DLL fl_rb_settings_get(uint32_t *wake_en,
		uint8_t *high_add, uint8_t *low_add);

/**
 * fl_rb_wake_sig() send a buffer to the device, which is the raw timing
 * waveform of the signal which will be used to wake the computer. To remove
 * a saved key, send any buffer with length equal to 1, which is invalid.
 *
 * @param 	buf 	data to transmit, please be responsible, numbers are in
 * 			micro seconds
 * @param	len	length of buffer, don't send more than 100 bits, must be
 * 			even number of bits (edges).
 *
 * @return	EOK
 * @return	-ENODEV		- Flirc not present
 * @return	-EWRONGDEV	- Unsupported, device in bootloader
 */
int _DLL fl_rb_wake_sig(uint16_t *buf, uint8_t len);


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

typedef enum fl_types_t {
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
