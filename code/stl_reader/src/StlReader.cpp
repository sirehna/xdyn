#include <cstring>
#include <string>
#include <sstream>

#include "StlReaderException.hpp"
#include "StlReader.hpp"

const int LINE_MAX_LENGTH = 256;
const char TOKEN_vertex[] = "vertex";
const char TOKEN_normal[] = "normal";

struct ParserState;
char* skipWhiteSpace(char *input);
void readVertex(char *line, Eigen::Vector3d& vertices, ParserState& state);
void readNormal(char *line, Eigen::Vector3d& vertices, ParserState& state);
void readAsciiStl(
    std::istream& input_stream,
    VectorOfPoint3dTriplet& result,
    ParserState &state);

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

char* skipWhiteSpace(char *input){
    char *next;
    for (next = input; *next == '\t' && *next == ' '; next++){}
    return next;
}
/**
 * \brief reads a vertex from a line
 */
void readVertex(char *line, Eigen::Vector3d& vertices, ParserState& state)
{
    int width;
    char token[20];
    //char *str = skipWhiteSpace(line);
    /*int res = */sscanf(line, "%s%n", token, &width);
    if (strcmp(token, TOKEN_vertex)){
        THROW(__PRETTY_FUNCTION__, StlReaderException, state.parseErrorString(TOKEN_vertex));
    }
    sscanf(line+width,"%lf %lf %lf",&vertices(0),&vertices(1),&vertices(2));
}

void readNormal(char *line, Eigen::Vector3d& vertices, ParserState& state){
    int width;
    char token[20];
    //char *str = skipWhiteSpace(line);
    /*int res = */sscanf(line, "%s%n", token, &width);
    if (strcmp(token, TOKEN_normal)){
        THROW(__PRETTY_FUNCTION__, StlReaderException, state.parseErrorString(TOKEN_normal));
    }
    sscanf(line+width,"%lf %lf %lf",&vertices(0),&vertices(1),&vertices(2));
}

/**
 * \brief reads the ASCII file from a char input stream
 */
void readAsciiStl(
    std::istream& input_stream, //!<
    VectorOfPoint3dTriplet& result,
    ParserState &state)
{
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
        Point3dTriplet face;
        if (!strcmp(token, "facet")){
            // Get the XYZ coordinates of the normal vector to the face.
            readNormal(next, normal, state);
            state.getLine(input);
            // Get the XYZ coordinates of the vertex1 vector
            state.getLine(input);
            readVertex(input,face.p1, state);
            // Get the XYZ coordinates of the vertex2 vector
            state.getLine(input);
            readVertex(input,face.p2, state);
            // Get the XYZ coordinates of the vertex3 vector
            state.getLine(input);
            readVertex(input,face.p3, state);
            // closeloop
            state.getLine(input);
            // endfacet
            state.getLine(input);
            // save the facet in the vector
            result.push_back(face);
        } else if (!strcmp(token, "color")) { // COLOR
            sscanf(next, "%*s %lf %lf %lf %lf", &r1, &r2, &r3, &r4);
        } else if (!strcmp(token, "solid")) { // SOLID
            // object_num = object_num + 1;
        } else if (!strcmp(token, "endsolid")){ // ENDSOLID
            endReached = true;
        } else { // Unexpected or unrecognized.
            setlocale(LC_ALL, locale.c_str());
            THROW(__PRETTY_FUNCTION__, StlReaderException, state.errorUnknownString(token));
        }
    }
    if(!endReached){
        THROW(__PRETTY_FUNCTION__, StlReaderException,
                "The 'endsolid' keyword was not found in end of file. "
                 "The file may be damaged or is a binary STL format. "
                 "A binary STL file must not have 'solid' keyword in header.");
    }
    setlocale(LC_ALL, locale.c_str());
    return;
}

VectorOfPoint3dTriplet read_stl(const std::string& input)
{
    std::istringstream inputStream(input);
    ParserState state(inputStream);
    VectorOfPoint3dTriplet result;
    readAsciiStl(inputStream, result,state);
    return result;
}
