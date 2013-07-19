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
	 * Definitions of message type IDs for the remocon app
	 */
	class RemoconMessageTypes
	{
	public:
		enum
		{
			MSGID_SET_WHEEL_DRIVE = CommonMessageTypes::LAST,
			MSGID_WHEEL_DRIVE_CHANGED,
			MSGID_ENCODER_SUBSCRIBE,
			MSGID_ENCODER_READING,
			LAST
		};
	};

} } }

#endif
