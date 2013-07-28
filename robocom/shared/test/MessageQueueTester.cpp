#include <unittest++/UnitTest++.h>

#include "../MessagePool.hpp"
#include "../MessageQueue.hpp"
#include "../msg/SetWheelDriveRequest.hpp"
#include "../msg/FlushResponse.hpp"


namespace robocom {
namespace shared
{

	using namespace robocom::shared;
	using namespace robocom::shared::msg;

	void __checkEqual (const Message& a, const Message& b)
	{
		CHECK_EQUAL( (int) a.getMessageType(), (int) b.getMessageType() );
		CHECK_EQUAL( a.isImmediate(), b.isImmediate() );
		CHECK_EQUAL( a.getTaskId(), b.getTaskId() );
		CHECK_EQUAL( a.getMillis(), b.getMillis() );
		CHECK_EQUAL( (int) a.getDataSize(), (int) b.getDataSize() );
		for ( int i = 0; i < a.getDataSize(); i++ ) {
			CHECK_EQUAL( (int) a.getUInt8(i), (int) b.getUInt8(i) );
		}
	}

	SUITE(MessageQueueTester)
	{
		TEST(PushPop)
		{
			MessagePool p;
			MessageQueue q(p);

			Message msg;
			CHECK( ! q.pop( msg, 1000u ) );

			{
				FlushResponse r( 99, 123u, 3, 5, 7, 6, 4, 2 );
				CHECK( q.push( r.asMessage() ) );
				CHECK( q.pop( msg, 1000u ) );
				__checkEqual( msg, r.asMessage() );
			}

			{
				SetWheelDriveRequest r( 98, 1, 230, 0, 11 );
				CHECK( q.push( r.asMessage() ) );
				CHECK( q.pop( msg, 1000u ) );
				__checkEqual( msg, r.asMessage() );
			}

			CHECK( ! q.pop( msg, 1000u ) );
		}

		TEST(PopOrder)
		{
			MessagePool p;
			MessageQueue q(p);
			
			SetWheelDriveRequest r1( 88, 5u, 0, 1, 0, 0 );
			SetWheelDriveRequest r2( 87, 0, 2, 0, 0 );
			SetWheelDriveRequest r3( 86, 0u, 0, 3, 0, 0 );
			SetWheelDriveRequest r4( 85, 0, 4, 0, 0 );
			SetWheelDriveRequest r5( 84, 0u, 0, 5, 0, 0 );
			SetWheelDriveRequest r6( 83, 3u, 0, 6, 0, 0 );
			SetWheelDriveRequest r7( 82, 10u, 0, 7, 0, 0 );

			q.push( r1.asMessage() );
			q.push( r2.asMessage() );
			q.push( r3.asMessage() );
			q.push( r4.asMessage() );
			q.push( r5.asMessage() );
			q.push( r6.asMessage() );
			q.push( r7.asMessage() );

			Message msg;

			CHECK( q.pop( msg, 2u ) );
			__checkEqual( r2.asMessage(), msg );

			CHECK( q.pop( msg, 2u ) );
			__checkEqual( r4.asMessage(), msg );

			CHECK( q.pop( msg, 2u ) );
			__checkEqual( msg, r3.asMessage() );

			CHECK( q.pop( msg, 2u ) );
			__checkEqual( msg, r5.asMessage() );

			CHECK( ! q.pop( msg, 2u ) );

			CHECK( q.pop( msg, 5u ) );
			__checkEqual( msg, r6.asMessage() );

			CHECK( q.pop( msg, 5u ) );
			__checkEqual( msg, r1.asMessage() );

			CHECK( ! q.pop( msg, 5u ) );

			CHECK( q.pop( msg, 100u ) );
			__checkEqual( msg, r7.asMessage() );

			CHECK_EQUAL( MessagePool::SLOT_COUNT, p.getFree() );
		}

		TEST(Size)
		{
			MessagePool p;
			MessageQueue q(p);
			Message m;
			m.clear();

 			// First push as many messages as possible, checking
			// that both sizes gets updated
			int s = 0;
			do
			{
				CHECK_EQUAL( s, (int) q.getSize() );
				CHECK_EQUAL( s, (int) q.getMaxSize() );
				s++;
			}
			while ( q.push( m ) );

			const int max_size = q.getMaxSize();
			s = max_size;

			// Now pop all messages, checking that only the size
			// gets updated and max size stays the same
			do
			{
				CHECK_EQUAL( s, (int) q.getSize() );
				CHECK_EQUAL( max_size, (int) q.getMaxSize() );
				s--;
			}
			while ( q.pop( m, 1u ) );
		}

		TEST(Clear)
		{
			MessagePool p;
			MessageQueue q(p);
			Message m;
			m.clear();

			// Fill up the queue with messages
			while ( q.push( m ) );

			q.clear();
			CHECK_EQUAL( q.getSize(), 0 );
			CHECK( q.push( m ) );
		}
	}

} }
