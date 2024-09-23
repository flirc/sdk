/**
 * COPYRIGHT 2024 Flirc, Inc. All rights reserved.
 *
 * This copyright notice is Copyright Management Information under 17 USC 1202
 * and is included to protect this work and deter copyright infringement.
 * Removal or alteration of this Copyright Management Information without
 * the express written permission of Flirc, Inc. is prohibited, and any
 * such unauthorized removal or alteration will be a violation of federal law.
 */

#include <stdint.h>
#include <stddef.h>

#ifdef FL_VERSION_HISTORY
#include <ll.h>
#endif

#ifndef I__FLIRC_H__
#define I__FLIRC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cpluspuls */

#ifdef BUILD_DLL
  #ifdef _DLL
    #undef _DLL
    #define _DLL __declspec(dllexport)
  #else
    #define _DLL __declspec(dllexport)
  #endif
#else
  #ifdef _DLL
    #undef _DLL
    #define _DLL
  #else
    #define _DLL
  #endif
#endif

#define BOOTLOADER 			1
#define FIRMWARE_FLIRC 			2
#define FIRMWARE 			2 	/* compatibility */
#define FIRMWARE_FLIRC_AMAZON		3
#define MAX_TIMEOUT			100

#ifdef FL_VERSION_HISTORY
struct fw_ver_hist {
	/* verison string */
	const char *v;
	/* summary string */
	const char *s;
	/* pointer to list node */
	ll_t node;
};
#endif

/**
 * fl_lib_version() - Retrieves the version of the Flirc library.
 *
 * This function returns a string representing the current version of the
 * Flirc library. It can be used to display the library version or for
 * logging and diagnostic purposes.
 *
 * @return          - Constant character pointer to the string containing
 *                    the library version. The memory for this string is 
 *                    managed internally and should not be freed or modified.
 */
const char _DLL *fl_lib_version(void);

/**
 * fl_open_device() searches for and opens flirc.
 *
 * @param VID  - USB Vendor ID
 * @param *mf  - manufacture string
 *
 * @return      BOOTLOADER   - Bootloader found and opend
 * @return      FIRMWARE     - Succesfully opened firmware
 * @return	-UNKNOWN_DEV - Device found, but unknown
 * @return      -ENXIO       - No such device or address (stderr.h)
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
 * @return      -ENXIO       - No such device or address (stderr.h)
 */
int _DLL fl_open_device_alt(unsigned int VID, const char *mf);

/**
 * fl_product_type() - Fetches the product type of a Flirc device.
 *
 * This is a legacy function for supporting Amazon SKU related products. No
 * parameters are required. The product type can be: BOOTLOADER,
 * FIRMWARE_FLIRC, or FIRMWARE_FLIRC_AMAZON.
 *
 * @return      BOOTLOADER            - If the device is in bootloader mode.
 * @return      FIRMWARE_FLIRC        - If the device is a standard Flirc.
 * @return      FIRMWARE_FLIRC_AMAZON - If the device is an Amazon Flirc.
 * @return      -ENXIO                - No such device or address (stderr.h).
 */
int _DLL fl_product_type(void);

/**
 * fl_close_device() ensures the safe termination of flirc's connection. This
 * function must be executed before the program concludes its execution.
 * Failure to do so may result in unpredictable behavior.
 *
 * This function does not require any arguments.
 *
 * @return      None.
 */
void _DLL fl_close_device(void);

/**
 * fl_set_boot_flag() sets a boot flag to prevent indefinite bootloader stay.
 *
 * This function is a safety mechanism which is automatically invoked when
 * running fl_upgrade_fw_timeout or its variants. After a device upgrade, the
 * device jumps to and runs the loaded image. If the device restarts or reboots,
 * instead of running the loaded application image, it could stay in the
 * bootloader indefinitely.
 *
 * For instance, if the loaded image is stuck in a while(1) loop without a
 * watchdog, the device could be stuck indefinitely and become unrecoverable.
 * After the upgrade, once a built-in self test is run, and assuming USB
 * communication is functional, this function can be invoked to set a non-volatile
 * flag which allows the normal boot process to proceed.
 *
 * This function requires no arguments.
 *
 * @return      EOK          - No errors
 * @return      -ENODEV      - Flirc not present
 * @return      -EWRONGDEV   - Unsupported, device in bootloader
 * @return      -LIBUSB_ERROR- libusb specific error.
 */
int _DLL fl_set_boot_flag(void);

