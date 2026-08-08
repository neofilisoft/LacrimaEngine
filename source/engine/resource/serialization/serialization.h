#pragma once

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rttr/registration>
#include <rttr/registration_friend.h>

// register all user defined serialization
namespace cereal
{
    // GLM vector serialization
    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tvec2<T, P> const& v)
    {
        ar(cereal::make_nvp("x", v.x),
           cereal::make_nvp("y", v.y));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tvec2<T, P>& v)
    {
        ar(cereal::make_nvp("x", v.x),
           cereal::make_nvp("y", v.y));
    }

    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tvec3<T, P> const& v)
    {
        ar(cereal::make_nvp("x", v.x),
           cereal::make_nvp("y", v.y),
           cereal::make_nvp("z", v.z));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tvec3<T, P>& v)
    {
        ar(cereal::make_nvp("x", v.x),
           cereal::make_nvp("y", v.y),
           cereal::make_nvp("z", v.z));
    }

    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tvec4<T, P> const& v)
    {
        ar(cereal::make_nvp("x", v.x),
           cereal::make_nvp("y", v.y),
           cereal::make_nvp("z", v.z),
           cereal::make_nvp("w", v.w));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tvec4<T, P>& v)
    {
        ar(cereal::make_nvp("x", v.x),
           cereal::make_nvp("y", v.y),
           cereal::make_nvp("z", v.z),
           cereal::make_nvp("w", v.w));
    }

    // GLM quaternion serialization
    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tquat<T, P> const& q)
    {
        ar(cereal::make_nvp("x", q.x),
           cereal::make_nvp("y", q.y),
           cereal::make_nvp("z", q.z),
           cereal::make_nvp("w", q.w));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tquat<T, P>& q)
    {
        ar(cereal::make_nvp("x", q.x),
           cereal::make_nvp("y", q.y),
           cereal::make_nvp("z", q.z),
           cereal::make_nvp("w", q.w));
    }

    // GLM matrix serialization (column-major)
    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tmat2x2<T, P> const& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tmat2x2<T, P>& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]));
    }

    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tmat2x3<T, P> const& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tmat2x3<T, P>& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]));
    }

    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tmat3x2<T, P> const& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tmat3x2<T, P>& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]));
    }

    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tmat3x3<T, P> const& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tmat3x3<T, P>& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]));
    }

    template<class Archive, typename T, glm::precision P>
    void save(Archive & ar, glm::tmat4x4<T, P> const& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]),
           cereal::make_nvp("col3", m[3]));
    }

    template<class Archive, typename T, glm::precision P>
    void load(Archive & ar, glm::tmat4x4<T, P>& m)
    {
        ar(cereal::make_nvp("col0", m[0]),
           cereal::make_nvp("col1", m[1]),
           cereal::make_nvp("col2", m[2]),
           cereal::make_nvp("col3", m[3]));
    }
} // namespace cereal