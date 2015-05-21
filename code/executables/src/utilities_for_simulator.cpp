#include "YamlOutput.hpp"
#include "parse_output.hpp"
#include "utilities_for_simulator.hpp"

YamlOutput create_a_wave_observer(const InputData& input_data);
YamlOutput create_a_wave_observer(const InputData& input_data)
{
    YamlOutput o;
    o.data = {"waves"};
    o.filename = input_data.wave_output;
    o.format = get_format(input_data.wave_output);
    return o;
}

void add_observers_from_cli_with_output_filename(
        const std::string& yaml,
        const InputData& input_data,
        std::vector<YamlOutput>& out);
void add_observers_from_cli_with_output_filename(
        const std::string& yaml,
        const InputData& input_data,
        std::vector<YamlOutput>& out)
{
    YamlOutput outputterCli = generate_default_outputter_with_all_states_in_it(yaml,input_data.output_filename);
    if (not(input_data.wave_output.empty()))
    {
        if (outputterCli.format=="ws")
        {
            outputterCli.data.push_back("waves");
        }
        else
        {
            out.push_back(create_a_wave_observer(input_data));
        }
    }
    out.push_back(outputterCli);
}

void add_observers_from_cli(
        const std::string& yaml,
        const InputData& input_data,
        std::vector<YamlOutput>& out);

void add_observers_from_cli(
        const std::string& yaml,
        const InputData& input_data,
        std::vector<YamlOutput>& out)
{
    if (not(input_data.output_filename.empty()))
    {
        add_observers_from_cli_with_output_filename(yaml, input_data, out);
    }
    else if (not(input_data.wave_output.empty()))
    {
        out.push_back(create_a_wave_observer(input_data));
    }
}

ListOfObservers get_observers(const std::string& yaml, const InputData& input_data)
{
    auto out = parse_output(yaml);
    add_observers_from_cli(yaml, input_data, out);
    return ListOfObservers(out);
}
