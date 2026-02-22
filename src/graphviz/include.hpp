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
        << "\trankdir=TB;\n"
        << "\tcompound=true;\n"
        << "\tsplines=true;\n" 
        << "\tnodesep=0.4;\n"
        << "\tranksep=0.6;\n"
        << "\tnode [\n"
            << "\t\tshape=record,\n"
            << "\t\tstyle=filled,\n"
            << "\t\tfontname=\"Hack\",\n"
            << "\t\tfontsize=9,\n"
            << "\t\tmargin=\"0.05,0.03\",\n"
            << "\t\tpenwidth=1.0\n"
        << "\t];\n"
        << "\tedge ["
            << "\t\tfontname=\"Hack\",\n"
            << "\t\tfontsize=9,\n"
            << "\t\tarrowsize=0.7\n"
        << "\t];\n"
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
        unsigned char colorbyte = 0xFF - (0xF * depth);
        char color[7] = "";
        snprintf (color, sizeof (color), "%02x%02x%02x", colorbyte, colorbyte, colorbyte);

        stream
        << "subgraph cluster_" << name 
        << " {\n\tcolor=\"#" << color 
        << "\";\n\tstyle=rounded;"
        << "\n\tlabel=\"" << name 
        << "\";\n";

        return ++depth;
    }

    unsigned char subgraphClose ()
    {
        stream << "}\n";
        return --depth;
    }
};