#include <unittest++/UnitTest++.h>

#include "../Message.hpp"

namespace robocom {
namespace shared
{

	SUITE(MessageTester)
	{
		TEST(DataSize)
		{
			Message msg;
			msg.setDataSize(6);
			CHECK_EQUAL( 6, (int) msg.getDataSize() );
			msg.setDataSize(0);
			CHECK_EQUAL( 0, (int) msg.getDataSize() );
		}

		TEST(MaxDataSize)
		{
			Message msg;
			msg.setImmediate();
			CHECK( msg.isImmediate() );
			CHECK_EQUAL( Message::MAX_DATA_SIZE, (int) msg.getMaxDataSize() );
			msg.setMillis( 15 );
			CHECK( ! msg.isImmediate() );
			CHECK_EQUAL( Message::MAX_DATA_SIZE - 4, (int) msg.getMaxDataSize() );
			msg.setImmediate();
			CHECK( msg.isImmediate() );
			CHECK_EQUAL( Message::MAX_DATA_SIZE, (int) msg.getMaxDataSize() );
		}

		TEST(TaskId)
		{
			Message msg;
			msg.setTaskId(0xFFEE);
			CHECK_EQUAL( 0xFFEE, msg.getTaskId() );
			msg.setTaskId(0);
			CHECK_EQUAL( 0, msg.getTaskId() );
		}

		TEST(MessageType)
		{
			Message msg;
			msg.setMessageType(12);
			CHECK_EQUAL( 12, (int) msg.getMessageType() );
			msg.setImmediate();
			CHECK_EQUAL( 12, (int) msg.getMessageType() );
			msg.setMessageType(1);
			CHECK_EQUAL( 1, (int) msg.getMessageType() );
			CHECK( msg.isImmediate() );
			CHECK_EQUAL( 1, (int) msg.getMessageType() );
		}

		TEST(Millis)
		{
			Message msg;
			msg.setMillis(25);
			CHECK_EQUAL( 25, msg.getMillis() );
			msg.setMillis( 0xFFEEDDCC );
			CHECK_EQUAL( 0xFFEEDDCC, msg.getMillis() );
			msg.setImmediate();
			CHECK_EQUAL( 0, msg.getMillis() );
			msg.setMillis(0);
			CHECK_EQUAL( 0, msg.getMillis() );
			CHECK( ! msg.isImmediate() );
		}

		TEST(DataUInt8)
		{
			Message msg;

			msg.setMillis(0);
			msg.setDataSize( msg.getMaxDataSize() );
			msg.setUInt8( 0, 5 );
			msg.setUInt8( msg.getDataSize() - 5, 20 );
			msg.setUInt8( msg.getDataSize() - 1, 100 );
			CHECK_EQUAL( 5, (int) msg.getUInt8(0) );
			CHECK_EQUAL( 20, (int) msg.getUInt8( msg.getDataSize() - 5 ) );
			CHECK_EQUAL( 100, (int) msg.getUInt8( msg.getDataSize() - 1 ) );

			msg.setImmediate();
			msg.setUInt8( msg.getDataSize() - 5, 50 );
			CHECK_EQUAL( 5, (int) msg.getUInt8(0) );
			CHECK_EQUAL( 50, (int) msg.getUInt8( msg.getDataSize() - 5 ) );
			CHECK_EQUAL( 100, (int) msg.getUInt8( msg.getDataSize() - 1 ) );

			msg.setMillis(15);
			CHECK_EQUAL( 5, (int) msg.getUInt8(0) );
			CHECK_EQUAL( 50, (int) msg.getUInt8( msg.getDataSize() - 5 ) );
			CHECK_EQUAL( 100, (int) msg.getUInt8( msg.getDataSize() - 1 ) );
		}

		TEST(DataUInt16)
		{
			Message msg;

			msg.setMillis(0);
			msg.setDataSize( msg.getMaxDataSize() );
			msg.setUInt16( 0, 5 );
			msg.setUInt16( msg.getDataSize() - 6, 0xFFEE );
			msg.setUInt16( msg.getDataSize() - 2, 100 );
			CHECK_EQUAL( 5, msg.getUInt16(0) );
			CHECK_EQUAL( 0xFFEE, msg.getUInt16( msg.getDataSize() - 6 ) );
			CHECK_EQUAL( 100, msg.getUInt16( msg.getDataSize() - 2 ) );

			msg.setImmediate();
			msg.setUInt16( msg.getDataSize() - 6, 0xEECC );
			CHECK_EQUAL( 5, msg.getUInt16(0) );
			CHECK_EQUAL( 0xEECC, msg.getUInt16( msg.getDataSize() - 6 ) );
			CHECK_EQUAL( 100, msg.getUInt16( msg.getDataSize() - 2 ) );

			msg.setMillis(15);
			CHECK_EQUAL( 5, msg.getUInt16(0) );
			CHECK_EQUAL( 0xEECC, msg.getUInt16( msg.getDataSize() - 6 ) );
			CHECK_EQUAL( 100, msg.getUInt16( msg.getDataSize() - 2 ) );
		}

		TEST(DataUInt32)
		{
			Message msg;

			msg.setMillis(0);
			msg.setDataSize( msg.getMaxDataSize() );
			msg.setUInt32( 0, 5 );
			msg.setUInt32( msg.getDataSize() - 8, 0xFFEECCBB );
			msg.setUInt32( msg.getDataSize() - 4, 100 );
			CHECK_EQUAL( 5, msg.getUInt32(0) );
			CHECK_EQUAL( 0xFFEECCBB, msg.getUInt32( msg.getDataSize() - 8 ) );
			CHECK_EQUAL( 100, msg.getUInt32( msg.getDataSize() - 4 ) );

			msg.setImmediate();
			msg.setUInt32( msg.getDataSize() - 8, 0xEECCBBAA );
			CHECK_EQUAL( 5, msg.getUInt32(0) );
			CHECK_EQUAL( 0xEECCBBAA, msg.getUInt32( msg.getDataSize() - 8 ) );
			CHECK_EQUAL( 100, msg.getUInt32( msg.getDataSize() - 4 ) );

			msg.setMillis(15);
			CHECK_EQUAL( 5, msg.getUInt32(0) );
			CHECK_EQUAL( 0xEECCBBAA, msg.getUInt32( msg.getDataSize() - 8 ) );
			CHECK_EQUAL( 100, msg.getUInt32( msg.getDataSize() - 4 ) );
		}

		TEST(Clear)
		{
			Message msg;
			msg.setTaskId( 12 );
			msg.setMessageType( 23 );
			msg.setImmediate();
			msg.setDataSize( msg.getMaxDataSize() );
			msg.setUInt32( 0, 0xFFFEEE );
			msg.setUInt8( msg.getDataSize() - 1, 33 );

			msg.clear();
			CHECK_EQUAL( 0, msg.getTaskId() );
			CHECK_EQUAL( 0, msg.getMessageType() );
			CHECK( ! msg.isImmediate() );
			CHECK_EQUAL( 0, msg.getDataSize() );

			msg.setDataSize( msg.getMaxDataSize() );
			for ( int i = 0; i < msg.getDataSize(); i++ ) {
				CHECK_EQUAL( 0, msg.getUInt8(i) );
			}
		}

		TEST(ComparePriority1)
		{
			Message msg1;
			msg1.setImmediate();

			Message msg2;
			msg2.setImmediate();

			CHECK_EQUAL( 0, msg1.comparePriority( msg2 ) );
			CHECK_EQUAL( 0, msg2.comparePriority( msg1 ) );
		}

		TEST(ComparePriority2)
		{
			Message msg1;
			msg1.setImmediate();

			Message msg2;
			msg2.setMillis(0u);

			CHECK_EQUAL( -1, msg1.comparePriority( msg2 ) );
			CHECK_EQUAL( 1, msg2.comparePriority( msg1 ) );
		}

		TEST(ComparePriority3)
		{
			Message msg1;
			msg1.setImmediate();

			Message msg2;
			msg2.setMillis(10u);

			CHECK_EQUAL( -1, msg1.comparePriority( msg2 ) );
			CHECK_EQUAL( 1, msg2.comparePriority( msg1 ) );
		}

		TEST(ComparePriority4)
		{
			Message msg1;
			msg1.setMillis(5u);

			Message msg2;
			msg2.setMillis(10u);

			CHECK_EQUAL( -1, msg1.comparePriority( msg2 ) );
			CHECK_EQUAL( 1, msg2.comparePriority( msg1 ) );
		}

		TEST(ComparePriority5)
		{
			Message msg1;
			msg1.setMillis(5u);

			Message msg2;
			msg2.setMillis(5u);

			CHECK_EQUAL( 0, msg1.comparePriority( msg2 ) );
			CHECK_EQUAL( 0, msg2.comparePriority( msg1 ) );
		}
	}

} }

