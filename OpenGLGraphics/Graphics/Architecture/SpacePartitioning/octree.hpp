#ifndef _OCTREE__HPP_
#define _OCTREE__HPP_

#include "shapes.hpp"
#include <unordered_map>
#include <memory>

namespace LocationalCode {
    static const unsigned maxBits = sizeof(unsigned) * 8;

    template<unsigned dimension = 3>
    int compute_locational_code(glm::vec<dimension, long long> pos, const unsigned root_size, const unsigned levels);

    template<unsigned dimension = 3>
    int common_locational_code(unsigned loc1, unsigned loc2);

    int compute_locational_code(const aabb& bv, const unsigned root_size, const unsigned levels);

    aabb compute_bv(unsigned loc, float size);
}

/**
 * @brief
 * 	Linear octree, each node stores a head for a linked list of T
 * @tparam T
 */
template <typename T>
class Octree
{
public:
    struct node
    {
        unsigned int locational_code = 0;
        unsigned char  children_active = 0;
        //T* first = nullptr;
		std::shared_ptr<T> first = nullptr;
    };

    std::unordered_map<unsigned int, node*> m_nodes;
private:
    unsigned int m_root_size;
    unsigned int m_levels;

public:
    ~Octree();

    void clear();
    node* create_node(const aabb& bv);
    node* create_node(unsigned int loc);
    node* find_node(const aabb& bv)const;
    node* find_node(unsigned int loc)const;
    void delete_node(unsigned int loc);
    void children_nodes(node* n, std::vector<node*>& childrens, int level)const;
    void set_root_size(unsigned s);
    void set_levels(unsigned l);
    [[nodiscard]] unsigned root_size() const { return m_root_size; }
};

#include "Octree.inl"

template<typename T>
Octree<T>::~Octree()
{
    clear();
}

template<typename T>
void Octree<T>::clear()
{
    for (auto& it : m_nodes)
        delete it.second;
    m_nodes.clear();
}

template<typename T>
typename Octree<T>::node* Octree<T>::create_node(const aabb& bv)
{
    return create_node(LocationalCode::compute_locational_code(bv, m_root_size, m_levels));
}

template<typename T>
typename Octree<T>::node* Octree<T>::create_node(unsigned int loc)
{
    if (loc == 0u)
        return nullptr;

    node* n = find_node(loc);
    if (n)
        return n;

    m_nodes[loc] = n = new node;
    n->locational_code = loc;
    n->children_active = 0;
    n->first = nullptr;

    if (loc == 0b1)
        return n;

    node* parent = create_node(loc >> 3);
    if (parent)
    {
        int childLoc = (loc & 0b111);
        parent->children_active |= 1u << childLoc;
    }

    return n;
}

template<typename T>
typename Octree<T>::node* Octree<T>::find_node(const aabb& bv) const
{
    return find_node(LocationalCode::compute_locational_code(bv, m_root_size, m_levels));
}

template<typename T>
typename Octree<T>::node* Octree<T>::find_node(unsigned int loc)const
{
    auto found = m_nodes.find(loc);
    if (found != m_nodes.end())
        return found->second;
    return nullptr;
}

template<typename T>
void Octree<T>::delete_node(unsigned int loc)
{
    auto found = m_nodes.find(loc);
    if (found != m_nodes.end())
    {
        delete found->second;
        m_nodes.erase(found);
    }
}

template<typename T>
void Octree<T>::children_nodes(node* n, std::vector<node*>& childrens, int level)const
{
    if (level == 0)
    {
        childrens.push_back(n);
        return;
    }
    if (level < 0)
    {
        childrens.push_back(n);
        if (n->children_active == 0)
            return;
    }

    for (int i = 0; i < 8; i++)
        if (n->children_active & (1 << i))
        {
            int lc = (n->locational_code << 3) + i;

            node* found = find_node(lc);
            if (found)
                children_nodes(found, childrens, level - 1);
        }
}

template<typename T>
void Octree<T>::set_root_size(unsigned s)
{
    m_root_size = s;
}

template<typename T>
void Octree<T>::set_levels(unsigned l)
{
    m_levels = l;
}

#endif