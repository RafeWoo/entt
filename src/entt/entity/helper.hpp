#ifndef ENTT_ENTITY_HELPER_HPP
#define ENTT_ENTITY_HELPER_HPP


#include <type_traits>
#include "../core/type_traits.hpp"
#include "../config/config.h"
#include "fwd.hpp"


namespace entt {


/**
 * @brief Alias for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
struct exclude_t: type_list<Type...> {};


/**
 * @brief Variable template for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
constexpr exclude_t<Type...> exclude{};


/**
 * @brief Alias for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
struct get_t: type_list<Type...>{};


/**
 * @brief Variable template for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
constexpr get_t<Type...> get{};


/**
 * @brief Converts a registry to a view.
 * @tparam Const Constness of the accepted registry.
 * @tparam Entity A valid entity type (see entt_traits for more details).
 */
template<bool Const, typename Entity>
struct as_view {
    /*! @brief Type of registry to convert. */
    using registry_type = std::conditional_t<Const, const entt::basic_registry<Entity>, entt::basic_registry<Entity>>;

    /**
     * @brief Constructs a converter for a given registry.
     * @param source A valid reference to a registry.
     */
    as_view(registry_type &source) ENTT_NOEXCEPT: reg{source} {}

    /**
     * @brief Conversion function from a registry to a view.
     * @tparam Exclude Types of components used to filter the view.
     * @tparam Component Type of components used to construct the view.
     * @return A newly created view.
     */
    template<typename Exclude, typename... Component>
    operator entt::basic_view<Entity, Exclude, Component...>() const {
        return reg.template view<Component...>(Exclude{});
    }

private:
    registry_type &reg;
};


/**
 * @brief Deduction guide.
 *
 * It allows to deduce the constness of a registry directly from the instance
 * provided to the constructor.
 *
 * @tparam Entity A valid entity type (see entt_traits for more details).
 */
template<typename Entity>
as_view(basic_registry<Entity> &) ENTT_NOEXCEPT -> as_view<false, Entity>;


/*! @copydoc as_view */
template<typename Entity>
as_view(const basic_registry<Entity> &) ENTT_NOEXCEPT -> as_view<true, Entity>;


/**
 * @brief Converts a registry to a group.
 * @tparam Const Constness of the accepted registry.
 * @tparam Entity A valid entity type (see entt_traits for more details).
 */
template<bool Const, typename Entity>
struct as_group {
    /*! @brief Type of registry to convert. */
    using registry_type = std::conditional_t<Const, const entt::basic_registry<Entity>, entt::basic_registry<Entity>>;

    /**
     * @brief Constructs a converter for a given registry.
     * @param source A valid reference to a registry.
     */
    as_group(registry_type &source) ENTT_NOEXCEPT: reg{source} {}

    /**
     * @brief Conversion function from a registry to a group.
     * @tparam Exclude Types of components used to filter the group.
     * @tparam Get Types of components observed by the group.
     * @tparam Owned Types of components owned by the group.
     * @return A newly created group.
     */
    template<typename Exclude, typename Get, typename... Owned>
    operator entt::basic_group<Entity, Exclude, Get, Owned...>() const {
        return reg.template group<Owned...>(Get{}, Exclude{});
    }

private:
    registry_type &reg;
};


/**
 * @brief Deduction guide.
 *
 * It allows to deduce the constness of a registry directly from the instance
 * provided to the constructor.
 *
 * @tparam Entity A valid entity type (see entt_traits for more details).
 */
template<typename Entity>
as_group(basic_registry<Entity> &) ENTT_NOEXCEPT -> as_group<false, Entity>;


/*! @copydoc as_group */
template<typename Entity>
as_group(const basic_registry<Entity> &) ENTT_NOEXCEPT -> as_group<true, Entity>;


/**
 * @brief Alias template to ease the assignment of tags to entities.
 *
 * If used in combination with hashed strings, it simplifies the assignment of
 * tags to entities and the use of tags in general where a type would be
 * required otherwise.<br/>
 * As an example and where the user defined literal for hashed strings hasn't
 * been changed:
 * @code{.cpp}
 * entt::registry registry;
 * registry.assign<entt::tag<"enemy"_hs>>(entity);
 * @endcode
 *
 * @note
 * Tags are empty components and therefore candidates for the empty component
 * optimization.
 *
 * @tparam Value The numeric representation of an instance of hashed string.
 */
template<ENTT_ID_TYPE Value>
using tag = std::integral_constant<ENTT_ID_TYPE, Value>;


}


#endif
