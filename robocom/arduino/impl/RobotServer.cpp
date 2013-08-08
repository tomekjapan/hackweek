// External component includes
#include "robocom/shared/msg/EncoderReadingNotice.hpp"
#include "robocom/shared/msg/EncoderReadingRequest.hpp"
#include "robocom/shared/msg/GyroReadingNotice.hpp"
#include "robocom/shared/msg/GyroReadingRequest.hpp"
#include "robocom/shared/msg/SetWheelDriveRequest.hpp"
#include "robocom/shared/msg/SimpleMessage.hxx"
#include "robocom/shared/msg/WheelDriveChangedNotice.hpp"

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
	, m_last_gyro_output_micros(0)
    , m_turn()
{
}


void
RobotServer::setup () throw ()
{
	m_motor_1.setup();
	m_motor_2.setup();

	m_encoder_1.setup();
	m_encoder_2.setup();

	m_gyro.initialize(); // TODO: Error check?
}


void
RobotServer::handleReset (const ResetRequest& req) throw ()
{
	m_encoder_1.clearSubscriber();
	m_encoder_2.clearSubscriber();
	m_gyro.clearSubscriber();

	_setWheelDrive( 0, 0, 0, 0 );
	_notifyWheelDriveChanged( req.asMessage() );
}


void
RobotServer::handleMessage (const Message& msg) throw ()
{
	switch ( msg.getMessageType() )
	{
	case SetWheelDriveRequest::MSGID:
		_processMessage( SetWheelDriveRequest( msg ) );
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

	_notifyWheelDriveChanged( req.asMessage() );
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
RobotServer::_notifyWheelDriveChanged (const Message& msg) throw ()
{
	addResponse(
		WheelDriveChangedNotice(
			msg.getTaskId(),
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
