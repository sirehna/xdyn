/*
 * H5InterfaceTest.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: jacquenot
 */

#include "h5_interface.hpp"
#include "h5_version.hpp"
#include "h5_interface_tests.hpp"

#include <string>

H5InterfaceTest::H5InterfaceTest() : a(ssc::random_data_generator::DataGenerator(666))
{
}

TEST_F(H5InterfaceTest, writeFileDescription)
{
    const std::string fileName("writeFileDescription.h5");
    EXPECT_EQ(1,h5_writeFileDescription(fileName));
    EXPECT_EQ(0,remove(fileName.c_str()));
}

#define MAX_NAME_LENGTH 32

typedef struct {
    int age;
    char sex;
    char name[MAX_NAME_LENGTH];
    float height;
} PersonalInformation;

template <> H5::CompType H5_Interface<PersonalInformation>::createId()
{
    const std::string member_age("Age");
    const std::string member_sex("Sex");
    const std::string member_name("Name");
    const std::string member_height("Height");
    H5::CompType mtype = H5::CompType(sizeof(PersonalInformation));
    mtype.insertMember(member_age, HOFFSET(PersonalInformation, age), H5::PredType::NATIVE_INT);
    mtype.insertMember(member_sex, HOFFSET(PersonalInformation, sex), H5::PredType::C_S1);
    mtype.insertMember(member_name, HOFFSET(PersonalInformation, name), H5::StrType(H5::PredType::C_S1, MAX_NAME_LENGTH));
    mtype.insertMember(member_height, HOFFSET(PersonalInformation, height), H5::PredType::NATIVE_FLOAT);
    return mtype;
}

TEST_F(H5InterfaceTest, should_be_able_to_serialize_and_parse_a_static_structure)
{
    const std::string fileName("SimpleCompound.h5");
    const std::string datasetName("PersonalInformation");
    PersonalInformation person_list[] = {
            { 18, 'M', "Mary",  (float)152.0 },
            { 32, 'F', "Tom",   (float)178.6 },
            { 29, 'M', "Tarou", (float)166.6 }};

    {
        H5_Serialize<PersonalInformation> s1(fileName, datasetName);
        s1.write(&person_list[0]);
        s1.write(&person_list[2]);
        s1 << person_list[1];
    }

    H5_Parse<PersonalInformation> s2;
    std::vector<PersonalInformation> res = s2.Read(fileName, datasetName);
    ASSERT_EQ(3,res.size());
    ASSERT_EQ(18,res.at(0).age);
    ASSERT_EQ('M',res.at(0).sex);
    ASSERT_STREQ("Mary",res.at(0).name);
    ASSERT_EQ((float)152.0,res.at(0).height);

    ASSERT_EQ(29,res.at(1).age);
    ASSERT_EQ('M',res.at(1).sex);
    ASSERT_STREQ("Tarou",res.at(1).name);
    ASSERT_EQ((float)166.6,res.at(1).height);

    ASSERT_EQ(32,res.at(2).age);
    ASSERT_EQ('F',res.at(2).sex);
    ASSERT_STREQ("Tom",res.at(2).name);
    ASSERT_EQ((float)178.6,res.at(2).height);
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups)
{
    const std::string fileName("should_be_able_to_create_file_with_groups.h5");
    H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    H5::Group group1_id = h5File.createGroup("/MyGroup", 0);
    H5::Group group2_id = h5File.createGroup("/MyGroup/Group_A", 0);
    H5::Group group3_id = group1_id.createGroup("Group_B", 0);
    group1_id.close();
    group2_id.close();
    group3_id.close();
    h5File.close();
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_dataset_in_groups)
{
    const std::string fileName("should_be_able_to_create_dataset_in_groups.h5");
    H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    const std::string datasetName("/A/B/C/D/toto");
    H5::DataType dtype(H5::PredType::NATIVE_DOUBLE);
    H5::DataSpace space = h5_CreateDataSpace1DUnlimited();
    H5::DataSet s = h5_CreateDataSet(h5File, datasetName, dtype, space);
    s.close();
    dtype.close();
    space.close();
    h5File.close();
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_throw_an_exception_when_trying_to_create_an_existing_dataset)
{
    const std::string fileName("should_throw_an_exception_when_trying_to_create_an_existing_dataset.h5");
    H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    const std::string datasetName("/A/B/C/D/toto");
    H5::DataType dtype(H5::PredType::NATIVE_DOUBLE);
    H5::DataSpace space = h5_CreateDataSpace1DUnlimited();
    H5::DataSet s = h5_CreateDataSet(h5File, datasetName, dtype, space);
    H5::DataSet ss;
    ASSERT_THROW(ss=h5_CreateDataSet(h5File, datasetName, dtype, space), H5InterfaceException);
    s.close();
    dtype.close();
    space.close();
    h5File.close();
    h5File.close();
    EXPECT_EQ(0,remove(fileName.c_str()));
}

