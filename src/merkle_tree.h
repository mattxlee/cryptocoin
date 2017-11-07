#ifndef __MERKLE_TREE_H__
#define __MERKLE_TREE_H__

#include <memory>
#include <vector>

#include "hash_utils.h"

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
  explicit Node(const DataValue &data);

  /**
   * @brief Constructor for Node.
   *
   * @param left Left child.
   * @param right Right child.
   * @param parent Parent child.
   */
  Node(NodePtr left, NodePtr right, NodePtr parent);

  /// Get hash value.
  const DataValue &get_hash() const;

  /// Set parent value.
  void set_parent(NodePtr parent) { parent_ = parent; }

  /// Returns true if parent setup.
  bool HasParent() const;

 private:
  NodePtr left_;
  NodePtr right_;
  NodePtr parent_;
  DataValue hash_;
};

NodePtr MakeMerkleTree(const std::vector<NodePtr> &vec_node);

NodePtr MakeMerkleTree(const std::vector<DataValue> &vec_data);

}  // namespace mt
}  // namespace coin

#endif