/**
 * fl_device_set_dfu() transitions the device to firmware upgrade mode. This
 * alters the product ID, necessitating a call to fl_close_device before
 * fl_open_device() can be invoked again. Failing to do so may lead to memory
 * leaks or undefined behavior.
 *
 * This function does not require any arguments.
 *
 * @return EOK           - No errors
 * @return -ENODEV       - Flirc not present
 * @return -EWRONGDEV    - Unsupported, device in bootloader
 * @return -LIBUSB_ERROR - libusb specific error.
 */
int _DLL fl_device_set_dfu(void);

/**
 * fl_last_upgrade_attempt() gets the status of the last upgrade.
 *
 * This function requires no arguments.
 *
 * @return	EOK           - No errors
 * @return	ECRC          - CRC error
 * @return	-LIBUSB_ERROR - libusb specific error.
 */
int _DLL fl_last_upgrade_attempt(void);

/**
 * fl_version() returns the hardcoded firmware version of either the bootloader
 * or main firmware without the patch. This is useful for quickly identifying
 * generation of the device.
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
 * @return	-LIBUSB_ERROR - libusb specific error.
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
 * @return	-LIBUSB_ERROR - libusb specific error.
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
 * @return	-LIBUSB_ERROR - libusb specific error.
 */
int _DLL fl_patch_version(void);

/**
 * fl_version_str() retrieves the firmware version of the bootloader or main
 * firmware in a string format. The returned string is a constant static
 * string and should not be deallocated.
 *
 * This function does not take any parameters.
 *
 * @return      VERSION       - Firmware version number as a string
 * @return      -ENODEV       - Flirc device is not present
 * @return      -EWRONGDEV    - Device is in unsupported bootloader mode
 * @return      -LIBUSB_ERROR - Incorrect mode has been selected
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
 * @return	-LIBUSB_ERROR - libusb specific error.
 */
uint32_t _DLL fl_fw_scm_hash(void);

/**
 * fl_fw_branch() retrieves the firmware's build branch. It's a static constant
 * string that shouldn't be deallocated.
 *
 * The function doesn't require any arguments.
 *
 * @return      char *        - Returns the build branch.
 * @return      NULL          - Indicates an error, actual error can be
 *                              determined by calling another function.
 */
char _DLL *fl_fw_branch(void);

/**
 * fl_fw_config() returns the firmware configuration string. It's a static
 * constant string that shouldn't be deallocated.
 *
 * This function requires no arguments.
 *
 * @return      char *        - Returns the build branch.
 * @return      NULL          - Indicates an error, actual error can be
 *                              determined by calling another function.
 */
char _DLL *fl_fw_config(void);

/**
 * fl_fw_dirty() returns a string that represents the extended version dirty
 * tag. It's a static constant string that shouldn't be deallocated.
 *
 * This function requires no arguments.
 *
 * @return      char *        - Returns the build branch.
 * @return      NULL          - Indicates an error, actual error can be
 *                              determined by calling another function.
 */
char _DLL *fl_fw_dirty(void);

/**
 * _DLL fl_leave_bootloader() instructs the bootloader to switch to the
 * primary image. Useful when no new firmware image is available, or DFU
 * mode was entered accidentally. It enables the firmware to return to its
 * normal state.
 *
 * Note: On reboot, the device may stay in the bootloader unless
 * fl_set_boot_flag is called while the device runs its normal application
 * image.
 *
 * This function doesn't require any arguments.
 *
 * @return      EOK           - Operation successful
 * @return      -ENODEV       - Flirc device not found
 * @return      -LIBUSB_ERROR - Invalid mode selected.
 */
int _DLL fl_leave_bootloader(void);

/**
 * fl_reboot() sends a command that resets the device, similar to
 * unplugging and replugging it. No data is lost with this command,
 * and it's rarely necessary to call it.
 *
 * This function does not require any arguments.
 *
 * @return      EOK           - Success
 * @return      -ENODEV       - Flirc not present
 * @return      -EWRONGDEV    - Unsupported, device in bootloader
 * @return      -LIBUSB_ERROR - Bad mode selected
 */
int _DLL fl_reboot(void);

/**
 * fl_wait_for_device() checks for flirc's presence in the system, and if
 * found, opens the device.
 *
 * @param VID  - USB Vendor ID
 * @param *mfg - Manufacturer string
 *
 * @return     BOOTLOADER   - Bootloader detected and opened
 * @return     FIRMWARE     - Successfully accessed firmware
 * @return     -ENXIO       - No device or address found (stderr.h)
 */
int _DLL fl_wait_for_device(unsigned int VID, const char *mfg);

