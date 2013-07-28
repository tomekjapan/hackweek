#ifndef ROBOCOM_SHARED_SERVER_HPP
#define ROBOCOM_SHARED_SERVER_HPP

#include "shared_base.hpp"

// Component includes
#include "msg/msg_fwds.hpp"
#include "MessageIO.hpp"
#include "MessagePool.hpp"
#include "MessageQueue.hpp"


namespace robocom {
namespace shared
{

	/**
	 * This class implements a framework for Arduino firmware
	 *
	 * It sets up an environment in which the firmware is driven by
	 * messages received over some kind of communication channel
	 * (like Serial).
	 */
	class Server
	{
	public:

		/// @name Lifetime management
		///@{

		/**
		 * Creates a new instance
		 *
		 * @param stream the StreamIO instance to use for the communication
		 *  channel
		 */
		Server (StreamIO& stream) throw ();

		/**
		 * Destroys the state of this object
		 */
		virtual ~Server () throw ();

		///@}


		/// @name Methods
		///@{

		/**
		 * Returns the number of milliseconds since an unspecified time
		 * during execution of the current process.
		 */
		UInt32 getMillis () const throw ();

		/**
		 * Returns the number of microseconds since an unspecified time
		 * during execution of the current process.
		 */
		UInt32 getMicros () const throw ();

		/**
		 * Sets up the server environment
		 *
		 * This function should be called once, typically from the
		 * setup() function of the Arduino sketch.
		 *
		 * The default implementation does nothing.
		 */
		virtual void setup ();

		/**
		 * Performs one step in a loop
		 *
		 * This function should be called periodically, typically from the
		 * loop() function of the Arduino sketch.
		 *
		 * The implementation will handle communication with
		 * the client, will handle common messages and will call event
		 * methods on this object.
		 */
		void loop ();

		///@}

	protected:

		/// @name Protected methods
		///@{

		/**
		 * Adds the given message to the output queue of this server
		 *
		 * The message will be delivered to the client when it requests
		 * a flush (with a FlushRequest) and the message millis value 
		 * is earlier than the getMillis() of this server. Thus it is
		 * possible to add a response that it will be delivered after
		 * some point in future.
		 *
		 * @return true if the message was added, false if there is no more
		 *  free space and the message was not added
		 */
		void addResponse (const Message& msg) throw ();

		/**
		 * Method called by the framework when the client requests the reset
		 * of the robot state
		 *
		 * By the time this function is called, the framework has already
		 * cleared the input and output message queues. Derived classes may
		 * do additional cleanup, such as stopping motors, resetting servo
		 * positions etc.
		 *
		 * The default implementation does not do anything.
		 */
		virtual void handleReset (const msg::ResetRequest& req);

		/**
		 * Method called by the framework when a message is due for processing
		 *
		 * This function in derived classes is responsible for handling
		 * application specific messages. Messages may alter the state
		 * of the system, and may cause responses to be generated and added
		 * to this server.
		 *
		 * The default implementation does not do anything.
		 */
		virtual void handleMessage (const Message& msg);

		/**
		 * Method called by the framework periodically
		 *
		 * Derived classes should gather any new measurements and add any
		 * generated responses this server.
		 *
		 * The default implementation does not do anything.
		 */
		virtual void handleStateUpdate ();

		///@}

	private:

		Server (const Server&);
		void operator= (const Server&);

		void _onNewMessage (const Message& msg);
		void _handleReset (const msg::ResetRequest& req);
		void _handleFlush (const msg::FlushRequest& req);
		
		MessagePool m_pool;
		MessageQueue m_input_queue;
		MessageQueue m_output_queue;
		MessageIO m_io;
#if ! defined(AVR)
		SInt64 m_base_seconds;
#endif
	};

} }

#endif
