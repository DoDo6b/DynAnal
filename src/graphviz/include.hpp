#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <random>

class GraphViz
{
private:
    std::ofstream stream;
    const std::string fname;

    unsigned char depth{1};
public:
    GraphViz (const std::string& fname) : fname (fname)
    {
        stream.open (fname);

        stream
        << "digraph GVZ {\n" 
        << "\tnode [shape=rect, fontname=\"Hack\", style=filled];\n"
        << "\tsplines=polyline;\n"
        << "\tedge [fontname=\"Hack\", fontsize=8];\n"
        << std::endl;
    }
    ~GraphViz ()
    {
        stream << "}" << std::endl;

        if (stream.is_open ()) stream.close ();

        std::string buffer = "dot -Tpng " + fname + " -o " + fname + ".png";
        system (buffer.c_str ());
    }

    template<typename T>
    GraphViz& operator<< (const T& val)
    {
        stream << val;
        return *this;
    }

    GraphViz& operator<< (std::ostream& (*manip)(std::ostream&))
    {
        stream << manip;
        return *this;
    }

    unsigned char subgraphOpen (const std::string& name)
    {
        char colorbyte = 0xFF - (0xA * depth);
        char color[8] = "";
        snprintf (color, sizeof (color), "%02x%02x%02x", colorbyte, colorbyte, colorbyte);

        stream << "subgraph " << name << "\n{\n\tbgcolor=" << color << ";\n\tlabel=\"" << name << "\";\n";

        return ++depth;
    }

    unsigned char subgraphClose ()
    {
        stream << "}\n";
        return --depth;
    }
};