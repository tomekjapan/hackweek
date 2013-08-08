/* Helper routines for logo turtle command execution.
 *
 * Dropbox Hack Week, August 2013.
 */
#ifndef LOGO_COMMANDS_HPP
#define LOGO_COMMANDS_HPP

#include "Gyro.hpp"

/**
 * Helper class for detecting a turn by a fixed amount.
 */
class LogoTurn {
  const Gyro * m_p_gyro;
  float m_start_yaw;
  float m_target_angle;
public:

  /**
   * Default constructor for an uninitialized object.
   * It will always return an angle of 0, and indicate it is done.
   */
  LogoTurn()
    : m_p_gyro(0)
    , m_start_yaw(0.0f)
    , m_target_angle(0.0f) {
  }

  /**
   * Starts a turn by the given angle.
   */
  static LogoTurn start(const Gyro& gyro, float target_angle) {
    return LogoTurn(gyro, target_angle);
  }

  /**
   * Returns whether the turn is finished.
   */  
  bool isDone() {
    return (m_target_angle >= 0) ? (getAngle() >= m_target_angle) : (getAngle() <= m_target_angle);
  }
  
  /**
   * Returns the current turn angle.
   */
  float getAngle() {
    if (!m_p_gyro) {
      return 0.0f;
    }
    float angle = m_p_gyro->getLatestReading().ypr[0] - m_start_yaw;
    if (angle > maxAngle()) {
      angle -= 2*M_PI;
    } else if (angle < minAngle()) {
      angle += 2*M_PI;
    }
    return angle;
  }
    
  float maxAngle() { return m_target_angle >= 0 ? (3.0f*M_PI/2.0f) : (M_PI/2.0f); }
  float minAngle() { return m_target_angle >= 0 ? (-M_PI/2.0f) : (-3.0f*M_PI/2.0f); }
private:
  
  LogoTurn(const Gyro& gyro, float target_angle)
    : m_p_gyro(&gyro)
    , m_start_yaw(gyro.getLatestReading().ypr[0])
    , m_target_angle(target_angle) {
  }
};

#endif // LOGO_COMMANDS_HPP
