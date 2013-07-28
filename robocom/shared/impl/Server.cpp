#if defined(AVR)
#include <Arduino.h>
#else
#include <time.h>
#include <cstring>
#endif

// Component includes
#include "../msg/FlushResponse.hpp"
#include "../msg/SimpleMessage.hxx"

// Module include
#include "../Server.hpp"

namespace robocom {
namespace shared
{

	using namespace common;
	using namespace robocom::shared::msg;


	Server::Server (StreamIO& stream) throw ()
		: m_pool( )
		, m_input_queue( m_pool )
		, m_output_queue( m_pool )
		, m_io( stream )
#if ! defined(AVR)
		, m_base_seconds( 0 )
#endif
	{
#if ! defined(AVR)
		::timespec ts;
		::bzero( & ts, sizeof( ts ) );

		if ( ::clock_gettime( CLOCK_REALTIME, & ts ) < 0 ) {
			NCR_UNEXPECTED( "failed to get the base time" );
		}

		m_base_seconds = ts.tv_sec;
#endif
	}


	Server::~Server () throw ()
	{
	}


	UInt32
	Server::getMillis () const throw ()
	{
#if defined(AVR)
		return millis();
#else
		::timespec ts;
		::bzero( & ts, sizeof( ts ) );

		if ( ::clock_gettime( CLOCK_PROCESS_CPUTIME_ID, & ts ) < 0 ) {
			NCR_UNEXPECTED( "failed to get the current time" );
		}

		ts.tv_sec -= static_cast< ::time_t>( m_base_seconds );
		return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
	}


	UInt32
	Server::getMicros () const throw ()
	{
#if defined(AVR)
		return micros();
#else
		::timespec ts;
		::bzero( & ts, sizeof( ts ) );

		if ( ::clock_gettime( CLOCK_PROCESS_CPUTIME_ID, & ts ) < 0 ) {
			NCR_UNEXPECTED( "failed to get the current time" );
		}

		ts.tv_sec -= static_cast< ::time_t>( m_base_seconds );
		return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
#endif
	}


	void
	Server::setup ()
	{
		// empty in the base class
	}


	void
	Server::loop ()
	{
		Message msg;

		if ( m_io.read( msg ) ) {
			_onNewMessage( msg );
		}
		else if ( m_input_queue.pop( msg, getMillis() ) ) {
			handleMessage( msg );
		}

		handleStateUpdate();
	}


	void
	Server::addResponse (const Message& msg) throw ()
	{
		if ( ! m_output_queue.push( msg ) ) {
			NCR_UNEXPECTED( "failed to add a response: queue is full" );
		}
	}


	void
	Server::handleReset (const ResetRequest& req)
	{
		// empty in the base class
	}


	void
	Server::handleMessage (const Message& msg)
	{
		// empty in the base class
	}


	void
	Server::handleStateUpdate ()
	{
		// empty in the base class
	}


	void
	Server::_onNewMessage (const Message& msg)
	{
		switch ( msg.getMessageType() )
		{
		case NoopRequest::MSGID:
			// do nothing message
			break;
		case EchoRequest::MSGID:
			m_io.write( msg );
			break;
		case ResetRequest::MSGID:
			_handleReset( ResetRequest( msg ) );
			break;
		case FlushRequest::MSGID:
			_handleFlush( FlushRequest( msg ) );
			break;
		default:
			m_input_queue.push( msg );
			break;
		}
	}


	void
	Server::_handleReset (const ResetRequest& req)
	{
		m_input_queue.clear();
		m_output_queue.clear();

		handleReset( req );
	}


	void
	Server::_handleFlush (const FlushRequest& req)
	{
		Message msg;
		while ( m_output_queue.pop( msg, getMillis() ) )
		{
			m_io.write( msg );

			// Update the state after each written message so that we
			// minimize the risk of missing any state changes
			handleStateUpdate();
		}

		m_io.write(
			FlushResponse(
				req.getTaskId(),
				getMillis(),
				m_pool.getMinFree(),
				m_pool.getFree(),
				m_input_queue.getMaxSize(),
				m_input_queue.getSize(),
				m_output_queue.getMaxSize(),
				m_output_queue.getSize()
			).asMessage()
		);
	}

} }
