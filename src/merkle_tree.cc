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

}  // namespace mt
}  // namespace coin
