#pragma once

#include <string>
#include <random>

#include "graphviz/include.hpp"
GraphViz GVZ ("../logs/graph.dot");

template <typename T>
class Var
{
public:
    //  Constructors: default, value
    Var (                const std::string& name)      : var (T()),                name (std::move (name)), snapshotID (rand ())
    {
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << var << " }\"];" << std::endl;
    }
    Var (const T& value, const std::string& name = "") : var (value),              name (std::move (name)), snapshotID (rand ())
    {
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << value << " }\", fillcolor=red];" << std::endl;
    }
    Var (      T&& value, const std::string& name = "") : var (std::move (value)), name (std::move (name)), snapshotID (rand ())
    {
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << value << " }\", fillcolor=green];" << std::endl;
    }



    //------------------------------------------------------------------------------------------------------
    // Constructors: copy, move
    Var (const Var& other)           : var (other.var),             name (other.name),              snapshotID (rand ())
    {
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << var << " }\"];" << std::endl;
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"copy-constructor\" color=red penwith=3 style=solid arrowhead=normal];" << std::endl;
    }

    Var (      Var&& other) noexcept : var (std::move (other.var)), name (std::move (other.name)),  snapshotID (rand ())
    {
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << var << " }\"];" << std::endl;
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"move-constructor\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;
    }



    //------------------------------------------------------------------------------------------------------
    //  Destroyer
    ~Var ()
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"RIP 2026-2026\", fillcolor=lightgrey];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Destroyed\" color=black penwith=3 style=solid arrowhead=normal];" << std::endl;
    }



    //------------------------------------------------------------------------------------------------------
    // Casters
    template<typename U = T, typename = std::enable_if_t<!std::is_reference_v<U> > >
    explicit operator U () const
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"Jumping into the void...\", fillcolor=orange];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Casted\" color=orange penwith=3 style=solid arrowhead=normal];" << std::endl;

        return var;
    }

    operator const T& () const
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"Jumping into the void...\", fillcolor=greenyellow];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Casted\" color=greenyellow penwith=3 style=solid arrowhead=normal];" << std::endl;

        return var;
    }

          T* operator-> ()       { return &var; }
    const T* operator-> () const { return &var; }
          T& operator*  ()       { return  var; }
    const T& operator*  () const { return  var; }



    //------------------------------------------------------------------------------------------------------
    // Assign operator: copy move
    Var& operator=   (const Var&  other)
    {
        var = other.var;
        
        int old = snapshotID;
        snapshot ();

        GVZ << "var" << old              << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;

        return *this;
    }
    Var& operator=   (const Var&& other)
    {
        var = std::move (other.var);

        int old = snapshotID;
        snapshot ();

        GVZ << "var" << old              << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;

        return *this;
    }

    Var& operator=   (const T& value)
    {
        var = value;

        int rid = rand ();
        GVZ << "id" << rid << "[label = \"#"<< rid << ": " << "@" << &value << " { " << value << " }\"];" << std::endl;

        int old = snapshotID;
        snapshot ();

        GVZ << "var" << old << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;
        GVZ << "id"  << rid << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;

        return *this;
    }
    Var& operator=   (const T&& value)
    {
        var = std::move (value);

        int rid = rand ();
        GVZ << "id" << rid << "[label = \"#"<< rid << ": " << "@" << &value << " { " << value << " }\"];" << std::endl;

        int old = snapshotID;
        snapshot ();

        GVZ << "var" << old << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;
        GVZ << "id"  << rid << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;
        
        return *this;
    }



    //------------------------------------------------------------------------------------------------------
    //  Arithmetic
    #define BinOP(op)                                                                                                                                                           \
    Var operator op (const Var& rhs) const                                                                                                                                      \
    {                                                                                                                                                                           \
                                                                                                                                                                                \
        Var<T> result (var op rhs.var);                                                                                                                                         \
                                                                                                                                                                                \
        GVZ << "var"  << snapshotID     << "->" << "var" << result.snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;  \
        GVZ << "var"  << rhs.snapshotID << "->" << "var" << result.snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;  \
                                                                                                                                                                                \
        return result;                                                                                                                                                          \
    }

    BinOP (+)
    BinOP (-)
    BinOP (*)
    BinOP (/)
    BinOP (%)
    BinOP (&)
    BinOP (|)
    BinOP (^)
    BinOP (<<)
    BinOP (>>)

    #undef BinOP
    
    #define BoolOP(op)                                                                                                                                          \
    bool operator op (const Var& rhs) const                                                                                                                     \
    {                                                                                                                                                           \
        int rid = rand ();                                                                                                                                      \
        std::string result = var op rhs.var ? "true" : "false";                                                                                                 \
        GVZ << "id" << rid << "[label = \"" << result << "\"];" << std::endl;                                                                                   \
                                                                                                                                                                \
        GVZ << "var"  << snapshotID     << "->" << "id" << rid << "[label=\"" << #op << "\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl; \
        GVZ << "var"  << rhs.snapshotID << "->" << "id" << rid << "[label=\"" << #op << "\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl; \
                                                                                                                                                                \
        return var op rhs.var;                                                                                                                                  \
    }

    BoolOP (<)
    BoolOP (<=)
    BoolOP (>)
    BoolOP (>=)
    BoolOP (==)
    BoolOP (!=)
    BoolOP (&&)
    BoolOP (||)

    #undef BoolOP

    bool operator!  () const { return !var; }
    Var  operator~  () const { return ~var; }

    #define AssOP(op)                                                                                                                                                   \
    Var& operator op (const Var& rhs)                                                                                                                                   \
    {                                                                                                                                                                   \
        var op rhs.var;                                                                                                                                                 \
                                                                                                                                                                        \
        int old = snapshotID;                                                                                                                                           \
        snapshot ();                                                                                                                                                    \
                                                                                                                                                                        \
        GVZ << "var" << old            << "->" << "var" << snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;  \
        GVZ << "var" << rhs.snapshotID << "->" << "var" << snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;  \
                                                                                                                                                                        \
                                                                                                                                                                        \
        return *this;                                                                                                                                                   \
    }

    AssOP (+=)
    AssOP (-=)
    AssOP (*=)
    AssOP (/=)
    AssOP (%=)
    AssOP (&=)
    AssOP (|=)
    AssOP (^=)
    AssOP (<<=)
    AssOP (>>=)

    #undef AssOP
    


    //------------------------------------------------------------------------------------------------------
    //  Streamer
    friend std::ostream& operator<< (std::ostream& stream, const Var& obj)
        {
        stream << obj.var;
        return stream;
    }
    friend std::istream& operator>> (std::istream& stream, const Var& obj)
    {
        stream >> obj.var;
        return stream;
    }



private:
    T var;
    std::string name;

    int snapshotID;

    int snapshot ()
    {
        snapshotID = rand ();
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << var << " }\"];" << std::endl;
        return snapshotID;
    }
};

#define LETA(t, name)     Var<t> name (#name)
#define LET(t, name, val) Var<t> name (val, #name)
#define SC(t, val) static_cast<Var<t>> (val)