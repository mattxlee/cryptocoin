#include "merkle_tree.h"

#include "hash_utils.h"

namespace coin {
namespace mt {

Node::Node(const data::Buffer &hash) : hash_(hash) {}

Node::Node(NodePtr left, NodePtr right) : left_(left), right_(right) {
  // Calculate hash value here.
  Hash256Builder hash_builder;
  hash_builder << left_->get_hash() << right_->get_hash();
  hash_ = hash_builder.FinalValue();
}

bool Node::HasParent() const { return parent_ != nullptr; }

NodePtr MakeMerkleTree(const std::vector<NodePtr> &vec_node) {
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
      next_vec_node.emplace_back(pnode);
      (*i)->set_parent(pnode);
      (*(i + 1))->set_parent(pnode);
      i += 2;
    } else {
      next_vec_node.push_back(*i);
      ++i;
    }
  }

  return MakeMerkleTree(next_vec_node);
}

void AddHashBelonesToNode(const NodePtr node,
                          std::vector<data::Buffer> &vec_hash) {
  if (node->IsBottom()) {
    vec_hash.push_back(node->get_hash());
    return;
  }

  // Left.
  if (node->get_left() != nullptr) {
    AddHashBelonesToNode(node->get_left(), vec_hash);
  }

  // Right.
  if (node->get_right() != nullptr) {
    AddHashBelonesToNode(node->get_right(), vec_hash);
  }
}

void CompareMerkleTree(NodePtr lhs, NodePtr rhs,
                       std::vector<data::Buffer> &vec_bad_hash,
                       std::vector<data::Buffer> &vec_miss_hash) {
  // Identical nodes?
  if ((lhs->get_hash().value == rhs->get_hash().value) ||
      (lhs == nullptr && rhs == nullptr)) {
    return;
  }

  // **** Continue child checking ****

  if (lhs == nullptr) {
    // Missing nodes (rhs)
    AddHashBelonesToNode(rhs, vec_miss_hash);
    return;
  }

  if (rhs == nullptr) {
    // Bad nodes (lhs)
    AddHashBelonesToNode(lhs, vec_bad_hash);
    return;
  }

  // Child left.
  CompareMerkleTree(lhs->get_left(), rhs->get_left(), vec_bad_hash,
                    vec_miss_hash);

  // Child right.
  CompareMerkleTree(lhs->get_right(), rhs->get_right(), vec_bad_hash,
                    vec_miss_hash);
}

}  // namespace mt
}  // namespace coin
