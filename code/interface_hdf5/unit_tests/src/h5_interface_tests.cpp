/*
 * H5InterfaceTest.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: jacquenot
 */

#include "h5_interface.hpp"
#include "h5_version.hpp"
#include "h5_interface_tests.hpp"
#include "InternalErrorException.hpp"

#include <string>

H5InterfaceTest::H5InterfaceTest() : a(ssc::random_data_generator::DataGenerator(666))
{
}

TEST_F(H5InterfaceTest, writeFileDescription)
{
    const std::string fileName("writeFileDescription.h5");
    ASSERT_EQ(1,h5_writeFileDescription(fileName));
    ASSERT_EQ(0,remove(fileName.c_str()));
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
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_1d_unlimited_dataspace)
{
    H5_Tools::createDataSpace1DUnlimited();
}

TEST_F(H5InterfaceTest, should_correctly_split_dataset_name_V0)
{
    const std::string s1("");
    const std::vector<std::string> s1Split = H5_Tools::split(s1, "/");
    ASSERT_TRUE(s1Split.empty());
}

TEST_F(H5InterfaceTest, should_correctly_split_dataset_name_V1)
{
    const std::string s1("toto");
    const std::vector<std::string> s1Split = H5_Tools::split(s1, "/");
    ASSERT_EQ(1,s1Split.size());
    ASSERT_EQ("toto",s1Split.at(0));
}

TEST_F(H5InterfaceTest, should_correctly_split_dataset_name_V2)
{
    const std::string s1("/A/B/C/D/toto");
    const std::vector<std::string> s1Split = H5_Tools::split(s1, "/");
    ASSERT_EQ(5,s1Split.size());
    ASSERT_EQ("A",s1Split.at(0));
    ASSERT_EQ("B",s1Split.at(1));
    ASSERT_EQ("C",s1Split.at(2));
    ASSERT_EQ("D",s1Split.at(3));
    ASSERT_EQ("toto",s1Split.at(4));
}

TEST_F(H5InterfaceTest, should_correctly_split_dataset_name_V3)
{
    const std::string s1("A/B/C/D/toto");
    const std::vector<std::string> s1Split = H5_Tools::split(s1, "/");
    ASSERT_EQ(5,s1Split.size());
    ASSERT_EQ("A",s1Split.at(0));
    ASSERT_EQ("B",s1Split.at(1));
    ASSERT_EQ("C",s1Split.at(2));
    ASSERT_EQ("D",s1Split.at(3));
    ASSERT_EQ("toto",s1Split.at(4));
}

TEST_F(H5InterfaceTest, should_correctly_split_dataset_name_V4)
{
    const std::string s1("A/B/C/D/toto/");
    const std::vector<std::string> s1Split = H5_Tools::split(s1, "/");
    ASSERT_EQ(6,s1Split.size());
    ASSERT_EQ("A",s1Split.at(0));
    ASSERT_EQ("B",s1Split.at(1));
    ASSERT_EQ("C",s1Split.at(2));
    ASSERT_EQ("D",s1Split.at(3));
    ASSERT_EQ("toto",s1Split.at(4));
    ASSERT_EQ("",s1Split.at(5));
}

