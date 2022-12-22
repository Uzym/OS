#include "../include/tree.h"
#include <iostream>

bool Tree::create_node(int id, int parent)
{
    std::map <int, std::set <int>> :: iterator it;

    it = this->tree.find(id);
    if (it != this->tree.end()) {
        return false;
    }
    it = this->tree.find(parent);
    if (it == this->tree.end()) {
        return false;
    }

    std::set <int> NullSet;
    this->tree[id] = NullSet;

    this->tree[parent].insert(id);

    return true;
}

bool Tree::delete_node(int id)
{
    std::map <int, std::set <int>> :: iterator it;
    std::set <int> :: iterator it_set;

    it = this->tree.find(id);
    if (it == this->tree.end()) {
        return false;
    }
    std::set <int> node_childs = this->tree[id];

    for (const auto &iter : this->tree) {
        it_set = iter.second.find(id); 
        if (it_set != iter.second.end()) {
            this->tree[iter.first].erase(it_set);
            
            break;
        }
    }

    for (const auto &iter_child : node_childs) {
        this->delete_node(iter_child);
    }
    this->tree.erase(it);

    return true;
}

void Tree::print_tree()
{
    for (auto iter : this->tree) {
        std::cout << iter.first << ": ";
        for (auto it : iter.second) {
            std::cout << it << " ";
        }
        std::cout << "\n";
    }
}

Tree::Tree()
{
    std::set <int> NullSet;
    tree[-1] = NullSet;
}
