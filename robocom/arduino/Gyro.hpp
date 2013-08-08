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
public:
  /**
   * Represents a single reading of all data from the gyro.
   */
  class Reading {
  public:
    Quaternion quaternian;  // [w, x, y, z]         quaternion container
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    
    Reading();
    Reading(const Reading& other);
    Reading& operator=(const Reading& other);
  };

  /**
   * Constructor
   */
  Gyro();
  
  /**
   * Initialize the gyro.  Should be called in setup()
   *
   * @return true on success, false if something failed
   */
  bool initialize();
  
  /**
   * Wait for the first reading to be available, and update it.
   * This method has no effect if the first reading was arleady taken.
   * This should from loop().  It hangs if called from setup().
   */
  void awaitFirstReading();
  
  /**
   * Checks for new input and updates the latest Gyro reading.
   *
   * @return true if the reading changed
   */
  bool updateReading();
  
  /**
   * Gets the latest reading.
   */
  const Reading& getLatestReading() const {
    return m_latest;
  }
  
  /**
   * Returns whether or not this Gyro has been successfully initialized.
   */
  bool isInitialized() const {
    return m_initialized;
  }
  
  /**
   * Returns whether or not this Gyro has been successfully initialized.
   */
  bool hasReading() const {
    return m_has_reading;
  }

  /**
   * Prints out the given Gyro reading, via serial.
   */
  static void printReading(HardwareSerial& serial, const Reading& reading);

private:
  Gyro(const Gyro&);
  Gyro& operator=(const Gyro&);
  
  Reading m_latest;
  
  MPU6050 m_mpu;
  uint8_t m_fifo_buffer[64]; // FIFO storage buffer
  uint16_t m_fifo_count;
  uint16_t m_packet_size;
  
  bool m_initialized;
  bool m_has_reading;
};

#endif // GYRO_HPP
