/* MPU6050 Gyro Helper routines.
 *
 * Dropbox Hack Week, August 2013.
 */
#ifndef GYRO_HPP
#define GYRO_HPP

#include <I2Cdev.h>

#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
#include <helper_3dmath.h>
#include <MPU6050.h>

/**
 * Helper class to encapsulate gyro functionality.
 */
class Gyro {
	static const int MPU6050_ADDRESS = 0x68;
	static const uint8_t MPU6050_INT_STATUS_DATA_READY = 0x02;
	static const uint8_t MPU6050_INT_STATUS_FIFO_OVERFLOW = 0x10;
public:
	/**
	 * Represents a single reading of all data from the gyro.
	 */
	class Reading {
	public:
		Quaternion quaternian;  // [w, x, y, z]         quaternion container
		VectorFloat gravity;    // [x, y, z]            gravity vector
		float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
		unsigned long millis;   // time in ms when the reading was taken
		unsigned long micros;   // time in us when the reading was taken
    
		Reading () throw ();
		explicit Reading (const Reading& other) throw ();
		Reading& operator= (const Reading& other) throw ();

		float getYawDegrees() const throw () { return ypr[0] * 180.0f/M_PI; }
		float getPitchDegrees() const throw () { return ypr[1] * 180.0f/M_PI; }
		float getRollDegrees() const throw () { return ypr[2] * 180.0f/M_PI; }
	};

	/**
	 * Constructor
	 */
	Gyro () throw ();
  
	/**
	 * Returns whether or not this Gyro has been successfully initialized.
	 */
	bool isInitialized () const throw () {
		return m_initialized;
	}
  
	/**
	 * Returns whether or not this Gyro has been successfully initialized.
	 */
	bool hasReading () const throw () {
		return m_has_reading;
	}

	/**
	 * Initialize the gyro.  Should be called in setup()
	 *
	 * @return true on success, false if something failed
	 */
	bool initialize () throw ();
  
	/**
	 * Wait for the first reading to be available, and update it.
	 * This method has no effect if the first reading was arleady taken.
	 * This should from loop().  It hangs if called from setup().
	 */
	void awaitFirstReading () throw ();
  
	/**
	 * Checks for new input and updates the latest Gyro reading.
	 *
	 * @return true if the reading changed
	 */
	bool updateReading () throw ();
  
	/**
	 * Gets the latest reading.
	 */
	const Reading& getLatestReading () const throw () {
		return m_latest;
	}
  
	/**
	 * Prints out the given Gyro reading, via serial.
	 */
	static void printReading(HardwareSerial& serial, const Reading& reading) throw ();

	void setSubscriber (uint16_t task_id, uint32_t min_delay_millis) throw ();

	void clearSubscriber () throw ();

	bool shouldReport () const throw ();
	void setReported () throw ();

	uint16_t getSubscriberTaskId () const throw () {
		return m_subscriber_task_id;
	}

private:
	Gyro(const Gyro&) throw ();
	Gyro& operator=(const Gyro&) throw ();
  
	Reading m_latest;
  
	MPU6050 m_mpu;
	uint8_t m_fifo_buffer[64]; // FIFO storage buffer
	uint16_t m_fifo_count;
	uint16_t m_packet_size;

	uint16_t m_subscriber_task_id;
	unsigned long m_min_report_delay_millis;
	unsigned long m_last_report_millis;
	
	bool m_initialized;
	bool m_has_reading;
	bool m_has_subscriber;
};

#endif // GYRO_HPP
