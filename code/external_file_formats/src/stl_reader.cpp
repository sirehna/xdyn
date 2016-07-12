#include <cstring>
#include <string>
#include <sstream>
#include <cstdio>

#include "MeshException.hpp"
#include "stl_reader.hpp"

const int LINE_MAX_LENGTH = 256;
const char TOKEN_vertex[] = "vertex";
const char TOKEN_normal[] = "normal";

struct ParserState;
EPoint readVertex(char *line, ParserState& state);
void readNormal(char *line, Eigen::Vector3d& vertices, ParserState& state);
VectorOfVectorOfPoints readAsciiStl(std::istream& input_stream, ParserState &state);

struct ParserState {
    ParserState(std::istream& istream):
        input_stream(istream){}

    std::string parseErrorString(const std::string &token){
        std::ostringstream ostr;
        ostr << "Error parsing \""<< token;
        return ostr.str();
    }

    std::string errorUnknownString(const std::string &token){
        std::ostringstream ostr;
        ostr << "Error unknown token "<< token;
        return ostr.str();
    }

    void getLine(char *line){
        input_stream.getline(line, LINE_MAX_LENGTH);
    }

    std::istream& input_stream;
};

/**
 * \brief reads a vertex from a line
 */
EPoint readVertex(char *line, ParserState& state)
{
    EPoint ret;
    int width;
    char token[20] = "";
    /*int res = */sscanf(line, "%s%n", token, &width);
    if (strcmp(token, TOKEN_vertex))
    {
        THROW(__PRETTY_FUNCTION__, MeshException, state.parseErrorString(TOKEN_vertex));
    }
    sscanf(line+width,"%lf %lf %lf",&ret(0),&ret(1),&ret(2));
    return ret;
}

void readNormal(char *line, Eigen::Vector3d& vertices, ParserState& state){
    int width;
    char token[20];
    /*int res = */sscanf(line, "%s%n", token, &width);
    if (strcmp(token, TOKEN_normal)){
        THROW(__PRETTY_FUNCTION__, MeshException, state.parseErrorString(TOKEN_normal));
    }
    sscanf(line+width,"%lf %lf %lf",&vertices(0),&vertices(1),&vertices(2));
}

/**
 * \brief reads the ASCII file from a char input stream
 */
VectorOfVectorOfPoints readAsciiStl(
    std::istream& input_stream, //!<
    ParserState &state)
{
    VectorOfVectorOfPoints result;
    //Start by storing the current locale. This is retrieved by passing NULL to
    // setlocale
    std::string locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "C");
    double r1 = 0.0, r2 = 0.0, r3 = 0.0, r4 = 0.0;
    bool endReached = false;
    char token[LINE_MAX_LENGTH];
    int  width = 0;
    char *next = NULL;
    char input[LINE_MAX_LENGTH];
    state.getLine(input);
    Eigen::Vector3d normal;

    // while characters still exists and no errors occurs
    while (input_stream.fail() == 0 && input_stream.eof() == 0) {
        // Read the next line of the file into INPUT.
        state.getLine(input);

        // Advance to the first nonspace character in INPUT.
        // 32==SPACE character
        for (next = input; *next != '\0' && *next == 32; next++){}
        // Skip blank lines and comments and linebreaks.
        if (*next == '\0' || *next == '#' || *next == '!' || *next == '$') {
            continue;
        }
        // Extract the first word in this line.
        sscanf(next, "%s%n", token, &width);
        // Set NEXT to point to just after this token.
        next = next + width;
        // FACET
        VectorOfPoints facet;
        if (!strcmp(token, "facet")){
            VectorOfPoints facet;
            // Get the XYZ coordinates of the normal vector to the face.
            readNormal(next, normal, state);
            state.getLine(input);
            // Get the XYZ coordinates of the vertex1 vector
            state.getLine(input);
            facet.push_back(readVertex(input, state));
            // Get the XYZ coordinates of the vertex2 vector
            state.getLine(input);
            facet.push_back(readVertex(input, state));
            // Get the XYZ coordinates of the vertex3 vector
            state.getLine(input);
            facet.push_back(readVertex(input, state));
            // closeloop
            state.getLine(input);
            // endfacet
            state.getLine(input);
            //
            result.push_back(facet);
        } else if (!strcmp(token, "color")) { // COLOR
            sscanf(next, "%*s %lf %lf %lf %lf", &r1, &r2, &r3, &r4);
        } else if (!strcmp(token, "solid")) { // SOLID
            // object_num = object_num + 1;
        } else if (!strcmp(token, "endsolid")){ // ENDSOLID
            endReached = true;
        } else { // Unexpected or unrecognized.
            setlocale(LC_ALL, locale.c_str());
            THROW(__PRETTY_FUNCTION__, MeshException, state.errorUnknownString(token));
        }
    }
    if(!endReached){
        THROW(__PRETTY_FUNCTION__, MeshException,
                "The 'endsolid' keyword was not found before the end of file. "
                 "The file may be damaged or is a binary STL format. "
                 "A binary STL file must not have 'solid' keyword in header.");
    }
    setlocale(LC_ALL, locale.c_str());
    return result;
}

