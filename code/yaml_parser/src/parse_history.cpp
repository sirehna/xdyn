#include <iomanip> // std::setprecision

#include "parse_history.hpp"

#include "YamlState.hpp"
#include <ssc/json.hpp>

YamlSimServerInputs decode_YamlSimServerInputs(const std::string& json)
{
    rapidjson::Document document;
    ssc::json::parse(json, document);
    YamlSimServerInputs infos;
    if (not(document.IsObject()))
    {
        THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "JSON should be an object (i.e. within curly braces), but it's not (it's a " << ssc::json::print_type(document) << "). The JSON we're looking at was: " << ssc::json::dump(document));
    }
    if (not(document.HasMember("states")))
    {
        THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "Missing key 'states' in JSON root.")
    }
    if (not(document["states"].IsArray()))
    {
      THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "Expecting a JSON array but got '" << ssc::json::dump(document["states"]));
    }
    infos.Dt = ssc::json::find_optional_double("Dt", document, 0);
    for (rapidjson::Value& v:document["states"].GetArray())
    {
        YamlState s;
        s.t = ssc::json::find_double("t", v);
        s.x = ssc::json::find_double("x", v);
        s.y = ssc::json::find_double("y", v);
        s.z = ssc::json::find_double("z", v);
        s.u = ssc::json::find_double("u", v);
        s.v = ssc::json::find_double("v", v);
        s.w = ssc::json::find_double("w", v);
        s.p = ssc::json::find_double("p", v);
        s.q = ssc::json::find_double("q", v);
        s.r = ssc::json::find_double("r", v);
        s.qr = ssc::json::find_double("qr", v);
        s.qi = ssc::json::find_double("qi", v);
        s.qj = ssc::json::find_double("qj", v);
        s.qk = ssc::json::find_double("qk", v);
        infos.states.push_back(s);
    }
    if (document.HasMember("commands"))
    {
        const rapidjson::Value& commands = document["commands"];
        if (not(commands.IsObject()) && not(commands.IsNull()))
        {
            THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "'commands' should be a JSON object (key-values): got " << ssc::json::print_type(commands))
        }
        else
        {
            if (not(commands.IsNull()))
            {
                for (rapidjson::Value::ConstMemberIterator it = commands.MemberBegin(); it != commands.MemberEnd(); ++it)
                {
                    infos.commands[it->name.GetString()] = ssc::json::find_double(it->name.GetString(), commands);
                }
            }
        }
    }

    return infos;
}

std::ostream& operator<<(std::ostream& os, const std::map<std::string, double>& m);
std::ostream& operator<<(std::ostream& os, const std::map<std::string, double>& m)
{
    os << "{";
    const size_t n = m.size();
    size_t i = 0;
    for (const auto key_value:m)
    {
        os << "\"" << key_value.first << "\": " << key_value.second;
        if (i < n-1)
        {
            os << ", ";
        }
        i++;
    }
    os << "}";
    return os;
}

std::string encode_YamlStates(const std::vector<YamlState>& states)
{
    std::stringstream ss;
    // Set precision to shortes possible representation , without losing precision
    // Cf. https://stackoverflow.com/a/23437425, and, more specifically answer https://stackoverflow.com/a/4462034
    ss << std::defaultfloat << std::setprecision(17);
    ss << "[";
    for (size_t i = 0 ; i < states.size() ; ++i)
    {
        ss << "{"
           << "\"t\": " << states[i].t << ", "
           << "\"x\": " << states[i].x << ", "
           << "\"y\": " << states[i].y << ", "
           << "\"z\": " << states[i].z << ", "
           << "\"u\": " << states[i].u << ", "
           << "\"v\": " << states[i].v << ", "
           << "\"w\": " << states[i].w << ", "
           << "\"p\": " << states[i].p << ", "
           << "\"q\": " << states[i].q << ", "
           << "\"r\": " << states[i].r << ", "
           << "\"qr\": " << states[i].qr << ", "
           << "\"qi\": " << states[i].qi << ", "
           << "\"qj\": " << states[i].qj << ", "
           << "\"qk\": " << states[i].qk << ", "
           << "\"phi\": " << states[i].phi << ", "
           << "\"theta\": " << states[i].theta << ", "
           << "\"psi\": " << states[i].psi << ", "
           << "\"extra_observations\": " << states[i].extra_observations
           << "}";
        if (i < states.size()-1) ss << ",";
        ss << std::endl;
    }
    ss << "]";
    return ss.str();
}
