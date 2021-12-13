#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <utility>

const std::pair<int, int> START(0, 0);
struct pair_hash
{
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const
  {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    return h1 ^ h2;
  }
};

std::pair<int, int> operator+(const std::pair<int, int> &l, const std::pair<int, int> &r)
{
  return {l.first + r.first, l.second + r.second};
}

std::pair<int, int> operator-(const std::pair<int, int> &l, const std::pair<int, int> &r)
{
  int first = l.first > r.first ? l.first - r.first : r.first - l.first;
  int second = l.second > r.second ? l.second - r.second : r.second - l.second;
  return {first, second};
}

using Unordered_set = std::unordered_set<std::pair<int, int>, pair_hash>;

void split(const std::string &row, Unordered_set &s)
{
  std::string symbol_x;
  std::string symbol_y;
  bool is_x = true;
  for (char c : row)
  {
    if (c == ',')
    {
      is_x = false;
      continue;
    }

    if (is_x)
      symbol_x.push_back(c);
    else
      symbol_y.push_back(c);
  }
  s.insert({std::stoi(symbol_x), std::stoi(symbol_y)});
}

void split_fold(const std::string &row, std::vector<std::pair<int, int>> &fold)
{
  std::string symbol;
  bool is_x = false;
  bool is_num = false;
  for (char c : row)
  {
    if (c == 'x')
    {
      is_x = true;
      continue;
    }

    if (c == '=')
    {
      is_num = true;
      continue;
    }

    if (is_num)
      symbol.push_back(c);
  }

  if (is_x)
    fold.push_back({std::stoi(symbol), 0});
  else
    fold.push_back({0, std::stoi(symbol)});
}

Unordered_set do_fold(const std::pair<int, int> &fold_p, const Unordered_set &s)
{
  auto it = s.begin();
  Unordered_set n_set;
  while (it != s.end())
  {
    if ((fold_p.first == 0 && fold_p.second >= it->second) || (fold_p.second == 0 && fold_p.first >= it->first))
    {
      n_set.insert(*it);
      it++;
      continue;
    }

    auto r_p = fold_p + fold_p - *it;
    it++;

    if (r_p < START)
      continue;

    n_set.insert(r_p);
  }
  return n_set;
}

void visualize(const Unordered_set &set)
{
  int x = 0;
  int y = 0;
  std::vector<std::vector<char>> m;
  for (const auto &v : set)
  {
    if (v.first > x)
      x = v.first;
    if (v.second > y)
      y = v.second;
  }

  for (size_t i = 0; i <= y; i++)
  {
    m.push_back(std::vector<char>());
    for (size_t j = 0; j <= x; j++)
    {
      m[i].push_back('.');
    }
  }

  for (const auto &v : set)
  {
    m[v.second][v.first] = '#';
  }

  for (const auto &arr : m)
  {
    for (const auto &ch : arr)
    {
      std::cout << ch;
    }
    std::cout << "\n";
  }
}

void solve(const std::string &fileName)
{
  Unordered_set set;
  std::vector<std::pair<int, int>> fold;
  std::ifstream in(fileName);
  std::string row;
  bool is_fold = false;
  while (!in.eof())
  {
    std::getline(in, row);
    if (row.empty())
    {
      is_fold = true;
      continue;
    }

    if (is_fold)
      split_fold(row, fold);
    else
      split(row, set);
  }

  // part 1
  set = do_fold(fold[0], set);
  std::cout << set.size() << "\n";

  // part 2
  for (size_t i = 1; i < fold.size(); i++)
  {
    set = do_fold(fold[i], set);
  }
  visualize(set);
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}