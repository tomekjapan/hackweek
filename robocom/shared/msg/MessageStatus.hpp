#ifndef ROBOCOM_SHARED_MESSAGE_STATUS_HPP
#define ROBOCOM_SHARED_MESSAGE_STATUS_HPP

namespace robocom {
namespace shared {
namespace msg
{

	/**
	 * Definitions of message related errors
	 */
	enum MessageStatus
	{
		STATUS_OK = 0,

		STATUS_E_MESSAGE_TYPE,
		STATUS_E_DATA_SIZE,
		STATUS_E_IMMEDIATE,
		STATUS_E_NOT_IMMEDIATE,
		STATUS_E_FREE_SLOTS,
		STATUS_E_INPUT_QUEUE_SIZE,
		STATUS_E_OUTPUT_QUEUE_SIZE,
		STATUS_E_MOTOR_1_DIRECTION,
		STATUS_E_MOTOR_2_DIRECTION
	};

} } }

#endif
