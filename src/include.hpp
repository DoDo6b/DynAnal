#pragma once

#include <string>
#include <random>
#include <typeinfo>
#include <memory>
#include <cxxabi.h>

#include "graphviz/include.hpp"
GraphViz GVZ ("../logs/graph.dot");

#define NAMED_VARCOLOR "\"#d0ffff\""
#define UNNANED_VARCOLOR "\"#ffd0d0\""
#define CASTED "\"#ffd000\""


std::string demangle (const char* name)
{
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle (name, NULL, NULL, &status), std::free };
    return status == 0 ? res.get () : name;
}


template <typename T>
class Var
{
public:
    //  Constructors: default, value
    Var (                std::string_view name, bool tracked = false)      : var (T()),                name (name), type ( std::move( demangle ( typeid (T).name () ) ) )
    {
        snapshot ();
    }
    Var (const T& value, std::string_view name = "", bool tracked = false) : var (value),              name (name), type ( std::move( demangle ( typeid (T).name () ) ) )
    {
        snapshot ();
    }
    Var (      T&& value, std::string_view name = "", bool tracked = false) : var (std::move (value)), name (name), type ( std::move( demangle ( typeid (T).name () ) ) )
    {
        snapshot ();
    }



    //------------------------------------------------------------------------------------------------------
    // Constructors: copy, move
    Var (const Var& other)           : var (other.var),             name (other.name),              type ( std::move( demangle ( typeid (T).name () ) ) )
    {
        snapshot ();
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"copy-constructor\" color=red penwith=3 style=solid];" << std::endl;
    }

    Var (      Var&& other) noexcept : var (std::move (other.var)), name (std::move (other.name)),  type ( std::move( demangle ( typeid (T).name () ) ) )
    {
        snapshot ();
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"move-constructor\" color=green penwith=3 style=solid];" << std::endl;
    }



    //------------------------------------------------------------------------------------------------------
    //  Destroyer
    ~Var ()
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"{RIP | 2026-2026}\", fillcolor=lightgrey];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Destroyed\" color=black penwith=3 style=solid];" << std::endl;
    }



    //------------------------------------------------------------------------------------------------------
    // Casters
    template<typename U = T, typename = std::enable_if_t<!std::is_reference_v<U> > >
    explicit operator U () const
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"{ ID | Type | Ptr }|{ " << rid << " | " << demangle(typeid (U).name ()) << " | ??? }\", fillcolor=" << CASTED << "];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Cast\" penwith=3 style=solid];" << std::endl;

        return static_cast<U> (var);
    }

    operator const T& () const
    {
        int rid = rand ();
        GVZ << "id" << rid << "[label = \"{ ID | Type | Ptr }|{ " << rid << " | " << demangle(typeid (T).name ()) << " | ??? }\", fillcolor=" << CASTED << "];" << std::endl;
        GVZ << "var" << snapshotID << "->" << "id" << rid << "[label=\"Cast\" penwith=3 style=solid];" << std::endl;

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

        GVZ << "var" << old              << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid];" << std::endl;
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid];" << std::endl;

        return *this;
    }
    Var& operator=   (const Var&& other)
    {
        var = std::move (other.var);

        int old = snapshotID;
        snapshot ();

        GVZ << "var" << old              << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid];" << std::endl;
        GVZ << "var" << other.snapshotID << "->" << "var" << snapshotID << "[label=\"move-assigned\" color=green penwith=3 style=solid];" << std::endl;

        return *this;
    }

    Var& operator=   (const T& value)
    {
        var = value;

        int rid = rand ();
        GVZ << "id" << rid << "[label = \"#"<< rid << ": " << "@" << &value << " { " << value << " }\"];" << std::endl;

        int old = snapshotID;
        snapshot ();

        GVZ << "var" << old << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid];" << std::endl;
        GVZ << "id"  << rid << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid];" << std::endl;

        return *this;
    }
    Var& operator=   (const T&& value)
    {
        var = std::move (value);

        int rid = rand ();
        GVZ << "id" << rid << "[label = \"#"<< rid << ": " << "@" << &value << " { " << value << " }\"];" << std::endl;

        int old = snapshotID;
        snapshot ();

        GVZ << "var" << old << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid];" << std::endl;
        GVZ << "id"  << rid << "->" << "var" << snapshotID << "[label=\"move-assign\" color=green penwith=3 style=solid];" << std::endl;
        
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
        GVZ << "var"  << snapshotID     << "->" << "var" << result.snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid];" << std::endl;  \
        GVZ << "var"  << rhs.snapshotID << "->" << "var" << result.snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid];" << std::endl;  \
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
        GVZ << "var"  << snapshotID     << "->" << "id" << rid << "[label=\"" << #op << "\" color=green penwith=3 style=solid];" << std::endl; \
        GVZ << "var"  << rhs.snapshotID << "->" << "id" << rid << "[label=\"" << #op << "\" color=green penwith=3 style=solid];" << std::endl; \
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
        GVZ << "var" << old            << "->" << "var" << snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid];" << std::endl;  \
        GVZ << "var" << rhs.snapshotID << "->" << "var" << snapshotID << "[label=\"" << #op << "\" color=green penwith=3 style=solid];" << std::endl;  \
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

    std::string_view name;
    std::string      type;

    int     snapshotID;

    int snapshot ()
    {
        snapshotID = rand ();
        std::string_view namestr {""}, color {""}, termName {""};

        if (name.empty () == false)
        {
            namestr  = "Name | ";
            termName = " | ";
            color    = NAMED_VARCOLOR;
        }
        else color   = UNNANED_VARCOLOR;

        GVZ << "var" << snapshotID << "[fillcolor=" << color << ", label = \"{ ID | " << namestr << "Type | Ptr }|{ " << snapshotID << " | " << name << termName << type << " | " << &var << " }\"];" << std::endl;
        return snapshotID;
    }
};

#define LETA(t, name)     Var<t> name (#name, true)
#define LET(t, name, val) Var<t> name (val, #name, true)
#define SC(t, val)        static_cast<Var<t>> (val)


class Subgraph
{
public:
    Subgraph  (const std::string& name) { GVZ.subgraphOpen  (name); }
    ~Subgraph ()                        { GVZ.subgraphClose (); }
};

#define BEG Subgraph (__func__);
