#include "Hdf5WaveObserver.hpp"
#include "Hdf5WaveObserverTest.hpp"

SimHdf5WaveObserverTest::SimHdf5WaveObserverTest() : a(ssc::random_data_generator::DataGenerator(546545))
{
}

SimHdf5WaveObserverTest::~SimHdf5WaveObserverTest()
{
}

void SimHdf5WaveObserverTest::SetUp()
{
}

void SimHdf5WaveObserverTest::TearDown()
{
}

Eigen::MatrixXd foo(const Eigen::VectorXd& x, const Eigen::VectorXd& y);
Eigen::MatrixXd foo(const Eigen::VectorXd& x, const Eigen::VectorXd& y)
{
    const size_t nx = x.size();
    const size_t ny = y.size();
    Eigen::MatrixXd m(nx,ny);
    for (size_t i=0;i<nx;++i)
    {
        for (size_t j=0;j<ny;++j)
        {
            m(i,j)=x[i]+y[j];
        }
    }
    return m;
}

TEST_F(SimHdf5WaveObserverTest, should_be_able_to_allocate_hdf5_file)
{
    const std::string filename("should_be_able_to_allocate_hdf5_file.h5");
    const size_t nx = 100;
    const size_t ny = 150;
    {
        SimHdf5WaveObserver s(filename,"WaveElevation",nx,ny);
    }
    EXPECT_EQ(0,remove(filename.c_str()));
}

TEST_F(SimHdf5WaveObserverTest, can_serialize_a_wave_field_elevation_in_time)
{
    const std::string filename("can_serialize_a_wave_field_elevation_in_time.h5");
    const size_t nx = 100;
    const size_t ny = 150;
    SurfaceElevationGrid waveElevationGrid;
    waveElevationGrid.t = 10.0;
    waveElevationGrid.x = Eigen::VectorXd(nx);
    waveElevationGrid.y = Eigen::VectorXd(ny);
    for (size_t i = 0;i<(size_t)waveElevationGrid.x.size();++i)
    {
        waveElevationGrid.x(i) = (double)i;
    }
    for (size_t j = 0;j<(size_t)waveElevationGrid.y.size();++j)
    {
        waveElevationGrid.y(j) = (double)j;
    }
    waveElevationGrid.z = foo(waveElevationGrid.x,waveElevationGrid.y);
    {
        SimHdf5WaveObserver s(filename,"WaveElevation",nx,ny);
        for (size_t i = 0;i<50;++i)
        {
            s<<waveElevationGrid;
        }
    }
    EXPECT_EQ(0,remove(filename.c_str()));
}