TEST_F(H5InterfaceTest, should_correctly_split_dataset_name_V5)
{
    const std::string s1("/A/B/C/D/toto/");
    const std::vector<std::string> s1Split = H5_Tools::split(s1, "/");
    ASSERT_EQ(6,s1Split.size());
    ASSERT_EQ("A",s1Split.at(0));
    ASSERT_EQ("B",s1Split.at(1));
    ASSERT_EQ("C",s1Split.at(2));
    ASSERT_EQ("D",s1Split.at(3));
    ASSERT_EQ("toto",s1Split.at(4));
    ASSERT_EQ("",s1Split.at(5));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups_raw)
{
    const std::string fileName("should_be_able_to_create_file_with_groups_raw.h5");
    H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    H5::Group group1_id = h5File.createGroup("/MyGroup", 0);
    H5::Group group2_id = h5File.createGroup("/MyGroup/Group_A", 0);
    H5::Group group3_id = group1_id.createGroup("Group_B", 0);
    group1_id.close();
    group2_id.close();
    group3_id.close();
    h5File.close();
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups_Level0)
{
    const std::string fileName("should_be_able_to_create_file_with_groups_Level0.h5");
    const std::string datasetName("");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::Group g = H5_Tools::createMissingGroups(h5File,datasetName);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups_Level1)
{
    const std::string fileName("should_be_able_to_create_file_with_groups_Level1.h5");
    const std::string datasetName("A");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::Group g = H5_Tools::createMissingGroups(h5File,datasetName);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups_Level2a)
{
    const std::string fileName("should_be_able_to_create_file_with_groups_Level2a.h5");
    const std::string datasetName("A/B");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::Group g = H5_Tools::createMissingGroups(h5File,datasetName);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups_Level2b)
{
    const std::string fileName("should_be_able_to_create_file_with_groups_Level2b.h5");
    const std::string datasetName("/A/B");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::Group g = H5_Tools::createMissingGroups(h5File,datasetName);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups_Level2c)
{
    const std::string fileName("should_be_able_to_create_file_with_groups_Level2c.h5");
    const std::string datasetName("/A/");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::Group g = H5_Tools::createMissingGroups(h5File,datasetName);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_file_with_groups_LevelX)
{
    const std::string fileName("should_be_able_to_create_file_with_groups_LevelX.h5");
    const std::string datasetName("/A/B/C/D/toto");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::Group g = H5_Tools::createMissingGroups(h5File,datasetName);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_create_dataset_in_groups)
{
    const std::string fileName("should_be_able_to_create_dataset_in_groups.h5");
    const std::string datasetName("/A/B/C/D/toto");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::DataType dtype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSpace space = H5_Tools::createDataSpace1DUnlimited();
        H5::DataSet s = H5_Tools::createDataSet(h5File, datasetName, dtype, space);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_throw_an_exception_when_trying_to_create_an_existing_dataset)
{
    const std::string fileName("should_throw_an_exception_when_trying_to_create_an_existing_dataset.h5");
    const std::string datasetName("/A/B/C/D/toto");
    {
        H5::H5File h5File(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        H5::DataType dtype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSpace space = H5_Tools::createDataSpace1DUnlimited();
        H5::DataSet s = H5_Tools::createDataSet(h5File, datasetName, dtype, space);
        H5::DataSet ss;
        ASSERT_THROW(ss=H5_Tools::createDataSet(h5File, datasetName, dtype, space), InternalErrorException);
    }
    ASSERT_EQ(0,remove(fileName.c_str()));
}

TEST_F(H5InterfaceTest, should_handle_correctly_group_name)
{
    const std::string delim("/");
    ASSERT_EQ("/",H5_Tools::ensureStringStartsAndEndsWithAPattern("", delim));
    ASSERT_EQ("/",H5_Tools::ensureStringStartsAndEndsWithAPattern("/", delim));
    ASSERT_EQ("//",H5_Tools::ensureStringStartsAndEndsWithAPattern("//", delim));
    ASSERT_EQ("/a/",H5_Tools::ensureStringStartsAndEndsWithAPattern("a", delim));
    ASSERT_EQ("/a/b/",H5_Tools::ensureStringStartsAndEndsWithAPattern("a/b", delim));
    ASSERT_EQ("/a/b/",H5_Tools::ensureStringStartsAndEndsWithAPattern("a/b/", delim));
    ASSERT_EQ("/a/b/",H5_Tools::ensureStringStartsAndEndsWithAPattern("/a/b/", delim));
    ASSERT_EQ("/a/b/c/",H5_Tools::ensureStringStartsAndEndsWithAPattern("/a/b/c", delim));
    ASSERT_EQ("/a/b/c/",H5_Tools::ensureStringStartsAndEndsWithAPattern("/a/b/c/", delim));
}

TEST_F(H5InterfaceTest, should_handle_correctly_root_group_name)
{
    const std::string delim("/");
    ASSERT_EQ("/",H5_Tools::ensureStringStartsWithAPattern("", delim));
    ASSERT_EQ("/",H5_Tools::ensureStringStartsWithAPattern("/", delim));
    ASSERT_EQ("//",H5_Tools::ensureStringStartsWithAPattern("//", delim));
    ASSERT_EQ("/a",H5_Tools::ensureStringStartsWithAPattern("a", delim));
    ASSERT_EQ("/a/b",H5_Tools::ensureStringStartsWithAPattern("a/b", delim));
    ASSERT_EQ("/a/b/",H5_Tools::ensureStringStartsWithAPattern("a/b/", delim));
    ASSERT_EQ("/a/b/",H5_Tools::ensureStringStartsWithAPattern("/a/b/", delim));
    ASSERT_EQ("/a/b/c",H5_Tools::ensureStringStartsWithAPattern("/a/b/c", delim));
    ASSERT_EQ("/a/b/c/",H5_Tools::ensureStringStartsWithAPattern("/a/b/c/", delim));
}

TEST_F(H5InterfaceTest, should_be_able_to_perform_string_replacement)
{
    const std::string input = "abc abc def";
    const std::string res = H5_Tools::replaceString(input,"bc","!!");
    ASSERT_EQ("a!! a!! def",res);
}

TEST_F(H5InterfaceTest, should_be_able_to_perform_inplace_string_replacement)
{
    std::string input = "abc abc def";
    ASSERT_EQ("a?? a?? def",H5_Tools::replaceStringInPlace(input,"bc","??"));
    ASSERT_EQ("a?? a?? def",input);
}

TEST_F(H5InterfaceTest, should_be_able_to_join_group_names)
{
    std::vector<std::string> gg;
    gg.push_back("a");
    gg.push_back("b");
    ASSERT_EQ("a/b",H5_Tools::join(gg,"/"));
    gg.push_back("c");
    ASSERT_EQ("a/b/c",H5_Tools::join(gg,"/"));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_string_in_hdf5_files)
{
    const std::string filename = "file.h5";
    H5_Tools::write(filename, "/inputs/command", "command");
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_several_strings_in_hdf5_files)
{
    const std::string filename = "file.h5";
    H5_Tools::write(filename, "/inputs/command", "command0");
    H5_Tools::write(filename, "/inputs/instruction", "instruction");
    H5_Tools::write(filename, "/inputs/hello", "hello");
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_throw_an_exception_when_writing_to_an_existing_string)
{
    const std::string filename = "should_throw_an_exception_when_writing_to_an_existing_string.h5";
    H5_Tools::write(filename, "/inputs/command", "command0");
    ASSERT_THROW(H5_Tools::write(filename, "/inputs/command", "command1"), InternalErrorException);
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, writing_an_empty_string_should_not_crash)
{
    const std::string filename = "writing_an_empty_string_should_not_crash.h5";
    H5_Tools::write(filename, "/inputs/command", "");
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_double_in_hdf5_files)
{
    const std::string filename = "file.h5";
    H5_Tools::write(filename, "/inputs/command", 1.2);
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_double_in_hdf5_files_and_read_it_back)
{
    const std::string filename = "file.h5";
    const std::string datasetname = "/inputs/command";
    if (H5_Tools::doesFileExists(filename)) remove(filename.c_str());
    H5_Tools::write(filename, datasetname, 1.2);
    double res;
    H5_Tools::read(filename, datasetname, res);
    ASSERT_EQ(1.2,res);
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_vector_of_doubles_in_hdf5_files)
{
    const std::string filename = "file.h5";
    if (H5_Tools::doesFileExists(filename)) remove(filename.c_str());
    H5_Tools::write(filename, "/inputs/command", std::vector<double>(5,666.0));
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_vector_of_doubles_in_hdf5_files_and_read_it_back)
{
    const std::string filename = "file.h5";
    if (H5_Tools::doesFileExists(filename)) remove(filename.c_str());
    const std::string datasetname = "/inputs/command";
    std::vector<double> a({1.0, 2.0, 3.0});
    std::vector<double> res;
    H5_Tools::write(filename, datasetname, a);
    H5_Tools::read(filename, datasetname, res);
    ASSERT_EQ(3,res.size());
    ASSERT_EQ(1.0, res[0]);
    ASSERT_EQ(2.0, res[1]);
    ASSERT_EQ(3.0, res[2]);
    ASSERT_EQ(0, remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_vector_vector_of_doubles_in_hdf5_files)
{
    const std::string filename = "file.h5";
    if (H5_Tools::doesFileExists(filename)) remove(filename.c_str());
    std::vector<std::vector<double> > a;
    a.push_back(std::vector<double>(3,0.0));
    a.push_back(std::vector<double>(3,0.0));
    H5_Tools::write(filename, "/inputs/command", a);
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_vector_vector_of_doubles_in_hdf5_files_and_read_it_back)
{
    const std::string filename = "file.h5";
    if (H5_Tools::doesFileExists(filename)) remove(filename.c_str());
    const std::string datasetname = "/inputs/command";
    std::vector<std::vector<double> > a;
    std::vector<std::vector<double> > res;
    a.push_back(std::vector<double>{1.0, 2.0, 3.0});
    a.push_back(std::vector<double>{4.0, 5.0, 6.0});
    H5_Tools::write(filename, datasetname, a);
    H5_Tools::read(filename, datasetname, res);
    ASSERT_EQ(2, res.size());
    ASSERT_EQ(3,res.at(0).size());
    ASSERT_EQ(1.0, res[0][0]);
    ASSERT_EQ(2.0, res[0][1]);
    ASSERT_EQ(3.0, res[0][2]);
    ASSERT_EQ(4.0, res[1][0]);
    ASSERT_EQ(5.0, res[1][1]);
    ASSERT_EQ(6.0, res[1][2]);
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_vector_vector_vector_of_doubles_in_hdf5_files)
{
    const std::string filename = "file.h5";
    if (H5_Tools::doesFileExists(filename)) remove(filename.c_str());
    std::vector<std::vector<std::vector<double> > > aaa;
    std::vector<std::vector<double> > aa;
    aa.push_back(std::vector<double>(3,0.0));
    aa.push_back(std::vector<double>(3,0.0));
    aaa.push_back(aa);
    H5_Tools::write(filename, "/inputs/command", aaa);
    ASSERT_EQ(0,remove(filename.c_str()));
}

TEST_F(H5InterfaceTest, should_be_able_to_write_a_vector_vector_vector_of_doubles_in_hdf5_files_and_read_it_back)
{
    const size_t n0 = 4;
    const std::string filename = "file.h5";
    if (H5_Tools::doesFileExists(filename)) remove(filename.c_str());
    const std::string datasetname = "/inputs/command";
    std::vector<std::vector<std::vector<double> > > res;
    std::vector<std::vector<std::vector<double> > > aaa;
    std::vector<std::vector<double> >aa;
    aa.push_back(std::vector<double>{1.0, 2.0, 3.0});
    aa.push_back(std::vector<double>{4.0, 5.0, 6.0});
    for (size_t i=0;i<n0;++i) aaa.push_back(aa);
    H5_Tools::write(filename, datasetname, aaa);
    H5_Tools::read(filename, datasetname, res);
    ASSERT_EQ(4, res.size());
    ASSERT_EQ(2,res.at(0).size());
    ASSERT_EQ(3,res.at(0).at(0).size());
    for (size_t i=0;i<n0;++i)
    {
        ASSERT_EQ(1.0, res[i][0][0]);
        ASSERT_EQ(2.0, res[i][0][1]);
        ASSERT_EQ(3.0, res[i][0][2]);
        ASSERT_EQ(4.0, res[i][1][0]);
        ASSERT_EQ(5.0, res[i][1][1]);
        ASSERT_EQ(6.0, res[i][1][2]);
    }
    ASSERT_EQ(0,remove(filename.c_str()));
}
