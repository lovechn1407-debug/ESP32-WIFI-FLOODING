#include "spammer.hpp"

// List of SSIDs stored in flash memory to save RAM
// Max 32 characters per SSID, separated by \n
const char targetSSIDs[] PROGMEM = {
  "WIFI FLOODING STARTED 01\n"
  "WIFI FLOODING STARTED 02\n"
  "WIFI FLOODING STARTED 03\n"
  "WIFI FLOODING STARTED 04\n"
  "WIFI FLOODING STARTED 05\n"
  "WIFI FLOODING STARTED 06\n"
  "WIFI FLOODING STARTED 07\n"
  "WIFI FLOODING STARTED 08\n"
  "WIFI FLOODING STARTED 09\n"
  "WIFI FLOODING STARTED 10\n"
  "WIFI FLOODING STARTED 11\n"
  "WIFI FLOODING STARTED 12\n"
  "WIFI FLOODING STARTED 13\n"
  "WIFI FLOODING STARTED 14\n"
  "WIFI FLOODING STARTED 15\n"
  "WIFI FLOODING STARTED 16\n"
  "WIFI FLOODING STARTED 17\n"
  "WIFI FLOODING STARTED 18\n"
  "WIFI FLOODING STARTED 19\n"
  "WIFI FLOODING STARTED 20\n"
};

// Standard Beacon Frame definition (109 bytes)
uint8_t spammer::_beaconPacket[109] = {
  /* 0 - 3  */ 0x80, 0x00, 0x00, 0x00,             // Type/Subtype: Management beacon frame
  /* 4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination: Broadcast
  /* 10 - 15 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source MAC Address
  /* 16 - 21 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // BSSID
  /* 22 - 23 */ 0x00, 0x00,                         // Sequence number (handled by SDK)
  /* 24 - 31 */ 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, // Timestamp
  /* 32 - 33 */ 0xe8, 0x03,                         // Beacon Interval (approx 1s)
  /* 34 - 35 */ 0x31, 0x00,                         // Capabilities Information
  /* 36 - 37 */ 0x00, 0x20,                         // Tag SSID: length 32
  /* 38 - 69 */ 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, // Blank space for SSID
  /* 70 - 71 */ 0x01, 0x08,                         // Supported Rates Tag
  /* 72 - 79 */ 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, // Transmission rates
  /* 80 - 82 */ 0x03, 0x01, 0x01,                   // Current Channel Tag
  /* 83 - 108*/ 0x30, 0x18, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, // RSN Info
                0x02, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x00, 0x0f, 
                0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 
                0x00, 0x00 
};

spammer::spammer(bool useWpa2) {
    _useWpa2 = useWpa2;
    _channelIndex = 0;
    _currentWifiChannel = 1;
    _lastAttackTime = 0;
    _lastPacketRateTime = 0;
    _packetCounter = 0;
    _packetSize = sizeof(_beaconPacket);
    _channels[0] = 1; // Default to channel 1 initially
}

void spammer::begin() {
    // Seed the random generator
    randomSeed(analogRead(0) + 1);
    
    // Choose a random starting channel between 1 and 14
    _channels[0] = random(1, 14);

    // Initialize the empty SSID buffer with spaces
    for (int i = 0; i < 32; i++) {
        _emptySsidBuffer[i] = ' ';
    }

    // Adjust packet capabilities based on WPA2 setting
    if (_useWpa2) {
        _beaconPacket[34] = 0x31;
    } else {
        _beaconPacket[34] = 0x21;
        _packetSize -= 26; // Remove the RSN information from the end
    }

    generateRandomMac();

    // Set WiFi to Station mode
    WiFi.mode(WIFI_MODE_STA);
    
    // Set the initial WiFi channel
    esp_wifi_set_channel(_channels[0], WIFI_SECOND_CHAN_NONE);
}

void spammer::switchToNextChannel() {
    if (sizeof(_channels) < 2) return;

    uint8_t ch = _channels[_channelIndex];

    _channelIndex++;
    if (_channelIndex >= sizeof(_channels)) {
        _channelIndex = 0;
    }

    if (ch != _currentWifiChannel && ch >= 1 && ch <= 14) {
        _currentWifiChannel = ch;
        esp_wifi_set_channel(_currentWifiChannel, WIFI_SECOND_CHAN_NONE);
    }
}

void spammer::generateRandomMac() {
    for (int i = 0; i < 6; i++) {
        _macAddress[i] = random(256);
    }
}

void spammer::update() {
    uint32_t currentTime = millis();

    // Spam SSIDs every 100ms
    if (currentTime - _lastAttackTime > 100) {
        _lastAttackTime = currentTime;

        int i = 0;
        int j = 0;
        int currentSsidNumber = 1;
        char tempChar;
        int totalSsidsLength = strlen_P(targetSSIDs);

        switchToNextChannel();

        // Iterate over all SSIDs
        while (i < totalSsidsLength) {
            j = 0;
            do {
                tempChar = pgm_read_byte(targetSSIDs + i + j);
                j++;
            } while (tempChar != '\n' && j <= 32 && i + j < totalSsidsLength);

            uint8_t ssidLength = j - 1;

            // Incrementally change MAC address for each SSID
            _macAddress[5] = currentSsidNumber;
            currentSsidNumber++;

            // Inject the MAC address into the beacon frame (Source & BSSID)
            memcpy(&_beaconPacket[10], _macAddress, 6);
            memcpy(&_beaconPacket[16], _macAddress, 6);

            // Clear the SSID field
            memcpy(&_beaconPacket[38], _emptySsidBuffer, 32);

            // Copy the new SSID into the beacon frame
            memcpy_P(&_beaconPacket[38], &targetSSIDs[i], ssidLength);

            // Update the SSID length field in the beacon frame (byte 37)
            _beaconPacket[37] = ssidLength;

            // Set the appropriate channel in the beacon frame
            _beaconPacket[82] = _currentWifiChannel;

            // Transmit the packet 3 times for better reliability
            for (int k = 0; k < 3; k++) {
                bool isSuccess = (esp_wifi_80211_tx(WIFI_IF_STA, _beaconPacket, _packetSize, false) == 0);
                if (isSuccess) {
                    _packetCounter++;
                }
                delay(2);
            }

            i += j; // Move to the next SSID string
        }
    }

    // Print transmission rate every 3 seconds
    if (currentTime - _lastPacketRateTime > 3000) {
        _lastPacketRateTime = currentTime;
        Serial.print(_packetCounter);
        Serial.println(" packets/s");
        _packetCounter = 0;
    }
}
