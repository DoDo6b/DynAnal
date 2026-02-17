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
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { ??? }\"];" << std::endl;
    }
    Var (const T& value, const std::string& name = "") : var (value),              name (std::move (name)), snapshotID (rand ())
    {
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << value << " }\", fillcolor=coral];" << std::endl;
    }
    Var (      T&& value, const std::string& name = "") : var (std::move (value)), name (std::move (name)), snapshotID (rand ())
    {
        GVZ << "var" << snapshotID << "[label = \"#"<< snapshotID << ": " << name << "@" << &var << " { " << value << " }\", fillcolor=green];" << std::endl;
    }



    //------------------------------------------------------------------------------------------------------
    // Constructors: copy, move
    Var (const Var& other)           : var (other.var),             name (other.name),              snapshotID (rand ())
    {
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"copy-constructor\" color=red penwith=3 style=solid arrowhead=normal];" << std::endl;
    }

    Var (      Var&& other) noexcept : var (std::move (other.var)), name (std::move (other.name)),  snapshotID (rand ())
    {
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
    operator T () const
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"Jumping into the void...\", fillcolor=yellow];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Casted\" color=yellow penwith=3 style=solid arrowhead=normal];" << std::endl;

        return static_cast<T> (var);
    }

    explicit operator const T& () const
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"Jumping into the void...\", fillcolor=yellow];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Casted\" color=yellow penwith=3 style=solid arrowhead=normal];" << std::endl;

        return var;
    }

          T* operator-> ()       { return &var; }
    const T* operator-> () const { return &var; }
          T& operator*  ()               { return  var; }
    const T& operator*  () const         { return  var; }



    //------------------------------------------------------------------------------------------------------
    // Assign operator: copy move
    Var& operator=   (const Var&  other)
    {
        var = other.var;
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"copy-assigned\" color=red penwith=3 style=solid arrowhead=normal];" << std::endl;

        return *this;
    }
    Var& operator=   (const Var&& other)
    {
        var = std::move (other.var);
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;

        return *this;
    }

    Var& operator=   (const T& value)
        {
        var = value;

        int rid = rand ();
        GVZ << "id" << rid << "[label = \"#"<< rid << ": " << "@" << &value << " { " << value << " }\"];" << std::endl;
        GVZ << "id" << rid << "->" << "var" << snapshotID << "[label=\"copy-assign\" color=red penwith=3 style=solid arrowhead=normal];" << std::endl;

        return *this;
    }
    Var& operator=   (const T&& value)
    {
        var = std::move (value);
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"#"<< rid << ": " << "@" << &value << " { " << value << " }\"];" << std::endl;
        GVZ << "id" << rid << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid arrowhead=normal];" << std::endl;
        
        return *this;
    }



    //------------------------------------------------------------------------------------------------------
    //  Arithmetic
    #define BinOP(op)                                   \
    Var operator op (const Var& rhs) const              \
    {                                                   \
        return Var<T> (var op rhs.var);                 \
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
    
    #define BoolOP(op)                                  \
    bool operator op (const Var& rhs) const             \
    {                                                   \
        return var op rhs.var;                          \
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

    #define AssOP(op)                   \
    Var& operator op (const Var& rhs)   \
    {                                   \
        var op rhs.var;                 \
        return *this;                   \
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

    int getID () { return snapshotID = rand (); }
};

#define LET(type, name, val) Var<type> name (val, #name)