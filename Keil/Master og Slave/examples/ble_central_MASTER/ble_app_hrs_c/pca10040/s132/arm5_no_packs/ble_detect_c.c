#include <stdint.h>
#include <string.h>
#include "ble_detect_c.h"
#include "nrf_gpio.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "SEGGER_RTT.h"

/** Noen ganger er et vanskelig å få includa .h filen her og i main.
		Gå inn i "Options for this target" og inn i C/C++ fanen og så inn i "include paths"
		i bunnen der skriver du stien til ble_detect_c.h filen. Denne ligger i mappa til prosjektet
		Da må du høyreklikke på den og properties, også kopierer du stien til den filen og limer den inn
		i bunnen av include paths.
		
		Er også lurt å legge til "C:\Keil_v5\ARM\Pack\ARM\CMSIS\4.2.0\CMSIS\Include".
*/

