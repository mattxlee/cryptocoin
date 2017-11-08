#ifndef __MERKLE_TREE_H__
#define __MERKLE_TREE_H__

#include <memory>
#include <vector>

#include "data_value.h"

namespace coin {
namespace mt {

class Node;
typedef std::shared_ptr<Node> NodePtr;

class Node {
 public:
  /**
   * @brief Make a single node with data.
   *
   * @param data Data to hash.
   */
  explicit Node(const data::Buffer &hash);

  /**
   * @brief Constructor for Node.
   *
   * @param left Left child.
   * @param right Right child.
   * @param parent Parent child.
   */
  Node(NodePtr left, NodePtr right);

  /// Get hash value.
  const data::Buffer &get_hash() const { return hash_; }

  /// Set parent value.
  void set_parent(NodePtr parent) { parent_ = parent; }

  /// Returns true if parent setup.
  bool HasParent() const;

 private:
  NodePtr left_;
  NodePtr right_;
  NodePtr parent_;
  data::Buffer hash_;
};

NodePtr MakeMerkleTree(const std::vector<NodePtr> &vec_node);

template <typename Container>
NodePtr MakeMerkleTree(const Container &container) {
  std::vector<NodePtr> vec_node;
  for (auto &val : container) {
    vec_node.emplace_back(std::make_shared<Node>(val.CalcHash()));
  }
  return MakeMerkleTree(vec_node);
}

}  // namespace mt
}  // namespace coin

#endif
