#include "base91x.hpp"
#include "generate_anthineas.hpp"
#include "stl_reader.hpp"

VectorOfVectorOfPoints anthineas() // Generated by convert_stl_files_to_code
{
    #include "anthineas_data.hpp"
    return read_binary_stl(base<91>::decode(s));
}
