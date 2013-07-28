#ifndef ROBOCOM_SHARED_MSG_MSG_FWDS_HPP
#define ROBOCOM_SHARED_MSG_MSG_FWDS_HPP

#include "MessageTypes.hpp"
#include "MessageStatus.hpp"

namespace robocom {
namespace shared {
namespace msg
{

	template <int ID> class SimpleMessage;
	typedef SimpleMessage<CommonMessageTypes::MSGID_NOOP> NoopRequest;
	typedef SimpleMessage<CommonMessageTypes::MSGID_ECHO> EchoRequest;
	typedef SimpleMessage<CommonMessageTypes::MSGID_ECHO> EchoResponse;
	typedef SimpleMessage<CommonMessageTypes::MSGID_FLUSH> FlushRequest;
	typedef SimpleMessage<CommonMessageTypes::MSGID_RESET> ResetRequest;

	class EncoderReadingNotice;
	class EncoderReadingRequest;
	class FlushResponse;
	class SetWheelDriveRequest;
	class WheelDriveChangedNotice;

	using namespace common;

} } }

#endif
