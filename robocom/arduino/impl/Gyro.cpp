/* Gyro Helper routines.
 *
 * Dropbox Hack Week, August 2013.
 */

#include <MPU6050_6Axis_MotionApps20.h>

#include "../Gyro.hpp"

Gyro::Gyro()
  : m_mpu()
  , m_fifo_buffer()
  , m_fifo_count(0)
  , m_packet_size(0)
  , m_initialized(false) {
}


bool Gyro::initialize() {
  m_mpu.initialize();
  if (!m_mpu.testConnection()) {
    return false;
  }
  if (0 != m_mpu.dmpInitialize()) {
    return false;
  }
  m_mpu.setDMPEnabled(true);
  m_packet_size = m_mpu.dmpGetFIFOPacketSize();
  return true;
}

void Gyro::awaitFirstReading() {
  while (!m_has_reading) {
    updateReading();
  }
}

bool Gyro::updateReading() {
    // Get INT_STATUS byte
    const uint8_t mpu_int_status = m_mpu.getIntStatus();

    // get current FIFO count
    m_fifo_count = m_mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpu_int_status & 0x10) || m_fifo_count >= 1024) {
        // reset so we can continue cleanly
        m_mpu.resetFIFO();
        return false;
    }

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    if (0 == (mpu_int_status & MPU6050_INT_STATUS_DATA_READY)) {
      return false;
    }
    
    // wait for correct available data length, should be a VERY short wait
    while (m_fifo_count < m_packet_size) {
      m_fifo_count = m_mpu.getFIFOCount();
    }

    // read a packet from FIFO
    m_mpu.getFIFOBytes(m_fifo_buffer, m_packet_size);
    
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    m_fifo_count -= m_packet_size;

    m_mpu.dmpGetQuaternion(&m_latest.quaternian, m_fifo_buffer);
    m_mpu.dmpGetGravity(&m_latest.gravity, &m_latest.quaternian);
    m_mpu.dmpGetYawPitchRoll(m_latest.ypr, &m_latest.quaternian, &m_latest.gravity);
    
    m_has_reading = true;
    return true;
}

void Gyro::printReading(HardwareSerial& serial, const Reading& reading) {
  Serial.print("quat[");
  Serial.print(reading.quaternian.w);
  Serial.print("\t");
  Serial.print(reading.quaternian.x);
  Serial.print("\t");
  Serial.print(reading.quaternian.y);
  Serial.print("\t");
  Serial.print(reading.quaternian.z);
  Serial.print("], g[");
  Serial.print(reading.gravity.x);
  Serial.print("\t");
  Serial.print(reading.gravity.y);
  Serial.print("\t");
  Serial.print(reading.gravity.z);
  Serial.print("], ypr[");
  Serial.print(reading.ypr[0] * 180/M_PI);
  Serial.print("\t");
  Serial.print(reading.ypr[1] * 180/M_PI);
  Serial.print("\t");
  Serial.print(reading.ypr[2] * 180/M_PI);
  Serial.println("]");
}



// Gyro::Reading helper class ////////////////////////////////////////////////////////

Gyro::Reading::Reading()
  : quaternian()
  , gravity()
  , ypr() {
}


Gyro::Reading::Reading(const Gyro::Reading& other)
  : quaternian(other.quaternian)
  , gravity(other.gravity)
  , ypr() {
    ::memcpy(&ypr, &other.ypr, sizeof(ypr));
}


Gyro::Reading& Gyro::Reading::operator=(const Gyro::Reading& other) {
  if (this != &other) {
    quaternian = other.quaternian;
    gravity = other.gravity;
    ::memcpy(&ypr, &other.ypr, sizeof(ypr));
  }
  return *this;
}  
