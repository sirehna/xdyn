#include "ListOfObservers.hpp"
#include "ListOfObserversTest.hpp"
#include "yaml_data.hpp"
#include "parse_output.hpp"

TEST_F(ListOfObserversTest, should_be_able_to_create_a_list_of_observers)
{
    const std::vector<YamlOutput> yaml = parse_output(test_data::full_example());
    {
        ListOfObservers s(yaml);
    }
    for (auto output:yaml)
    {
        if (output.format == "hdf5")
        {
            EXPECT_EQ(0,remove(output.filename.c_str()));
        }
    }
}
