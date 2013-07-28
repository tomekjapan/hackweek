#ifndef ROBOCOM_SHARED_SHARED_FWDS_HPP
#define ROBOCOM_SHARED_SHARED_FWDS_HPP

#if defined(AVR)
class Stream;
#endif

namespace robocom {
namespace shared
{

	class Message;
	class MessageIO;
	class MessageListNode;
	class MessagePool;
	class MessageQueue;
	class Server;

#if defined(AVR)
	typedef ::Stream StreamIO;
#else
	class StreamIO;
#endif

	using namespace common;

} }

#endif
