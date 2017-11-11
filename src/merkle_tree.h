#ifndef __MERKLE_TREE_H__
#define __MERKLE_TREE_H__

#include <memory>
#include <vector>

#include "data_value.h"
#include "hash_utils.h"

namespace coin {
namespace mt {

template <typename T>
class Node {
 public:
  typedef std::shared_ptr<Node<T>> NodePtr;

 public:
  /**
   * Make a single node with data.
   *
   * If a tree node construct by data of type `T`, means this node is leaf,
   * and hash value is calculated from pdata_
   *
   * @param data Data value.
   */
  explicit Node(const T &data) {
    pdata_ = std::make_shared<T>(data);
    hash_ = data.CalcHash();  // Calculate hash value.
  }

  /**
   * Make a parent node of two child node(left and right).
   *
   * @param left Left child.
   * @param right Right child.
   * @param parent Parent child.
   */
  Node(NodePtr left, NodePtr right) : left_(left), right_(right) {
    if (left_ != nullptr && right_ != nullptr) {
      // Both exist.
      Hash256Builder hash_builder;
      hash_builder << left_->get_hash() << right_->get_hash();
      hash_ = hash_builder.FinalValue();
    } else if (left_ != nullptr) {
      hash_ = left_->get_hash();
    } else if (right_ != nullptr) {
      hash_ = right_->get_hash();
    } else {
      // TODO Both nullptr?
    }
  }

  /// Get hash value.
  const data::Buffer &get_hash() const { return hash_; }

  /// Assign data.
  void set_data(const T &data) { pdata_ = std::make_shared<T>(data); }

  /// Get data.
  const std::shared_ptr<T> &get_data() const { return pdata_; }

  /// Set parent node.
  void set_parent(NodePtr parent) { parent_ = parent; }

  /// Get parent node.
  NodePtr get_parent() const { return parent_; }

  /// Get left node.
  NodePtr get_left() const { return left_; }

  /// Get right node.
  NodePtr get_right() const { return right_; }

  /// Make merkle-tree
  static NodePtr MakeMerkleTree(const std::vector<NodePtr> &vec_node) {
    if (vec_node.empty()) return nullptr;

    if (vec_node.size() == 1) {
      // Done.
      return vec_node[0];
    }

    std::vector<NodePtr> next_vec_node;

    // Get and hash.
    auto i = std::begin(vec_node);
    while (i != std::end(vec_node)) {
      NodePtr left = *i;
      if (i + 1 != std::end(vec_node)) {
        auto pnode = std::make_shared<Node>(*i, *(i + 1));
        next_vec_node.push_back(pnode);
        (*i)->set_parent(pnode);
        (*(i + 1))->set_parent(pnode);
        i += 2;
      } else {
        next_vec_node.push_back(*i);
        ++i;
      }
    }

    // Build tree in next depth.
    return MakeMerkleTree(next_vec_node);
  }

 private:
  NodePtr left_;
  NodePtr right_;
  NodePtr parent_;
  std::shared_ptr<T> pdata_;
  data::Buffer hash_;
};

/**
 * Make a new merkle-tree.
 *
 * @param container Merkle-tree make from values from this container.
 *
 * @return Root node of merkle-tree
 */
template <typename Container>
typename Node<typename Container::value_type>::NodePtr MakeMerkleTree(
    const Container &container) {
  // Build first-level tree nodes.
  std::vector<typename Node<typename Container::value_type>::NodePtr> vec_node;
  // Loop each values from container.
  for (const typename Container::value_type &val : container) {
    vec_node.emplace_back(
        std::make_shared<Node<typename Container::value_type>>(val));
  }
  return Node<typename Container::value_type>::MakeMerkleTree(vec_node);
}

}  // namespace mt
}  // namespace coin

#endif
