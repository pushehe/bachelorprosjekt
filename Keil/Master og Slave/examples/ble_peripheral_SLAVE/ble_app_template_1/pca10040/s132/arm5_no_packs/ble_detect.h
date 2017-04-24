/* Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 * 
 */

#ifndef BLE_DETECT_H__
#define BLE_DETECT_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_UUID_OUR_BASE_UUID              {0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00} // 128-bit base UUID
#define BLE_UUID_OUR_SERVICE                0xABCD // Just a random, but recognizable value

#define BLE_UUID_CHARACTERISTIC_UUID				0xBEEF
#define BLE_UUID_CHARACTERISTICS_DOOR_UUID 	0xFEED
/**
 * @brief This structure contains various status information for our service. 
 * It only holds one entry now, but will be populated with more items as we go.
 * The name is based on the naming convention used in Nordic's SDKs. 
 * 'ble’ indicates that it is a Bluetooth Low Energy relevant structure and 
 * ‘os’ is short for Our Service). 
 */
typedef struct
{
    uint16_t									conn_handle;
    uint16_t									service_handle;     /**< Handle of Our Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t	char_handles;		
}ble_os_t;

/**@brief Function for handling BLE Stack events related to our service and characteristic.
 *
 * @details Handles all events from the BLE stack of interest to Our Service.
 *
 * @param[in]   p_our_service       Our Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_our_service_on_ble_evt(ble_os_t * p_our_service, ble_evt_t * p_ble_evt);

/**@brief Function for initializing our new service.
 *
 * @param[in]   p_ble_detect       Pointer to Our Service structure.
 */
void ble_detect_init(ble_os_t * p_ble_detect);

/**@brief Function for updating and sending new characteristic values
 *
 * @details The application calls this function whenever our timer_timeout_handler triggers
 *
 * @param[in]   p_ble_detect            Our Service structure.
 * @param[in]   sensor_value     				New characteristic value.
 */
void sensor_characteristic_update(ble_os_t *p_ble_detect, uint32_t *sensor_value);
void door_characteristic_update(ble_os_t *p_ble_detect, uint32_t *sensor_value);

#endif  /* _ BLE_DETECT_H__ */
