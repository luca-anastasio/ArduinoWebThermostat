/********************************************************************************
 * Arduino Ethernet Thermostat													*
 * Copyright (C) 2015															*
 * Luca Anastasio <anastasio.lu@gmail.com>										*
 *																				*
 * This program is free software; you can redistribute it and/or modify			*
 * it under the terms of the GNU General Public License as published by			*
 * the Free Software Foundation; either version 3 of the License, or			*
 * (at your option) any later version.											*
 *																				*
 * This program is distributed in the hope that it will be useful,				*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of				*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the					*
 * GNU General Public License for more details.									*
 *																				*
 * You should have received a copy of the GNU General Public License			*
 * along with this program; if not, write to the Free Software					*
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA *
 ********************************************************************************/
#ifndef configuration_h
#define configuration_h

//-------- Libraries
#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "TimeLib.h"
#include "SPI.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#include "EEPROM.h"

//----------- Pins and Constants Definitions -----------
const uint8_t ONE_WIRE_BUS_PIN = 9;		// pin connected to the temperature probes
const uint8_t ZONE_0_PIN = 2;			// pin connected to the relay
const uint8_t ZONE_1_PIN = 3;
const uint8_t ZONE_2_PIN = 5;
const uint8_t ZONE_3_PIN = 6;
const uint8_t HEATER_PIN = 7;
const uint8_t COOLER_PIN = 8;
const uint8_t THERM_INCREMENTS = 2;		// 0.50 °C, the increment in set temperature when clicking on + or -
const uint8_t eeprom_address_0 = 0;
const uint8_t eeprom_address_1 = eeprom_address_0 +22;
const uint8_t eeprom_address_2 = eeprom_address_1 +22;
const uint8_t eeprom_address_3 = eeprom_address_2 +22;

//------------------------------- temperature probes addresses -----------------------------
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature *temp_sensors = new DallasTemperature(&oneWire);				// poiter to the temperature sensors object, 
const uint8_t probe_0[8] = { 0x28, 0xFF, 0xC8, 0x92, 0x00, 0x16, 0x02, 0x48 };		// to be passed in to the thermostat objects, requires OneWire object
const uint8_t probe_1[8] = { 0x28, 0xFF, 0xC7, 0x69, 0x00, 0x16, 0x01, 0x20 };		// use an address finder sketch to find each probe address
const uint8_t probe_2[8] = { 0x28, 0xFF, 0xAC, 0x7D, 0x00, 0x16, 0x01, 0x77 };
const uint8_t probe_3[8] = { 0x28, 0xFF, 0x39, 0x7E, 0x00, 0x16, 0x01, 0x9A };

//----------------- IP settings, time zone, NTP -------------------------------------
static uint8_t arduino_mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };		// set the ethernet shield MAC address here
static uint8_t arduino_ip[] = { 10, 42, 0, 127 };							// set the static IP address here
static char ntp_server_address[] = "ntp.ien.it";							// NTP server address
const int8_t time_zone = 2;
const uint8_t ntp_port = 123;

//--------------------------------- Webserver settings ----------------------------------------------------
#define WEBDUINO_AUTH_REALM ""		// message shown at the web authentication prompt
#define WEBDUINO_FAVICON_DATA ""
#include "WebServer.h"
#define WEBDUINO_CREDENTIALS "YWRtaW46YWRtaW4="		// admin:admin username and password pair, encoded in base64
#define WEBDUINO_PREFIX "/thermostat"				// the page will be available at: http://192.168.0.127/thermostat
#define LISTENING_PORT 80
const int NTP_PACKET_SIZE = 48;
EthernetUDP udp_conn;

//---------------------------------- Thermostat settings --------------------------------------------------------------------
#define THERMOSTAT_RESOLUTION 10	// 9=0.5°C, 10=0.25°C, 11=0.125°C, 12=1/16°C resolution of the temperature probes readings, default to 9
#define THERMOSTAT_RANGE 1			// 0.50 °C, the range around the actual temperature in which the thermostat will be acting, default to 1
#include "thermostat.h"
thermostat *zone_0 = new thermostat(ZONE_0_PIN, temp_sensors, probe_0, eeprom_address_0);		// pin connected to the relay, temperature sensors object pointer, 
thermostat *zone_1 = new thermostat(ZONE_1_PIN, temp_sensors, probe_1, eeprom_address_1);		// probe address, desired reading resolution, activation temperature range.
thermostat *zone_2 = new thermostat(ZONE_2_PIN, temp_sensors, probe_2, eeprom_address_2);		// e.g. when measured temp goes under (set_temp - range) the thermostat will be activated,
thermostat *zone_3 = new thermostat(ZONE_3_PIN, temp_sensors, probe_3, eeprom_address_3);		// when measured temp goes over (set_temp + range) the thermostat will be deactivated
const uint8_t num_thermostats = 4;
thermostat *thermostats[num_thermostats] = { zone_0, zone_1, zone_2, zone_3 };
#define CHECK_INTERVAL 2000
#define HOUR_RESET_INTERVAL 86400000
#define WEEKDAY_RESET_INTERVAL 60000

//----------------------------------------------- Language, Font settings ----------------------------------------------------
//#define ENGLISH
#define ITALIAN
P(span_start) = "<span style=\"font-family: verdana; font-size: 16px; color: #000000;\">";


#endif
