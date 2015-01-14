#include "ListOfObservers.hpp"
#include "ListOfObserversTest.hpp"
#include "yaml_data.hpp"
#include "parse_output.hpp"

TEST_F(ListOfObserversTest, toto)
{
    ListOfObservers s(parse_output(test_data::full_example()));
}