/**
 * fl_wait_for_device_timeout() waits for flirc to be present, and
 * opens the device. If the timeout has passed, the function fails.
 * This function is unsuitable for the alternative interface and is
 * used during upgrades.
 *
 * This is a helper function that can be implemented with a loop that
 * occasionally tries to open the device. Used internally during
 * upgrades.
 *
 * @param VID     - USB Vendor ID
 * @param *mfg    - Manufacturer string
 * @param timeout - Timeout in seconds
 *
 * @return BOOTLOADER - Bootloader found and opened
 * @return FIRMWARE   - Successfully opened firmware
 * @return -ENXIO     - No such device or address
 * @return -ETIMEOUT  - Timeout reached
 */
int _DLL fl_wait_for_device_timeout(unsigned int VID,
                const char *mfg, int timeout);

/**
 * fl_upgrade_fw() is an all-in-one firmware upgrader. It waits for a device,
 * sets it in DFU mode (if not already), waits for it to reappear, and uploads
 * the specified firmware file.
 *
 * @param user_file - Firmware image file location
 * @param VID       - USB Vendor ID
 * @param *man      - Manufacturer string
 * @param *priv     - Container for C++ class, progress bar
 * @param *p_bar    - Function pointer to progress bar
 *
 * @return EOK             - Success
 * @return -EUPGRADE_FAIL  - Upgrade failed
 */
int _DLL fl_upgrade_fw(const char *user_file, unsigned int VID,
                       const char *manufacturer, void *priv,
                       void (*p_bar)(int perc, void *priv));

/**
 * fl_upgrade_fw_timeout() is an all-in-one firmware upgrader function. It waits
 * for a device to appear and fails if the timeout is reached.
 *
 * @param user_file - The path of the firmware image.
 * @param VID       - The USB Vendor ID.
 * @param *man      - The manufacturer string.
 * @param *priv     - Container for C++ class, progress bar.
 * @param *p_bar    - Function pointer to progress bar.
 * @param timeout   - The timeout duration in seconds.
 *
 * @return EOK            - Successful upgrade.
 * @return -EUPGRADE_FAIL - The upgrade process failed.
 */
int _DLL fl_upgrade_fw_timeout(const char *user_file, unsigned int VID,
                               const char *man, void *priv,
                               void (*p_bar)(int perc, void *priv),
                               int timeout);
/**
 * fl_upgrade_fw_ext() is an all-in-one firmware upgrade function. It uses a
 * buffer instead of a file for the firmware image.
 *
 * @param buff   - Firmware image buffer.
 * @param VID    - USB Vendor ID.
 * @param *man   - Manufacturer string.
 * @param *priv  - Container for C++ class, progress bar.
 * @param *p_bar - Function pointer to progress bar update function.
 *
 * @return EOK            - Upgrade successful.
 * @return -EUPGRADE_FAIL - Upgrade failed.
 */
int _DLL fl_upgrade_fw_ext(unsigned char *buf, size_t len, int VID,
                const char *manufacturer, void *priv,
                void (*p_bar)(int perc, void *priv));

/**
 * fl_upgrade_fw_ext_timeout() all in one upgrader function. When waiting for
 * device to appear, will fail after timeout has passed. Otherwise, is
 * identical to above.
 *
 * @param buff    - image buffer
 * @param VID     - USB Vendor ID
 * @param *man    - manufacturer string
 * @param *priv   - container for C++ class, progress bar
 * @param *p_bar  - function pointer to progress bar
 * @param timeout - timeout in seconds
 *
 * @return EOK            - Success
 * @return -EUPGRADE_FAIL - Upgrade failed
 */
int _DLL fl_upgrade_fw_ext_timeout(unsigned char *buf, size_t len, int VID,
		const char *manufacturer, void *priv,
		void (*p_bar)(int perc, void *priv), int timeout);
/**
 * \ingroup desc
 * flirc_parms_t defines parameter symbols for recording sessions.
 *
 * WAIT=1       - Parameter used to indicate waiting in setRecord function.
 * DONT_WAIT=0  - Parameter used to indicate no waiting in setRecord function.
 */
typedef enum flirc_parms_t {
        WAIT=1,
        DONT_WAIT=0,
} flirc_parms_t;

/**
 * fl_set_delete() switches flirc into delete mode. In this mode, the next
 * remote button that flirc detects will be removed from its memory. If the
 * button is not found, flirc ignores the request and resumes normal operation.
 * If found, the button is deleted and flirc returns to normal operation.
 *
 * @param WAIT      - Enables blocking functionality
 * @param DONT_WAIT - Enables non-blocking functionality
 *
 * @return EOK            - Operation completed successfully
 * @return -ENODEV        - Flirc device not present
 * @return -EWRONGDEV     - Unsupported, device in bootloader mode
 * @return -KEY_NOT_FOUND - Deletion failed, button does not exist
 * @return -LIBUSBERR     - LIBUSB error occurred
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
 * fl_set_noise_cancel() toggles the noise canceling feature on flirc. This
 * function is used when phantom keys are detected on flirc.
 *
 * @param val  - 1: Enable Suspend Detection, 0: Disable Suspend Detection
 *
 * @return EOK         - Operation completed successfully
 * @return -EINVAL     - Invalid value was provided
 * @return -ENODEV     - Flirc device not present
 * @return -LIBUSBERR  - LIBUSB error occurred
 */
