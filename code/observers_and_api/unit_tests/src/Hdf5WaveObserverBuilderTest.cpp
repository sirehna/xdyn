#include "Hdf5WaveObserver.hpp"
#include "Hdf5WaveObserverBuilder.hpp"
#include "Hdf5WaveObserverTest.hpp"

TEST_F(SimHdf5WaveObserverTest, SimHdf5WaveObserver_can_be_created_from_a_filename)
{
    const std::string fileName("SimHdf5WaveObserver_can_be_created_from_a_filename.h5");
    const std::string datasetName("waveElevation");
    const size_t nx(10);
    const size_t ny(30);
    {
        const Hdf5WaveObserverBuilder s(fileName,datasetName,nx,ny);
        ASSERT_EQ(nx,s.get_nx());
        ASSERT_EQ(ny,s.get_ny());
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(SimHdf5WaveObserverTest, SimHdf5WaveObserver_can_be_created_from_a_H5File)
{
    const std::string fileName("SimHdf5WaveObserver_can_be_created_from_a_filename.h5");
    const std::string datasetName("waveElevation");
    const size_t nx(10);
    const size_t ny(30);
    {
        const Hdf5WaveObserverBuilder s(H5::H5File(fileName,H5F_ACC_TRUNC),datasetName,nx,ny);
        ASSERT_EQ(nx,s.get_nx());
        ASSERT_EQ(ny,s.get_ny());
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(SimHdf5WaveObserverTest, SimHdf5WaveObserver_can_be_created_from_a_H5File2)
{
    const std::string fileName("SimHdf5WaveObserver_can_be_created_from_a_filename.h5");
    const std::string datasetName("simu0/waveElevation");
    const size_t nx(10);
    const size_t ny(30);
    {
        const Hdf5WaveObserverBuilder s(H5::H5File(fileName,H5F_ACC_TRUNC),datasetName,nx,ny);
        ASSERT_EQ(nx,s.get_nx());
        ASSERT_EQ(ny,s.get_ny());
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}
