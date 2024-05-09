
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_MPU6050 mpu;

void MPU6050setup(void)
{

	// Try to initialize!
	if (!mpu.begin())
	{
		Serial.println("Conecte el giroscopio");
		display.setTextSize(2); // tamano del texto
		display.setTextColor(1);
		display.setCursor(0, 0);
		display.println("Conecte el");
		display.setCursor(0, 16);
		display.println("giroscopio");
		display.setCursor(0, 32);
		display.println("y reinicie");


		display.display();

		while (1)
		{
			delay(10);
		}
	}
	Serial.println("Giroscopio listo");

	display.clearDisplay();
	display.setTextSize(2); // tamano del texto
	display.setTextColor(2);
	display.setCursor(0, 0);
	display.print("Giroscopio listo");

	// setupt motion detection
	mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
	mpu.setMotionDetectionThreshold(1);
	mpu.setMotionDetectionDuration(20);
	mpu.setInterruptPinLatch(true); // Keep it latched.  Will turn off when reinitialized.
	mpu.setInterruptPinPolarity(true);
	mpu.setMotionInterrupt(true);
	delay(500);

	display.display();
	delay(2000);
	display.clearDisplay();
	display.display();
}

void MPU6050loop(void)
{

	if (mpu.getMotionInterruptStatus())
	{
		// Get new sensor events with the readings
		sensors_event_t a, g, temp;
		mpu.getEvent(&a, &g, &temp);

		//  Print out the values
		Serial.print("AccelX:");
		Serial.print(a.acceleration.x);
		Serial.print(",");
		Serial.print("AccelY:");
		Serial.print(a.acceleration.y);
		Serial.print(",");
		Serial.print("AccelZ:");
		Serial.print(a.acceleration.z);
		Serial.print(", ");
		Serial.print("GyroX:");
		Serial.print(g.gyro.x);
		Serial.print(",");
		Serial.print("GyroY:");
		Serial.print(g.gyro.y);
		Serial.print(",");
		Serial.print("GyroZ:");
		Serial.print(g.gyro.z);
		Serial.println("");
	}

	// delay(10);
}
