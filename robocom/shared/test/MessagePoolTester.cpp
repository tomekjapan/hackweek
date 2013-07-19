#include <unittest++/UnitTest++.h>

#include "../MessagePool.hpp"

using namespace robocom::shared;

namespace robocom {
namespace shared
{
	const int SLOT_COUNT = MessagePool::SLOT_COUNT;

	SUITE(MessagePoolTester)
	{
		TEST(AllocFree)
		{
			MessagePool p;
			CHECK_EQUAL( SLOT_COUNT, (int) p.getFree() );
			CHECK_EQUAL( SLOT_COUNT, (int) p.getMinFree() );

			MessageListNode* node_ptrs[SLOT_COUNT];
			for ( int i = 0; i < SLOT_COUNT; i++ )
			{
				node_ptrs[i] = p.alloc();
				CHECK( 0 != node_ptrs[i] );
			}

  			CHECK_EQUAL( 0, (int) p.getFree() );
			CHECK_EQUAL( 0, (int) p.getMinFree() );
			CHECK( 0 == p.alloc() );

			for ( int i = 5; i < 8; i++ )
			{
				p.free( node_ptrs[i] );
				node_ptrs[i] = 0;
			}

  			CHECK_EQUAL( 3, (int) p.getFree() );
			CHECK_EQUAL( 0, (int) p.getMinFree() );

			for ( int i = 5; i < 8; i++ )
			{
				node_ptrs[i] = p.alloc();
				CHECK( 0 != node_ptrs[i] );
			}

  			CHECK_EQUAL( 0, (int) p.getFree() );
			CHECK_EQUAL( 0, (int) p.getMinFree() );
			CHECK( 0 == p.alloc() );

			for ( int i = 0; i < SLOT_COUNT; i++ ) {
				p.free( node_ptrs[i] );
				node_ptrs[i] = 0;
			}

  			CHECK_EQUAL( SLOT_COUNT, (int) p.getFree() );
			CHECK_EQUAL( 0, (int) p.getMinFree() );
		}

		TEST(MinFree)
		{
			MessagePool p;
			CHECK_EQUAL( SLOT_COUNT, (int) p.getMinFree() );

			MessageListNode* p_node1 = p.alloc();
			CHECK_EQUAL( SLOT_COUNT - 1, (int) p.getMinFree() );

			MessageListNode* p_node2 = p.alloc();
			CHECK_EQUAL( SLOT_COUNT - 2, (int) p.getMinFree() );

			p.free( p_node1 );
			CHECK_EQUAL( SLOT_COUNT - 2, (int) p.getMinFree() );

			p.free( p_node2 );
			CHECK_EQUAL( SLOT_COUNT - 2, (int) p.getMinFree() );
		}
	}

} }
