#include "orbit_map.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

#include "infix_ostream_iterator.h"
#include "read_file.h"

namespace {

std::pair<std::string, std::string> splitLink(const std::string &link) {
  const std::vector<std::string> tokens = split_string(link, ")");
  assert(tokens.size() == 2);
  return std::make_pair(tokens[0], tokens[1]);
}

Node createMap(const std::vector<std::string> &links) {
  std::list<Node> freeNodes{Node{"COM"}};

  for (const auto link : links) {
    const auto temp = splitLink(link);
    const auto parentName = temp.first;
    const auto childName = temp.second;

    const auto it = std::find_if(
        freeNodes.begin(), freeNodes.end(),
        [childName](const Node &n) { return n.getName() == childName; });
    Node toInsert = (it == freeNodes.end()) ? Node{childName} : *it;
    if (it != freeNodes.end()) {
      freeNodes.erase(it);
    }

    InsertChildVisitor v{parentName, toInsert};

    for (auto &node : freeNodes) {
      if (!v.isDone()) {
        node.accept(v);
      }
    }

    if (!v.isDone()) {
      freeNodes.push_back(Node{parentName});
      freeNodes.back().addChild(toInsert);
    }
  }

  assert(freeNodes.size() == 1);
  assert(freeNodes.front().getName() == "COM");

  return freeNodes.front();
}

} // namespace

Node::Node(std::string name) : m_name{std::move(name)} {
  // empty
}

void Node::accept(INodeVisitor &visitor) {
  visitor.enter();
  visitor.visit(*this);

  for (auto &child : m_nodes) {
    child.accept(visitor);
  }
  visitor.exit();
}

InsertChildVisitor::InsertChildVisitor(std::string parentName, Node child)
    : m_parentName{std::move(parentName)},
      m_childToInsert{std::move(child)}, m_done{false} {
  // empty
}

void InsertChildVisitor::visit(Node &n) {
  if (!m_done && n.getName() == m_parentName) {
    n.addChild(m_childToInsert);
    m_done = true;
  }
}

void PrintVisitor::visit(Node &n) {
  for (const auto &child : n.getNodes()) {
    std::cout << n.getName() << " -> " << child.getName() << std::endl;
  }
  std::cout << std::endl;
}

void NumOrbitVisitor::visit(Node &n) { num += depth; }

PathFinderVisitor::PathFinderVisitor(std::string target)
    : m_target{std::move(target)} {
  // empty
}

void PathFinderVisitor::visit(Node &n) {
  if (n.getName() == m_target) {
    m_found = true;
  }

  if (m_found) {
    return;
  }

  m_path.push_back(n.getName());
}

void PathFinderVisitor::exit() {
  if (!m_found) {
    m_path.pop_back();
  }
}

OrbitMap::OrbitMap(const std::string &description)
    : m_com{createMap(split_string(description, "\n"))} {}

int OrbitMap::numOrbits() {
  NumOrbitVisitor v;
  m_com.accept(v);
  return v.getNum();
}

void OrbitMap::print() {
  PrintVisitor v;
  m_com.accept(v);
}

std::vector<std::string> OrbitMap::pathTo(const std::string &target) {
  PathFinderVisitor v{target};
  m_com.accept(v);
  return v.getPath();
}

std::vector<std::string> OrbitMap::pathBetween(const std::string &source,
                                               const std::string &target) {
  auto pathComToSource = pathTo(source);
  auto pathComToTarget = pathTo(target);

  while (pathComToSource[0] == pathComToTarget[0] &&
         pathComToSource[1] == pathComToTarget[1]) {
    pathComToSource.erase(pathComToSource.begin());
    pathComToTarget.erase(pathComToTarget.begin());
  }

  std::vector<std::string> ret;

  for (auto it = pathComToSource.rbegin(); it != pathComToSource.rend(); ++it) {
    ret.push_back(*it);
  }

  pathComToTarget.erase(pathComToTarget.begin());

  for (const auto &s : pathComToTarget) {
    ret.push_back(s);
  }

  return ret;
}

int OrbitMap::pathLength(const std::string &target) {
  return pathLength(pathTo(target));
}

int OrbitMap::pathLength(const std::string &source, const std::string &target) {
  return pathLength(pathBetween(source, target));
}

int OrbitMap::pathLength(const std::vector<std::string> &path) {
  return static_cast<int>(path.size() - 1);
}