int _DLL fl_set_noise_cancel(int val);

/**
 * fl_get_noise_cancel() retrieves the current noise cancellation state.
 *
 * This function doesn't require any arguments.
 *
 * @return 1 - Noise Cancellation Enabled
 * @return 0 - Noise Cancellation Disabled
 *
 * @return -ENODEV    - Flirc device not present
 * @return -LIBUSBERR - Error in LIBUSB
 */
int _DLL fl_get_noise_cancel(void);

/**
 * fl_set_sensitivity() configures the sensitivity level of flirc.
 *
 * This is an advanced function, not yet available in GUI.
 *
 * @param  val  - Sensitivity level. Should be between 0-3
 *                (low_sens to hig_sens).
 *
 * @return EOK        - Operation completed successfully
 * @return -EINVAL    - If the input value is invalid.
 * @return -ENODEV    - If flirc device is not present.
 * @return -LIBUSBERR - In case of a LIBUSB error.
 */
int _DLL fl_set_sensitivity(int val);

/**
 * fl_get_sensitivity() retrieves the currently set sensitivity level.
 *
 * This function does not require any arguments.
 *
 * @return sensitivity - Current sensitivity level (1-3).
 *
 * @return      -ENODEV      - If flirc device is not present.
 * @return      -LIBUSBERR   - In case of a LIBUSB error.
 */
int _DLL fl_get_sensitivity(void);

/**
 * fl_set_interkey_delay() Sets the interkey delay. This will help with button
 * repeat issues. This function is only for advanced users. This only is
 * supported on the first generation of flirc.
 *
 * @param val - A value between 0-7. This is translated to a delay in ms by the
 * firmware.
 *
 * @return EOK         - Operation completed successfully.
 * @return -EINVAL     - Invalid input value.
 * @return -ENODEV     - Flirc device not present.
 * @return -LIBUSBERR  - Error occurred in LIBUSB.
 */
int _DLL fl_set_interkey_delay(int val);

/**
 * fl_get_interkey_delay() retrieves the currently set interkey delay value.
 * Only supported on the first generation of Flirc.
 *
 * This function requires no arguments.
 *
 * @return val - Represents a delay in ms (range: 0-7) as per firmware.
 *
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBERR - LIBUSB error code
 */
int _DLL fl_get_interkey_delay(void);

/**
 * fl_enable_rom_table() enables or disables a specified rom table.
 *
 * @param table - Specifies the rom table to be modified:
 *                0 - FireTV
 *                1 - Kodi
 *                2 - Streacom (Microsoft WMCE)
 *                3 - Flirc WMCE
 *                4 - NVIDIA Shield
 *
 * @param en    - Determines the state of the rom table:
 *                0 - Disable
 *                1 - Enable
 *
 * @return EOK       - Operation completed successfully.
 * @return -EINVAL   - Invalid table/en value provided.
 * @return -ENODEV   - Flirc device not detected.
 * @return -LIBUSBERR- LIBUSB encountered an error.
 */
int _DLL fl_enable_rom_table(uint32_t table, uint8_t en);

/**
 * fl_get_rom_table() retrieves the state of a specified table.
 *
 * @param table - The table to verify. The options are:
 *                0 - FireTV
 *                1 - Kodi
 *                2 - Streacom (Microsoft WMCE)
 *                3 - Flirc WMCE
 *                4 - NVIDIA Shield
 *
 * @return 0         - Table is Disabled
 * @return 1         - Table is Enabled
 * @return -ENODEV   - Flirc not present
 * @return -LIBUSBERR - LIBUSB error code
 */
int _DLL fl_get_rom_table(uint32_t table);

/**
 * fl_set_sequence_modifiers() toggles sequencing of modifier keys and others.
 * Particularly useful for windows media center users. Instead of sending
 * CTRL+ALT+R simultaneously, it first presses control, then alt, then R, until
 * all are pressed together.
 *
 * @param val - 0:1 (disable/enable)
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc not present.
 * @return -LIBUSBERR - LIBUSB error.
 */
int _DLL fl_set_sequence_modifiers(int val);

/**
 * fl_get_sequence_modifiers() retrieves the current sequence modifier state.
 *
 * This function doesn't require any arguments.
 *
 * @return 0:1       - Disable/enable sequence modifiers
 * @return -ENODEV   - Flirc device not found
 * @return -LIBUSBERR - LIBUSB error occurred
 */
