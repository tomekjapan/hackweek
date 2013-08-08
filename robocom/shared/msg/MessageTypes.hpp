#ifndef ROBOCOM_SHARED_MESSAGE_TYPES_HPP
#define ROBOCOM_SHARED_MESSAGE_TYPES_HPP

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * Definitions of common message type IDs
	 */
	class CommonMessageTypes
	{
	public:
		enum
		{
			MSGID_NOOP,
			MSGID_ECHO,
			MSGID_RESET,
			MSGID_FLUSH,
			LAST
		};
	};

	/**
	 * Definitions of message type IDs for the robocom app
	 */
	class RobocomMessageTypes
	{
	public:
		enum
		{
			MSGID_WHEEL_DRIVE = CommonMessageTypes::LAST,
			MSGID_ENCODER_READING,
			MSGID_GYRO_READING,
			MSGID_SERVO_ANGLE,
			LAST
		};
	};

} } }

#endif
