#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2561_U.h"
#include "Adafruit_TSL2591.h"
#include "Adafruit_SI1145.h"

Adafruit_TSL2561_Unified tsl2561 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_TSL2591 tsl2591 = Adafruit_TSL2591(2591);
Adafruit_SI1145 si1145 = Adafruit_SI1145();

void setup()
{
	Serial.begin(115200);
	if (!tsl2561.begin()) {
		Serial.println("no TSL2561");
	}
	if (!tsl2591.begin()) {
		Serial.println("no TSL2591");
	}
	if (!si1145.begin()) {
		Serial.println("no SI1145");
	}
	tsl2561.enableAutoRange(true);
	//tsl2561.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
	tsl2591.setGain(TSL2591_GAIN_LOW);
	tsl2591.setTiming(TSL2591_INTEGRATIONTIME_500MS);

	si1145.setVisibleGain(Adafruit_SI1145::Gain::ADC_7);
	si1145.setIRGain(Adafruit_SI1145::Gain::ADC_0);
}

float lux2ev(float lux)
{
	return (log(lux) - log(2.5)) / log(2);
}

void loop()
{
	uint16_t ir, full;
	tsl2561.getLuminosity(&full, &ir);

	Serial.print("TSL2561 IR: "); Serial.print(ir);  Serial.print("  ");
	Serial.print("Full: "); Serial.print(full); Serial.print("  ");
	float lux = tsl2561.calculateLux(full, ir);
	Serial.print("Lux: "); Serial.print(lux); Serial.print("  ");
	Serial.print("EV: "); Serial.println(lux2ev(lux));

	uint32_t lum = tsl2591.getFullLuminosity();
	ir = lum >> 16;
	full = lum & 0xFFFF;
	Serial.print("TSL2591 IR: "); Serial.print(ir);  Serial.print("  ");
	Serial.print("Full: "); Serial.print(full); Serial.print("  ");
	lux = tsl2591.calculateLux(full, ir);
	Serial.print("Lux: "); Serial.print(lux, 6); Serial.print("  ");
	Serial.print("EV: "); Serial.println(lux2ev(lux));

	full = si1145.readVisible();
	ir = si1145.readIR();
	Serial.print("SI1145 IR: "); Serial.print(ir);  Serial.print("  ");
	Serial.print("Full: "); Serial.print(full); Serial.print("  ");
	lux = si1145.calculateLux(full, ir);
	Serial.print("Lux: "); Serial.print(lux, 6); Serial.print("  ");
	Serial.print("EV: "); Serial.println(lux2ev(lux));
	Serial.println("");

	
	delay(1500);
}