int _DLL fl_get_sequence_modifiers(void);

/**
 * fl_set_builtin_profile_support() toggles built-in profile support like
 * harmony xbmc profile. If disabled, keys can be remapped by the user.
 *
 * @param val - 0:1 (Disable:Enable)
 *
 * @return EOK        - Operation successful.
 * @return -EINVAL    - Invalid value.
 * @return -ENODEV    - Flirc not present.
 * @return -LIBUSBERR - LIBUSB error code.
 */
int _DLL fl_set_builtin_profile_support(int val);

/**
 * fl_get_builtin_profile_support() retrieves the status of builtin profile
 * support. It doesn't require any arguments.
 *
 * @return 1       - Feature is enabled
 * @return 0       - Feature is disabled
 * @return -ENODEV - Flirc device not present
 * @return -LIBUSBERR - LIBUSB error code
 */
int _DLL fl_get_builtin_profile_support(void);

/**
 * fl_set_sleep_detect() controls sleep detection for Flirc devices.
 * If the host is asleep, a wakeup command is sent before the paired key.
 * This is only supported on Flirc's first generation. Flirc 2.0 always
 * wakes up the machine unless the 'wake' key is recorded.
 *
 * @param val - 1 to enable suspend detection, 0 to disable it.
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc device is not present.
 * @return -LIBUSBERR - Error code from LIBUSB.
 */
int _DLL fl_set_sleep_detect(int val);

/**
 * fl_get_sleep_detect() checks if suspend detection is enabled.
 * It's only supported on the first Flirc generation.
 *
 * This function doesn't require any arguments.
 *
 * @return 1           - If suspend is enabled.
 * @return 0           - If suspend is not enabled.
 * @return -ENODEV     - If Flirc is not present.
 * @return -LIBUSBERR  - In case of a LIBUSB error code.
 */
int _DLL fl_get_sleep_detect(void);

/**
 * fl_fw_state() gets the state of flirc
 *
 * This function requires no arguments.
 *
 * @return BOOTLOADER - Flirc is in bootloader state
 * @return FIRMWARE   - Flirc is in firmware state
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBERR - LIBUSB error code
 */
int _DLL fl_fw_state(void);

/**
 * fl_eeprom_peek() returns data in given address
 *
 * @param add - offset in the EEPROM
 *
 * @return val        - value inside address
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBERR - LIBUSB error code
 */
int _DLL fl_eeprom_peek(int address);

/**
 * fl_eeprom_poke() write the data to the specified offset
 *
 * @param  add	- offset in the EEPROM
 * @param  data	- data to put in offset
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBERR - LIBUSB error code
 */
int _DLL fl_eeprom_poke(uint16_t address, uint16_t data);

/**
 * fl_get_state() returns the status of the flirc IR state machine. This
 * function allows for granular control as the state can be set or cleared.
 *
 * @return 0 - STATE_PLAYBACK: Normal Operation
 * @return 1 - STATE_RECORD: The next button will be recorded with the HID
 * @return 2 - STATE_DELETE: The next IR code will be deleted from storage
 * @return 3 - SUCCESS: The previous state was successful
 * @return -ENODEV: Flirc not present
 * @return -LIBUSBERR: LIBUSB error code
 * @return -EWRONGDEV: Unsupported, device in bootloader
 */
int _DLL fl_get_state(void);

/**
 * fl_clear_state() resets the state machine, puts the device back into normal
 * operation, or STATE_PLAYBACK.
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBER  - LIBUSB error code
 * @return -EWRONGDEV - Unsupported, device in bootloader
 */
int _DLL fl_clear_state(void);

/**
 * fl_display_config() prints out the configuration in human readable
 * format. Depends on stdout. For use in a commandline application.
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBER  - LIBUSB error code
 * @return -EWRONGDEV - Unsupported, device in bootloader
 */
int fl_display_config(void);

/**
 * fl_format_config() clears all saved configuration data in the device. All
 * recorded buttons will be erased.
 *
 * This function requires no arguments.
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBER  - LIBUSB error code
 * @return -EWRONGDEV - Unsupported, device in bootloader
 */
int _DLL fl_format_config(void);

/**
 * fl_save_config() this function downloads the configuration file from a flirc
 * device and saves it to the location specified by the user. The resulting file
 * format is a raw binary file that is proprietary.
 *
 * @param file - location to save the file
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBER  - LIBUSB error code
 * @return -EWRONGDEV - Unsupported, device in bootloader
 * @return -EBADF     - File could not be created/opened
 * @return -ENOMEM    - Malloc fail
 */
