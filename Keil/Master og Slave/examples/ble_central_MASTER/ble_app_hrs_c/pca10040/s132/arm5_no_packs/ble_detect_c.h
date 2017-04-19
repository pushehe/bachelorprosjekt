#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_UUID_OUR_BASE_UUID              {0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00} // 128-bit base UUID
#define BLE_UUID_OUR_SERVICE                0xABCD // Just a random, but recognizable value

#define BLE_UUID_CHARACTERISTIC_UUID				0xBEEF


typedef enum
{
    BLE_DETECT_C_EVT_DISCOVERY_COMPLETE = 1,  	/**< Event indicating that the BLE Detect Service has been discovered at the peer. */
    BLE_DETECT_C_EVT_NOTIFICATION         			/**< Event indicating that a notification of the Detection characteristic has been received from the peer. */
} ble_detect_c_evt_type_t;

typedef struct
{
		uint32_t		detect_value;		
}ble_detect_t;



typedef struct
{
		uint16_t										detect_cccd_handle;
		uint16_t										detect_handle;
	
}detect_db_t;

typedef struct
{
		 ble_detect_c_evt_type_t	evt_type;
		 uint16_t									conn_handle;
		 union
		 {
					detect_db_t					peer_db;
					ble_detect_t				value;
		 } params;
} ble_detect_c_evt_t;