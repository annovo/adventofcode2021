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
      symbol.push_back(c);

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

void update_map(std::map<std::pair<int, int>, int> &m, int first_x, int last_x,
                int first_y, int last_y)
{
  int sign_x = first_x > last_x ? -1 : 1;
  int sign_y = first_y > last_y ? -1 : 1;
  while (true)
  {
    auto pair = std::make_pair(first_x, first_y);
    auto it = m.find(pair);

    if (it == m.end())
      m.insert({pair, 1});
    else
      it->second = it->second + 1;

    if (first_y == last_y && first_x == last_x)
      break;

    if (first_x != last_x)
      first_x += sign_x;

    if (first_y != last_y)
      first_y += sign_y;
  }
}

int find_overlap(const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &input, bool first)
{
  std::map<std::pair<int, int>, int> m;
  int sum = 0;
  for (size_t i = 0; i < input.size(); i++)
  {
    auto two_pairs = input[i];
    auto p0 = two_pairs.first;
    auto p1 = two_pairs.second;
    if (first && (p0.first != p1.first && p0.second != p1.second))
      continue;
    update_map(m, p0.first, p1.first, p0.second, p1.second);
  }
  for (auto const &it : m)
  {
    if (it.second > 1)
      sum++;
  }
  return sum;
}

std::pair<int, int> solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> input;
  while (!in.eof())
  {
    std::getline(in, row);
    auto p = split(row);
    input.push_back(p);
  }
  return std::make_pair(find_overlap(input, true), find_overlap(input, false));
}

int main(int argc, char *argv[])
{
  std::pair<int, int> ans = solve(argv[1]);
  std::cout << ans.first << "-" << ans.second << "\n";
}