int _DLL fl_save_config(const char *user_file);

/**
 * fl_load_config() uploads a file to a flirc device that was previously
 * saved with fl_save_config().
 *
 * @param file - location of the file to load
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBER  - LIBUSB error code
 * @return -EWRONGDEV - Unsupported, device in bootloader
 * @return -EBADF     - File could not be created/opened
 * @return -ENOMEM    - Malloc fail
 * @return -EFAULT    - Invalid file address
 */
int _DLL fl_load_config(const char *user_file);

/**
 * fl_keys_recorded() gets the used space in size 'keys'. If one button was
 * recorded, we return 1.
 *
 * @return space_used - space free on device
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBER  - LIBUSB error code
 * @return -EWRONGDEV - Unsupported, device in bootloader
 */
int fl_keys_recorded(void);

/**
 * fl_keys_remaining() gets the space free, in size keys
 *
 * @return space_free - space free on device
 * @return -ENODEV    - Flirc not present
 * @return -LIBUSBER  - LIBUSB error code
 * @return -EWRONGDEV - Unsupported, device in bootloader
 * @return -ENOMEM    - Malloc fail
 */
int fl_keys_remaining(void);

/**
 * fl_delete_index() specify the index of the key to delete
 *
 * @param index - index to delete.
 *
 * @return EOK           - Operation successful.
 * @return -EINVAL       - Invalid value
 * @return -ENODEV       - Flirc not present
 * @return -EWRONGDEV    - Unsupported, device in bootloader
 * @return -LIBUSB_ERROR - libusb specific error.
 */
int fl_delete_index(int index);

/**
 * fl_libusb_logging() toggles libusb logging. Primarily useful for
 * debugging Flirc gen 1, which employs libusb.
 *
 * @param enable - 1 to activate logging, 0 to deactivate.
 *
 * This function does not return any value.
 */
void fl_libusb_logging(int enable);

/**
 * fl_log() get usb event log. This function returns the number of characters
 * placed in the passed buf
 *
 * @param buf - empty buf pointer to be filled. Min of 1024
 *
 * @return EOK           - Operation successful, but no bytes left
 * @return -EINVAL       - Invalid value
 * @return -ENODEV       - Flirc not present
 * @return -EWRONGDEV    - Unsupported, device in bootloader
 * @return -LIBUSB_ERROR - libusb specific error.
 */
int fl_log(char *buf);

/**
 * fl_get_sku() retrieves the embedded sku from a device
 *
 * This function requries no arguments.
 *
 * @return char * - Returns the build branch.
 * @return NULL   - Indicates an error, actual error can be determined by
 * calling another function.
 */
char * _DLL fl_get_sku(void);

/**
 * fl_print_ver_diff() - Logs firmware version history based on a comparison
 * string. The string must be a valid semantic string. This function is
 * particularly useful for applications utilizing stdout.
 *
 * @param comp - Semantic string to compare against firmware version.
 *
 * @return     - Integer status code representing success or failure.
 */
int fl_print_ver_diff(const char *comp);

/**
 * fl_get_ver_hist() - Fetches the head of the firmware version history log
 * list. The function compares the provided string against the firmware
 * version to generate the list.
 *
 * @param comp - String to compare against firmware version.
 *
 * @return     - Pointer to the head of the firmware version history log list.
 *               Returns NULL on error.
 */
struct list_head *fl_get_ver_hist(const char *comp);

/**
 * fl_set_debug_pipe() enables a specific log pipe. Currently only supports
 * RAW_IR.
 *
 * @param index - 0 RAW_IR
 * @param en    - 1 Enable, 0 Disable
 *
 * @return EOK           - Operation successful.
 * @return -EINVAL       - Invalid value
 * @return -ENODEV       - Flirc not present
 * @return -EWRONGDEV    - Unsupported, device in bootloader
 * @return -LIBUSB_ERROR - libusb specific error
 */
int _DLL fl_set_debug_pipe(uint32_t pipe, uint8_t en);

/**
 * fl_get_debug_pipe() retrieves a 32 bit value where each bit index represents
 * a debug pipe. Since RAW_IR is the only one supported, this value will always be
 * 1, 0, or -1.
 *
 * @param 	- none
 *
 * @return pipe          - 32 bit (0 - none, 1 - RAW_IR)
 * @return -ENODEV       - Flirc not present
 * @return -EWRONGDEV    - Unsupported, device in bootloader
 * @return -LIBUSB_ERROR - libusb specific error.
 */
int _DLL fl_get_debug_pipe(uint32_t pipe);

