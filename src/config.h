	/**
		------ ------ ------ ------ ------ ------ user DEFINES for library ------ ------ ------ ------ ------ ------ 
	*/
	#include "serialFeedback_EN.h"					// language file for serial feedback currently available EN & NL
	
	#define DEBUG_LVL					2			// Debug level: 0 - 3 | none - max
	#define DEBUG_EEPROM_CONFIG			false
	#define DEBUG_FREE_HEAP				false
	#define SERIAL_SPEED				115200
	#define BOOTSTATISTICS				true
		

	// config 
	#define CFG_INCLUDE 				true  		// include Config mode (Wifimanager!!!)
	#define CFG_STORAGE					ST_CLOUD	// ST_CLOUD / ST_SPIFSS / ST_HYBRID
	#define CFG_AUTHENTICATE			false		// Set authentication
	#define CFG_PASS					"admin"		// initial authentication password. You edit & change this in config mode. | max 16 char
	#define CFG_PAGE_INFO				true		// include the info page in Config mode
	#define CFG_PAGE_IAS 				true  		// include the IAS page in Config mode
	

	// Wifi defines
	#define WIFI_SMARTCONFIG			false		// Set to true to enable smartconfig by smartphone app "ESP Smart Config" or "ESP8266 SmartConfig" | This will add (+/- 2%) of program storage space and +/- 1%) of dynamic memory
	#define WIFI_MULTI					true		// false: only 1 ssid & pass will be used | true: 3 sets of ssid & pass will be used
	#define WIFI_MULTI_FORCE_RECONN_ANY	false		// By default wifi multi will only try to reconnect to the last AP it was connectected to. Setting this to true will force your esp to connect to any of the available AP's from the list.
	#define WIFI_CONN_MAX_RETRIES 		20			// sets the maximum number of retries when trying to connect to the wifi
	#define WIFI_USE_MDNS 				true  		// include MDNS responder http://yourboard.local
	
	
	// Internal clock
	#if defined  ESP8266
	   #define SNTP_INT_CLOCK_UPD			true		// Synchronize the internal clock useing SNTP? BearSSL: This is necessary to verify that the TLS certificates offered by servers are currently valid.
	   #define SNTP_INT_CLOCK_UPD_INTERVAL	43200000	// Clock update internal in miliseconds (default 43200000 = 12 hour)
	#elif defined ESP32
	   #define SNTP_INT_CLOCK_UPD			false		// The esp32 uses mbedTLS instead of BearSSL and does not need the time. Your welcome to turn it on for your own projects!
	   #define SNTP_INT_CLOCK_UPD_INTERVAL	43200000	// Clock update internal in miliseconds (default 43200000 = 12 hour)
	#endif
	
	
	// HTTPS defines
	#define HTTPS         				true		// Use HTTPS for OTA updates
	#define HTTPS_8266_TYPE				CERTIFICATE	// FNGPRINT / CERTIFICATE
	#define HTTPS_CERT_STORAGE			ST_SPIFFS	// ST_SPIFFS / ST_PROGMEM
	#define HTTPS_FNGPRINT				"34:6D:0A:26:F0:40:3A:0A:1B:F1:CA:8E:C8:0C:F5:14:21:83:7C:B1" // Initial fingerprint. You can edit & change later in config mode.
	
	
	// OTA defines
	#define OTA_HOST 					"iotappstory.com"   // OTA update host
	#define OTA_UPD_FILE 				"/ota/updates.php" 	// file at host that handles 8266 updates
	#define OTA_LOG_FILE 				"/ota/logs.php" 	// file at host that handles 8266 updates
	#define OTA_UPD_CHECK_SPIFFS		true				// Do you want to OTA update SPIFFS? | true / false
	#define OTA_UPD_CHECK_NEXTION		false				// Do you want to OTA update your Nextion display? | true / false

	
	// Nextion display defines
	#if defined  ESP8266
		#define NEXT_RES				5		// Nextion reset pin | Default 5 / D1 | Use this pin to control a transistor or relay to "hard" reset your display(power) after updates
		#define NEXT_RX					14		// Nextion RX pin | Default 14 / D5
		#define NEXT_TX					12		// Nextion TX pin | Default 12 / D6
		#define NEXT_BAUD				115200	// Nextion baudrate | 115200 / 57600
	#elif defined ESP32
		#define NEXT_RES				5		// Nextion reset pin | Default 5 / D1 | Use this pin to control a transistor or relay to "hard" reset your display(power) after updates
		#define NEXT_RX					16		// Nextion RX pin | Default 14 / D5
		#define NEXT_TX					17		// Nextion TX pin | Default 12 / D6
		#define NEXT_BAUD				115200	// Nextion baudrate | 115200 / 57600
	#endif
 
 
	// EERPOM & max nr of addable fields
	#if defined  ESP8266
		#define EEPROM_SIZE 			4096	// EEPROM_SIZE depending on device
		#define MAXNUMEXTRAFIELDS 		8		// wifimanger | max num of fields that can be added
	#elif defined ESP32
		#define EEPROM_SIZE		 		1984
		#define MAXNUMEXTRAFIELDS 		12
	#else
		#define EEPROM_SIZE 			1024
		#define MAXNUMEXTRAFIELDS 		8
	#endif
	
	
	/**
		------ ------ ------ ------ ------ ------ internal DEFINES for library ------ ------ ------ ------ ------ ------ 
	*/

	
	// length of config variables
	#define STRUCT_CHAR_ARRAY_SIZE  	50
	#define STRUCT_PASSWORD_SIZE		64		// Thankyou reibuehl
	#define STRUCT_COMPDATE_SIZE    	20
	#define STRUCT_BNAME_SIZE       	30
	#define STRUCT_HOST_SIZE        	24
	#define STRUCT_FILE_SIZE        	31

	
	// constants used to define the status of the mode button based on the time it was pressed. (miliseconds)
	#define MODE_BUTTON_SHORT_PRESS       500
	#define MODE_BUTTON_LONG_PRESS        4000
	#define MODE_BUTTON_VERY_LONG_PRESS   10000
	
	
	// define storage types (internal use)
	#define ST_SPIFFS	0
	#define ST_SD		1
	#define ST_PROGMEM	2
	#define ST_CLOUD	3
	#define ST_HYBRID	4
	
	
	#define FNGPRINT	0
	#define CERTIFICATE	1
	
	
	// used for storing the config struct to eeprom
	#define MAGICBYTES    				"CFG"
	#define MAGICEEP      				"%"
	
	
	// define for get chip id
	#if defined  ESP8266
		#define ESP_GETCHIPID ESP.getChipId()
	#elif defined ESP32
		#define ESP_GETCHIPID (uint32_t)ESP.getEfuseMac()
	#endif