bool startsWith(const std::string& input, const std::string& pattern);
bool startsWith(const std::string& input, const std::string& pattern)
{
    return pattern.size() <= input.size() && input.compare(0, pattern.size(), pattern) == 0;
}

std::string removeStartingSpacesFromString(const std::string& str,
                 const std::string& whitespace = " \t");
std::string removeStartingSpacesFromString(const std::string& str,
                 const std::string& whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) return ""; // no content
    return str.substr(strBegin);
}

bool isStlDataBinary(const std::string& input)
{
    const std::string trimedInput = removeStartingSpacesFromString(input);
    if (startsWith(trimedInput, "solid ") || startsWith(trimedInput, "SOLID "))
    {
        return false;
    }
    else
    {
        return true;
    }
}

VectorOfVectorOfPoints read_stl(const std::string& input)
{
    if (isStlDataBinary(input))
    {
        return read_binary_stl(input);
    }
    else
    {
        std::istringstream inputStream(input);
        ParserState state(inputStream);
        return readAsciiStl(inputStream, state);
    }
}

VectorOfVectorOfPoints read_binary_stl(std::istream& stream) // Shamelessly copied from http://ravehgonen.wordpress.com/tag/stl-file-format/
{
    char buffer[4] = {0};
    VectorOfVectorOfPoints ret;

    stream.ignore(80);
    stream.read(buffer, sizeof buffer);
    unsigned int nFaces = 0;
    memcpy((void*)&nFaces, (void*)&buffer, sizeof(buffer));

    float v[12]; // normal=3 + vertices=3*3 = 12

    // Every Face is 50 Bytes: Normal(3*float), Vertices(9*float), 2 Bytes Spacer
    for (size_t i=0; i<nFaces; ++i)
    {
        if (stream.good())
        {
            stream.ignore(12); // Ignore the normal (MeshBuilder recalculates it anyway)
            for (size_t j=0; j<9; ++j)
            {
                // As per http://stackoverflow.com/a/12861635
                for (size_t k = 0 ; k < sizeof buffer ; ++k)
                {
                    char c;
                    stream.get(c);
                    *(((char*)&(v[j])) + k) = c;//fgetc(buffer);
                }
            }
            stream.ignore(2); // Ignore separator
        }

       VectorOfPoints t;
       t.push_back(EPoint(v[0], v[1], v[2]));
       t.push_back(EPoint(v[3], v[4], v[5]));
       t.push_back(EPoint(v[6], v[7], v[8]));
       ret.push_back(t);
    }
    return ret;
}

VectorOfVectorOfPoints read_binary_stl(const std::string& input)
{
    std::stringstream ss(input);
    return read_binary_stl(ss);
}

std::string replace(char c, const std::string& replacement, const std::string& s);
std::string replace(char c, const std::string& replacement, const std::string& s)
{
    std::string result;
    size_t searchStartPos = 0;

    std::string chars = std::string("\\") + c;
    size_t pos = s.find_first_of(chars);
    while (pos != std::string::npos)
    {
        result += s.substr(searchStartPos, pos - searchStartPos);
        if (s[pos] == '\\')
        {
            result += std::string("\\") + c;
            searchStartPos = pos + 2;
        }
        else if (s[pos] == c)
        {
            result += replacement;
            searchStartPos = pos + 1;
        }

        pos = s.find_first_of(chars, searchStartPos);
    }
    return result;
}

std::string escape_backslashes(const std::string& s);
std::string escape_backslashes(const std::string& s)
{
    return replace('\\', "\\\\", s);
}
