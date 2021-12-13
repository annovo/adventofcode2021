#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>

const std::string START = "start";
const std::string END = "end";

class Graph
{
public:
  int dfs(const std::string &v, bool part_one)
  {
    if (v == END)
      return 1;

    if (m_marked.find(v) != m_marked.end())
    {
      if (part_one || v == START || m_visited_small != "")
        return 0;
      m_visited_small = v;
    }

    if (islower(v.at(0)))
      m_marked.insert(v);

    auto it = m_g.find(v)->second;
    int r = 0;
    for (size_t i = 0; i < it.size(); i++)
    {
      r += dfs(it[i], part_one);
    }
    if (!part_one && m_visited_small == v)
      m_visited_small = "";
    else
      m_marked.erase(v);
    return r;
  }

  int find_all_paths(bool part_one)
  {
    m_marked = {};
    return dfs(START, part_one);
  }

  void add_edge(const std::string &a, const std::string &b)
  {
    if (m_g.find(a) == m_g.end())
      m_g.insert({a, std::vector<std::string>()});
    if (m_g.find(b) == m_g.end())
      m_g.insert({b, std::vector<std::string>()});

    m_g.find(a)->second.push_back(b);
    m_g.find(b)->second.push_back(a);
  }

private:
  std::unordered_map<std::string, std::vector<std::string>> m_g;
  std::unordered_set<std::string> m_marked;
  std::string m_visited_small = "";
};

void split(const std::string &row, Graph &g)
{
  std::string symbol_a;
  std::string symbol_b;
  bool is_a = true;
  for (char c : row)
  {
    if (c == '-')
    {
      is_a = false;
      continue;
    }

    if (is_a)
      symbol_a.push_back(c);
    else
      symbol_b.push_back(c);
  }
  g.add_edge(symbol_a, symbol_b);
}

void solve(const std::string &fileName)
{
  Graph g;
  std::ifstream in(fileName);
  std::string row;
  while (!in.eof())
  {
    std::getline(in, row);
    split(row, g);
  }

  std::cout << g.find_all_paths(true) << "-" << g.find_all_paths(false) << "\n";
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}