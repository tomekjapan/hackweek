/* Gyro Helper routines.
 *
 * Dropbox Hack Week, August 2013.
 */

#include <MPU6050_6Axis_MotionApps20.h>

#include "../Gyro.hpp"

Gyro::Gyro() throw ()
	: m_mpu()
	, m_fifo_buffer()
	, m_fifo_count(0)
	, m_packet_size(0)
	, m_subscriber_task_id(0)
	, m_min_report_delay_millis(0)
	, m_last_report_millis(0)
	, m_initialized(false)
	, m_has_reading(false)
    , m_has_subscriber(false) {
}


bool Gyro::initialize() throw () {
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

void Gyro::awaitFirstReading() throw () {
	while (!m_has_reading) {
		updateReading();
	}
}

bool Gyro::updateReading() throw () {
    // Get INT_STATUS byte
    const uint8_t mpu_int_status = m_mpu.getIntStatus();

#if(1) // ART_DBG
    // get current FIFO count
    m_fifo_count = m_mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpu_int_status & MPU6050_INT_STATUS_FIFO_OVERFLOW) || m_fifo_count >= 1024) {
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
    m_latest.micros = ::micros();
    m_latest.millis = ::millis();
	
    m_has_reading = true;
    return true;
#else
    m_has_reading = true;
	return true;
#endif
}

void Gyro::printReading(HardwareSerial& serial, const Reading& reading) throw () {
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


void
Gyro::setSubscriber(uint16_t task_id, uint32_t min_delay_millis) throw () {
	m_subscriber_task_id = task_id;
	m_min_report_delay_millis = min_delay_millis;
	m_has_subscriber = true;
}


void
Gyro::clearSubscriber() throw () {
	m_has_subscriber = false;
}


bool
Gyro::shouldReport() const throw () {
	if (!m_has_subscriber) {
		return false;
	}
	return m_latest.millis >= m_last_report_millis + m_min_report_delay_millis;
}

void
Gyro::setReported() throw () {
	m_last_report_millis = m_latest.millis;
}

// Gyro::Reading helper class ////////////////////////////////////////////////////////

Gyro::Reading::Reading() throw ()
	: quaternian()
	, gravity()
	, ypr()
	, millis(0)
	, micros(0) {
}


Gyro::Reading::Reading(const Gyro::Reading& other) throw ()
	: quaternian(other.quaternian)
	, gravity(other.gravity)
	, ypr()
	, millis(other.millis)
	, micros(other.micros) {
	::memcpy(&ypr, &other.ypr, sizeof(ypr));
}


Gyro::Reading& Gyro::Reading::operator=(const Gyro::Reading& other) throw () {
	if (this != &other) {
		quaternian = other.quaternian;
		gravity = other.gravity;
		::memcpy(&ypr, &other.ypr, sizeof(ypr));
		millis = other.millis;
		micros = other.micros;
	}
	return *this;
}  
