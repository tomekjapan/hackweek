// External component includes
#include "robocom/shared/msg/EncoderReadingNotice.hpp"
#include "robocom/shared/msg/EncoderReadingRequest.hpp"
#include "robocom/shared/msg/GyroReadingNotice.hpp"
#include "robocom/shared/msg/GyroReadingRequest.hpp"
#include "robocom/shared/msg/SetWheelDriveRequest.hpp"
#include "robocom/shared/msg/SetServoAngleRequest.hpp"
#include "robocom/shared/msg/SimpleMessage.hxx"
#include "robocom/shared/msg/WheelDriveChangedNotice.hpp"
#include "robocom/shared/msg/LogoCompleteNotice.hpp"
#include "robocom/shared/msg/LogoMoveRequest.hpp"
#include "robocom/shared/msg/LogoPenRequest.hpp"
#include "robocom/shared/msg/LogoTurnRequest.hpp"

// Component includes
#include "../LogoCommands.hpp"

// Module include
#include "../RobotServer.hpp"


using namespace robocom::shared;
using namespace robocom::shared::msg;


RobotServer::RobotServer (StreamIO& stream) throw ()
	: Server( stream )
	, m_motor_1( MOTOR_1_DIR_PIN, MOTOR_1_SIGNAL_PIN )
	, m_motor_2( MOTOR_2_DIR_PIN, MOTOR_2_SIGNAL_PIN )
	, m_encoder_1( ENCODER_1_PIN )
	, m_encoder_2( ENCODER_2_PIN )
	, m_gyro()
	, m_servo( SERVO_PIN, 90 /*base angle*/ )
	, m_p_logo_command(0)
    , m_logo_turn( m_gyro, m_motor_1, m_motor_2 )
	, m_logo_move( m_gyro, m_motor_1, m_motor_2, m_encoder_1, m_encoder_2 )
	, m_logo_pen( m_servo )
{
}


void
RobotServer::setup () throw ()
{
	m_motor_1.setup();
	m_motor_2.setup();

	m_encoder_1.setup();
	m_encoder_2.setup();

	m_servo.setup();

	m_gyro.initialize(); // TODO: Error check?
}


void
RobotServer::handleReset (const ResetRequest& req) throw ()
{
	m_encoder_1.clearSubscriber();
	m_encoder_2.clearSubscriber();
	m_gyro.clearSubscriber();

	m_servo.setBase();

	_clearLogoCommand();

	_setWheelDrive( 0, 0, 0, 0 );
	_notifyWheelDriveChanged( req.getTaskId() );
}


void
RobotServer::handleMessage (const Message& msg) throw ()
{
	switch ( msg.getMessageType() )
	{
	case SetWheelDriveRequest::MSGID:
		_processMessage( SetWheelDriveRequest( msg ) );
		break;
	case SetServoAngleRequest::MSGID:
		_processMessage( SetServoAngleRequest( msg ) );
		break;
	case EncoderReadingRequest::MSGID:
		_processMessage( EncoderReadingRequest( msg ) );
		break;
	case GyroReadingRequest::MSGID:
		_processMessage( GyroReadingRequest( msg ) );
		break;
	}
}


void
RobotServer::handleStateUpdate () throw ()
{
	if ( m_encoder_1.update() ) {
		_notifyEncoderReading( m_encoder_1 );
	}

	if ( m_encoder_2.update() ) {
		_notifyEncoderReading( m_encoder_2 );
	}

	m_gyro.awaitFirstReading();
	if (m_gyro.updateReading()) {
		if (m_gyro.shouldReport()) {
			_notifyGyroReading( m_gyro );
			m_gyro.setReported();
		}
    }

	if ( _hasLogoCommand() && _getLogoCommand().update() )
	{
		_notifyWheelDriveChanged( _getLogoCommand().getTaskId() );
		_notifyLogoComplete( _getLogoCommand().getTaskId(), STATUS_OK );
		_clearLogoCommand();
	}
}


void
RobotServer::_processMessage (const SetWheelDriveRequest& req) throw ()
{
	if ( STATUS_OK != req.validate() )
	{
		NCR_UNEXPECTED( "invalid SetWheelDriveRequest");
		return;
	}

	_setWheelDrive(
		req.getMotor1Direction(),
		req.getMotor1Signal(),
		req.getMotor2Direction(),
		req.getMotor2Signal()
	);

	_notifyWheelDriveChanged( req.getTaskId() );
}


void
RobotServer::_processMessage (const SetServoAngleRequest& req) throw ()
{
	if ( STATUS_OK != req.validate() )
	{
		NCR_UNEXPECTED( "invalid SetServoAngleRequest");
		return;
	}

	_setServoAngle( req.getServoId(), req.getAngle() );
}


