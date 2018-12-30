#ifdef  ESP32
	#ifndef otaUpdate_h
		#define otaUpdate_h

		#include <IOTAppStory.h>
		#include <WiFiClientSecure.h>
		#include <StreamString.h>
		#include <Update.h>
											 
		#if NEXT_OTA == true
			#include <ESPNexUpload.h>
			#define U_NEXTION 300
		#endif
		
		#define U_FLASH   0
		#define U_SPIFFS  100
		#define U_AUTH    200
		

		
		struct strConfig;
		
		
		class otaUpdate{
			
			public:
				String error;
				otaUpdate(strConfig &config, int command = U_FLASH);
				bool getUpdate();
				bool install();
			
			private:
				WiFiClientSecure _client;
				String _xname,_xver,_xmd5;
				
				strConfig* _config;
				int _command;
				uint32_t _totalSize 			= 0;										// total size of received update
				const char* _updateHost		= "iotappstory.com";    // ota update host
				const char* _updateFile		= "/ota/esp32-v2.php"; 	// file at host that handles 8266 updates
			
				bool installESP();
				bool installNEXTION();
		};
	#endif
#endif