#include <stdint.h>
#include <string.h>
#include "ble_detect.h"
#include "nrf_gpio.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "SEGGER_RTT.h"


// ALREADY_DONE_FOR_YOU: Declaration of a function that will take care of some housekeeping of ble connections related to our service and characteristic
void ble_our_service_on_ble_evt(ble_os_t * p_ble_detect, ble_evt_t * p_ble_evt)
{
    // OUR_JOB: Step 3.D Implement switch case handling BLE events related to our service. 
		switch (p_ble_evt->header.evt_id)
		{
				case BLE_GAP_EVT_CONNECTED:
						p_ble_detect ->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
						break;
				case BLE_GAP_EVT_DISCONNECTED:
						p_ble_detect ->conn_handle = BLE_CONN_HANDLE_INVALID;
						break;
				default:
						break;
						
    }
				
}

/**@brief Function for adding our new characterstic to "Our service" that we initiated in the previous tutorial. 
 *
 * @param[in]   p_ble_detect        Our Service structure.
 */
static uint32_t detect_char_add(ble_os_t * p_ble_detect)
{
		// Step 2.A, Add a custom characteristics UUID
		uint32_t					  err_code;
		ble_uuid_t					char_uuid;
		ble_uuid128_t				base_uuid = BLE_UUID_OUR_BASE_UUID;
		char_uuid.uuid			= BLE_UUID_CHARACTERISTIC_UUID;
		err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
		APP_ERROR_CHECK(err_code);
		
		// Step 2.F, Add read/write properties to our characteristics.
		ble_gatts_char_md_t char_md;
		memset(&char_md, 0, sizeof(char_md));
		char_md.char_props.read = 1;
		char_md.char_props.write = 0;
		
		                                                
		// Step 3.A, Configure Client Characteristics Configuration Descriptor metadata and add to char_md structure
		ble_gatts_attr_md_t cccd_md;
		memset(&cccd_md, 0, sizeof(cccd_md));
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
		cccd_md.vloc							= BLE_GATTS_VLOC_STACK;
		char_md.p_cccd_md					= &cccd_md;
		char_md.char_props.notify = 1;
		
		
		
		// Step 2.B, Configrure the attribute metadata
		ble_gatts_attr_md_t attr_md;
		memset(&attr_md, 0, sizeof(attr_md));
		attr_md.vloc				= BLE_GATTS_VLOC_STACK;
		
		
		// Step 2.G, Set read/write security levels to our characteristics
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
		
		// Step 2.C, Configure the caracteristic value attribute
		ble_gatts_attr_t attr_char_value;
		memset(&attr_char_value, 0, sizeof(attr_char_value));
		attr_char_value.p_uuid			= &char_uuid;
		attr_char_value.p_attr_md		= &attr_md;
		
		
		
		// Step 2.H, Set characteristics length in number of bytes
		attr_char_value.max_len 	= 1;
		attr_char_value.init_len 	= 1;
		uint8_t value[1] 					= {0x00};
		attr_char_value.p_value		= value; 
		
		// Step 2.E, Add our new characteristics to the service
		err_code = sd_ble_gatts_characteristic_add(p_ble_detect->service_handle, &char_md, &attr_char_value, &p_ble_detect->char_handles);
		APP_ERROR_CHECK(err_code);
		
		
		return NRF_SUCCESS;
}

/**@brief Function for initiating our new service.
 *
 * @param[in]   p_ble_detect        Our Service structure.
 *
 */ 
void ble_detect_init(ble_os_t * p_ble_detect)
{

		uint32_t err_code; // Variable to hold return codes from library and softdevice functions
		
	
    // STEP 3: Declare 16 bit service and 128 bit base UUIDs and add them to BLE stack table     
		ble_uuid_t	service_uuid;
		ble_uuid128_t	base_uuid = BLE_UUID_OUR_BASE_UUID;
		service_uuid.uuid = BLE_UUID_OUR_SERVICE;
		err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
		APP_ERROR_CHECK(err_code);
		
    //Step 3.B, Set our service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
		p_ble_detect->conn_handle = BLE_CONN_HANDLE_INVALID;
		
		// STEP 4: Add our service

		err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &p_ble_detect->service_handle);
		APP_ERROR_CHECK(err_code);
		
		// OUR_JOB: Call the function our_char_add() to add our new characteristic to the service. 
		detect_char_add(p_ble_detect);
		
    // Print messages to Segger Real Time Terminal
    // UNCOMMENT THE FOUR LINES BELOW AFTER INITIALIZING THE SERVICE OR THE EXAMPLE WILL NOT COMPILE.
		//    SEGGER_RTT_WriteString(0, "Exectuing our_service_init().\n"); // Print message to RTT to the application flow
		//    SEGGER_RTT_printf(0, "Service UUID: 0x%#04x\n", service_uuid.uuid); // Print service UUID should match definition BLE_UUID_OUR_SERVICE
		//    SEGGER_RTT_printf(0, "Service UUID type: 0x%#02x\n", service_uuid.type); // Print UUID type. Should match BLE_UUID_TYPE_VENDOR_BEGIN. Search for BLE_UUID_TYPES in ble_types.h for more info
		//    SEGGER_RTT_printf(0, "Service handle: 0x%#04x\n", p_ble_detect->service_handle); // Print out the service handle. Should match service handle shown in MCP under Attribute values
		
		
}

// ALREADY_DONE_FOR_YOU: Function to be called when updating characteristic value
void sensor_characteristic_update(ble_os_t *p_ble_detect, uint32_t *sensor_value)
{
    // OUR_JOB: Step 3.E, Update characteristic value
    if (p_ble_detect->conn_handle != BLE_CONN_HANDLE_INVALID)
		{
				uint16_t									len = 1;
				ble_gatts_hvx_params_t		hvx_params;
				memset(&hvx_params, 0, sizeof(hvx_params));
				
				hvx_params.handle	= p_ble_detect->char_handles.value_handle;
				hvx_params.type		= BLE_GATT_HVX_NOTIFICATION;
				hvx_params.offset	= 0;
				hvx_params.p_len	= &len;
        hvx_params.p_data = (uint8_t*)sensor_value;
				
				sd_ble_gatts_hvx(p_ble_detect->conn_handle, &hvx_params);
				
		}

}