void
RobotServer::_processMessage (const EncoderReadingRequest& req)
{
	if ( STATUS_OK != req.validate() )
	{
		NCR_UNEXPECTED( "invalid EncoderReadingRequest");
		return;
	}

	Encoder& encoder = req.getEncoderId() == m_encoder_1.getId()
		? m_encoder_1
		: m_encoder_2;

	if ( ! req.getIsSubscribe() ) {
		encoder.clearSubscriber();
	}
	else {
		encoder.setSubscriber( req.getTaskId() );
	}
}


void
RobotServer::_processMessage (const LogoTurnRequest& req) throw ()
{
	if ( STATUS_OK != req.validate() )
	{
		_notifyLogoComplete( req.getTaskId(), req.validate() );
		return;
	}

	if ( _hasLogoCommand() )
	{
		_notifyLogoComplete( req.getTaskId(), STATUS_E_LOGO_ACTIVE );
		return;
	}

	const float angle = req.getDirection() == 0
		? req.getAngle()
		: -req.getAngle();

	if ( m_logo_turn.start( req.getTaskId(), angle ) )
	{
		_setLogoCommand( m_logo_turn );
		_notifyWheelDriveChanged( req.getTaskId() );
	}
}


void
RobotServer::_processMessage (const LogoMoveRequest& req) throw ()
{
	if ( STATUS_OK != req.validate() )
	{
		_notifyLogoComplete( req.getTaskId(), req.validate() );
		return;
	}

	if ( _hasLogoCommand() )
	{
		_notifyLogoComplete( req.getTaskId(), STATUS_E_LOGO_ACTIVE );
		return;
	}

	if ( m_logo_move.start(
			 req.getTaskId(), req.getDirection(), req.getDistance() ) )
	{
		_setLogoCommand( m_logo_move );
		_notifyWheelDriveChanged( req.getTaskId() );
	}
}


void
RobotServer::_processMessage (const LogoPenRequest& req) throw ()
{
	if ( STATUS_OK != req.validate() )
	{
		_notifyLogoComplete( req.getTaskId(), req.validate() );
		return;
	}

	if ( _hasLogoCommand() )
	{
		_notifyLogoComplete( req.getTaskId(), STATUS_E_LOGO_ACTIVE );
		return;
	}

	if ( m_logo_pen.start( req.getTaskId(), req.getDirection() ) ) {
		_setLogoCommand( m_logo_pen );
	}
}


void
RobotServer::_processMessage (const GyroReadingRequest& req)
{
	if ( STATUS_OK != req.validate() )
	{
		NCR_UNEXPECTED( "invalid GyroReadingRequest");
		return;
	}

	if ( ! req.getIsSubscribe() ) {
		m_gyro.clearSubscriber();
	}
	else {
		m_gyro.setSubscriber( req.getTaskId(), req.getMinDelayMillis() );
	}
}


void
RobotServer::_notifyWheelDriveChanged (UInt16 task_id) throw ()
{
	addResponse(
		WheelDriveChangedNotice(
			task_id,
			getMillis(),
			m_motor_1.getDirection(),
			m_motor_1.getSignal(),
			m_motor_2.getDirection(),
			m_motor_2.getSignal()
		).asMessage()
	);
}


void
RobotServer::_notifyEncoderReading (const Encoder& encoder) throw ()
{
	// We only get here if the client subscribed to the encoder readings

	addResponse(
		EncoderReadingNotice(
			encoder.getSubscriber(),
			getMillis(),
			encoder.getId(),
			encoder.getTotal(),
			encoder.getMicros()
		).asMessage()
	);
}


void
RobotServer::_notifyLogoComplete (
	UInt16 task_id,
	UInt8 completion_status
) throw ()
{
	addResponse(
		LogoCompleteNotice(
			task_id,
			getMillis(),
			completion_status
		).asMessage()
	);
}


void
RobotServer::_notifyGyroReading (const Gyro& gyro) throw ()
{
	// We only get here if the client subscribed to the gyro readings

	const Gyro::Reading& reading = gyro.getLatestReading();
	addResponse(
		GyroReadingNotice(
			gyro.getSubscriberTaskId(),
			reading.micros / 1000,
			reading.getYawDegrees(),
			reading.getPitchDegrees(),
			reading.getRollDegrees(),
			reading.micros
		).asMessage()
	);
}


void
RobotServer::_setWheelDrive (
	UInt8 motor_1_direction,
	UInt8 motor_1_signal,
	UInt8 motor_2_direction,
	UInt8 motor_2_signal
) throw ()
{
	m_motor_1.setDirection( motor_1_direction );
	m_motor_1.setSignal( motor_1_signal );
	m_motor_2.setDirection( motor_2_direction );
	m_motor_2.setSignal( motor_2_signal );
}


void
RobotServer::_setServoAngle (
	UInt8 servo_id,
	UInt8 angle
) throw ()
{
	if ( servo_id == 0 ) {
		m_servo.setAngle( angle );
	}
}
