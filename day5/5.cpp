#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

std::pair<std::pair<int, int>, std::pair<int, int>> split(const std::string &row)
{
  std::pair<std::pair<int, int>, std::pair<int, int>> fields({0, 0}, {0, 0});
  int i = 0;
  std::string symbol;
  for (char c : row)
  {

    if (isdigit(c))
    {
      symbol.push_back(c);
    }

    if (c == ',' || c == '-')
    {
      int v = std::stoi(symbol);
      symbol = "";
      switch (i)
      {
      case 0:
        fields.first.first = v;
        break;
      case 1:
        fields.first.second = v;
        break;
      case 2:
        fields.second.first = v;
        break;
      default:
        break;
      }
      i++;
    }
  }

  fields.second.second = std::stoi(symbol);
  return fields;
}

void update_map(std::map<std::pair<int, int>, int> &m, int first_x, int last_x, int sign_x,
                int first_y, int last_y, int sign_y)
{
  while (true)
  {
    auto pair = std::make_pair(first_x, first_y);
    auto it = m.find(pair);
    if (it == m.end())
    {

      m.insert({pair, 1});
    }
    else
    {
      it->second = it->second + 1;
    }

    if (first_y == last_y && first_x == last_x)
      break;
    if (first_x != last_x)
    {
      first_x += sign_x;
    }
    if (first_y != last_y)
    {
      first_y += sign_y;
    }
  }
}

std::pair<int, int> findOverlap(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::map<std::pair<int, int>, int> m;
  std::map<std::pair<int, int>, int> m_all;
  int sum = 0;
  int sum_all = 0;
  while (!in.eof())
  {
    std::getline(in, row);
    auto p = split(row);

    //part 1
    if (p.first.first == p.second.first || p.first.second == p.second.second)
    {
      int sign = (p.first.second > p.second.second) || (p.first.first > p.second.first) ? -1 : 1;
      update_map(m, p.first.first, p.second.first, sign, p.first.second, p.second.second, sign);
    }

    //part 2
    int sign_x = p.first.first > p.second.first ? -1 : 1;
    int sign_y = p.first.second > p.second.second ? -1 : 1;
    update_map(m_all, p.first.first, p.second.first, sign_x, p.first.second, p.second.second, sign_y);
  }

  for (auto const &it : m)
  {
    if (it.second > 1)
      sum++;
  }
  for (auto const &it : m_all)
  {
    if (it.second > 1)
      sum_all++;
  }
  return std::make_pair(sum, sum_all);
}

int main(int argc, char *argv[])
{
  std::pair<int, int> ans = findOverlap(argv[1]);
  std::cout << ans.first << "-" << ans.second << "\n";
}