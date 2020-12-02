#pragma once

#include <string>
#include <vector>
#include <list>

// forward declaration
class INodeVisitor;


class Node
{
public:
  Node(std::string name);

  void accept(INodeVisitor & visitor);

  void addChild(const Node & node) { m_nodes.push_back(node); }

  const auto & getName() const { return m_name; }
  const auto & getNodes() const { return m_nodes; }

  bool operator==(const Node & other) { return m_name == other.getName(); }

private:
  std::string m_name;
  std::list<Node> m_nodes;
};


class INodeVisitor
{
public:
  virtual ~INodeVisitor() = default;
  virtual void visit(Node & n) = 0;
  virtual void enter() {}
  virtual void exit() {}
};


class InsertChildVisitor : public INodeVisitor
{
public:
  InsertChildVisitor(std::string parentName, Node child);
  ~InsertChildVisitor() = default;

  void visit(Node & n) override;

  bool isDone() const { return m_done; }

private:
  std::string m_parentName;
  Node m_childToInsert;

  bool m_done;
};


class PrintVisitor : public INodeVisitor
{
public:
  ~PrintVisitor() = default;

  void visit(Node & n) override;
};


class NumOrbitVisitor : public INodeVisitor
{
public:
  ~NumOrbitVisitor() = default;
  void visit(Node & n) override;

  void enter() override { depth += 1;}
  void exit() override { depth -= 1;}

  int getNum() const { return num; }

private:
  int depth = -1;
  int num = 0;
};

class PathFinderVisitor : public INodeVisitor
{
public:
  PathFinderVisitor(std::string target);
  ~PathFinderVisitor() = default;

  void visit(Node & n) override;
  void exit() override;

  const auto & getPath() const { return m_path; }

private:
  std::string m_target;
  std::vector<std::string> m_path;
  bool m_found = false;
};


class OrbitMap
{
public:
  OrbitMap(const std::string & description);

  int numOrbits();
  void print();

  std::vector<std::string> pathTo(const std::string & target);
  std::vector<std::string> pathBetween(const std::string & source, const std::string & target);

  int pathLength(const std::string & target);
  int pathLength(const std::string & source, const std::string & target);
  int pathLength(const std::vector<std::string> & path);

private:
  Node m_com;
};