/**
 * fl_normalize_config() - Normalizes recorded keys for single remote setups.
 *
 * This function calculates the interkey delay of the configuration and
 * normalizes all recorded keys. It's designed specifically for Flirc Gen2
 * hardware. Avoid using this function if you have multiple remotes. Do not
 * use this on gen1 hardware.
 *
 * @param       - None. This function doesn't take any parameters.
 *
 * @return EOK           - Operation successful.
 * @return -1       	 - Other error
 * @return -ENODEV       - Flirc not present
 * @return -EWRONGDEV    - Unsupported, device in bootloader
 */
int _DLL fl_normalize_config(void);

/**
 * fl_transmit_raw() - Sends a raw IR packet for Flirc Gen2 hardware.
 *
 * Transmits a raw IR packet, specified in microseconds. Designed for Flirc
 * Generation 2 hardware. Care is needed for buffer format, length, and
 * inter-key delays, considering firmware version differences.
 *
 * @param buf    - Data to transmit, in microseconds.
 * @param len    - Buffer length; max 100 timing markers
 * @param ik     - Delay between packets. Microseconds for firmware < 4.10.0,
 *                 milliseconds for >= 4.10.0. Auto-detects and converts units.
 *                 Defaults to 40ms if too low.
 * @param repeat - Repeat count, typically 3. Zero for infinite, currently not
 *                 supported.
 *
 * @return EOK           - Operation successful.
 * @return -ENODEV       - Flirc not present
 * @return -EWRONGDEV    - Unsupported, device in bootloader
 */
int _DLL fl_transmit_raw(uint16_t *buf, uint16_t len, uint16_t ik, uint8_t repeat);

/**
 * flirc_send_pronto() transmits a Pronto-encoded buffer. This function
 * is only supported on gen2 hardware.
 *
 * @param *buf   - Pointer to the buffer that holds the Pronto codes.
 * @param length - The number of elements in the buffer.
 * @param rep    - The number of times the transmission is repeated.
 *
 * @return EOK        - Indicates a successful operation.
 * @return -ENODEV    - Flirc device is not present.
 * @return -EWRONGDEV - Device is unsupported or in bootloader.
 */
int _DLL flirc_send_pronto(uint16_t *buf, uint32_t length, uint8_t rep);

/**
 * fl_unit_test() conducts a closed loop test. It sends a signal and verifies
 * if the transmitted signal is identical to the received one.
 *
 * This function does not require any parameters.
 *
 * @return EOK        - Successful operation.
 * @return -1         - Test failure.
 * @return -ENODEV    - Flirc device is not found.
 * @return -EWRONGDEV - Device is unsupported or in bootloader.
 */
int _DLL fl_unit_test(void);

/**
 * fl_last_hash() grab the last hash seen, doesn't have to be in the database. Only
 * supported on gen2 hardware.
 *
 * @param 	- none
 *
 * @return hash       - 32 bit unique button hash
 * @return -ENODEV    - Flirc not present
 * @return -EWRONGDEV - Unsupported, device in bootloader
 */
int _DLL fl_last_hash(void);

typedef enum {
    IOSPIRIT = 0,
} usb_iface_type;

/**
 * fl_usb_iface_en() enable or disable a specific usb interface. Only supported
 * on gen2 hardware.
 *
 * @param usb_iface_type - See enums below
 * @param en		 - enable or disable
 *
 * @return EOK        - Successful operation.
 * @return -ENODEV    - Flirc device is not found.
 * @return -EWRONGDEV - Device is unsupported or in bootloader.
 */
int _DLL fl_usb_iface_en(usb_iface_type type, uint8_t en);

/**
 * struct ir_packet - Represents a single captured IR packet.
 *
 * @param buf      - Timing buffer containing the timings (in microseconds) 
 *                   of the captured IR signal.
 * @param len      - The actual number of entries populated in 'buf'. 
 * @param elapsed  - Time (in miliseconds) elapsed since the last edge 
 *                   was captured.
 */
struct ir_packet {
	uint16_t buf[256];
	uint16_t len;
	uint16_t elapsed;
};

/**
 * fl_ir_packet_poll() - Polls for an IR packet from a flirc USB device.
 *
 * This function checks if a flirc USB device has a complete IR packet ready.
 * If a frame is available, the 'ir' structure is filled with timing data and
 * relevant details. Useful for real-time systems or applications that need to
 * regularly check for IR input.
 *
 * @param *ir  - Pointer to the ir_packet structure to store the captured IR
 *               signal details.
 *
 * @return 0           - No frame available yet.
 * @return 1           - Frame ready and 'ir' is populated.
 * @return -ENODEV     - Flirc not present.
 * @return -EWRONGDEV  - Unsupported, device in bootloader.
 */
int _DLL fl_ir_packet_poll(struct ir_packet *ir);

