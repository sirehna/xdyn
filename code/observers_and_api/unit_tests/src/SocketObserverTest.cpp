#include "SocketObserver.hpp"
#include "SocketObserverTest.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

SocketObserverTest::SocketObserverTest() : a(ssc::random_data_generator::DataGenerator(546545))
{
}

TEST_F(SocketObserverTest, should_be_able_to_use_boost_asio_timer_synchronously)
{
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(1));
    t.wait();
}

void dummyFunction(const boost::system::error_code& /*e*/);
void dummyFunction(const boost::system::error_code& /*e*/)
{
}

TEST_F(SocketObserverTest, should_be_able_to_use_boost_asio_timer_asynchronously)
{
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(1));
    t.async_wait(&dummyFunction);
    io.run();
}