/**
 * fl_dev_flush() - Clears any pending data in the ir raw receive endpoint. After polling
 * the device and a packet is received, calling this function is necessary to ensure the 
 * queue is properly flushed. 
 *
 * @return EOK        - Successful operation.
 * @return -ENODEV    - Flirc not present.
 * @return -EWRONGDEV - Unsupported, device in bootloader.
 */
void fl_dev_flush(void);

/**
 * fl_ir_transmit_kill() - Stops the transmitter from transmission.
 *
 * This function is used to halt the ongoing transmission from the transmitter.
 *
 * @param none
 *
 * @return EOK        - Successful operation.
 * @return -ENODEV    - Flirc not present.
 * @return -EWRONGDEV - Unsupported, device in bootloader.
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
 * @param wake_en  - enable or disable, Bit: 0
 * @param high_add - Apple Remote ADDR high
 * @param low_add  - Apple Remote ADDR low
 *
 * @return EOK        - Successful operation.
 * @return -ENODEV    - Flirc not present.
 * @return -EWRONGDEV - Unsupported, device in bootloader.
 */
int _DLL fl_rb_settings_set(uint32_t wake_en,
		uint8_t high_add, uint8_t low_add);

/**
 * fl_rb_settings_get() grab the settings on the device. Variables passed
 * by reference.
 *
 * @param wake_en  - enable or disable, Bit: 0
 * @param high_add - Apple Remote ADDR high
 * @param low_add  - Apple Remote ADDR low
 *
 * @return EOK        - Successful operation.
 * @return -ENODEV    - Flirc not present.
 * @return -EWRONGDEV - Unsupported, device in bootloader.
 */
int _DLL fl_rb_settings_get(uint32_t *wake_en,
		uint8_t *high_add, uint8_t *low_add);

/**
 * fl_rb_wake_sig() send a buffer to the device, which is the raw timing
 * waveform of the signal which will be used to wake the computer. To remove
 * a saved key, send any buffer with length equal to 1, which is invalid.
 *
 * @param *buf - Data to be used for wake
 * @param len  - Length of the data, must be even to represent a valid signal
 *
 * @return EOK        - Operation successful.
 * @return -ENODEV    - Flirc device not present.
 * @return -EWRONGDEV - Unsupported, device in bootloader.
 */
int _DLL fl_rb_wake_sig(uint16_t *buf, uint8_t len);

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

/**
 * \ingroup desc
 * fl_types_t is an enumeration of error and state types.
 *
 * The enum values are used to indicate the state of a process or the type
 * of an error that occurred during execution.
 *
 * @value EOK               - No error occurred
 * @value STATE_PLAYBACK    - Playback state
 * @value STATE_RECORD      - Record state
 * @value STATE_DELETE      - Delete state
 * @value NO_INTERRUPT      - No interrupt, returned from setRecord
 * @value ENOMEM            - Out of memory error
 * @value EBADF             - Bad file descriptor error
 * @value EFAULT            - Bad address error
 * @value EINVAL            - Invalid argument error
 * @value ENODEV            - No device error
 * @value ENOSYS            - Function not implemented error
 * @value ECANCELED         - Operation canceled error
 * @value EWRONGDEV         - Wrong device error
 * @value EUPGRADE_FAILED   - Upgrade failed error
 * @value EIDXRANGE         - Index out of range error
 * @value ENXIO             - No such device or address error
 * @value LIBUSBERR         - LIBUSB error code
 * @value ETIMEOUT          - Error, time out
 * @value UNKNOWN_DEV       - Invalid USB device error
 * @value _ERROR_T_COUNT    - Count of error types
 */
typedef enum fl_types_t {
	EOK = 0,
	STATE_PLAYBACK = 0,
	STATE_RECORD = 1,
	STATE_DELETE = 2,
	NO_INTERRUPT = 4,
	ENOMEM,
	EBADF,
	EFAULT,
	EINVAL,
	ENODEV,
	ENOSYS,
	ECANCELED,
	EWRONGDEV,
	EUPGRADE_FAILED,
	EIDXRANGE,
	ENXIO, 
	LIBUSBERR,
	ETIMEOUT,
	UNKNOWN_DEV,
	_ERROR_T_COUNT
} error_type;

/**
 * strerr() converts an error code to a corresponding string.
 *
 * This function takes an error code as an input and returns the
 * corresponding error message string. It is useful for providing
 * more descriptive error messages to the user.
 *
 * @param err  - Error code to be converted to a string.
 *
 * @return     err_string - A constant string that describes the
 *                          error corresponding to the input code.
 */
const char _DLL *strerr(int err);

/**
 * Windows Compatibility
 */
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
