#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

void split(const std::string &row, std::unordered_map<std::string, char> &map)
{
  std::string symbol;
  char ch;
  bool is_ch = false;
  for (char c : row)
  {
    if (c == ' ' || c == '-')
      continue;

    if (c == '>')
    {
      is_ch = true;
      continue;
    }

    if (is_ch)
    {
      ch = c;
      break;
    }
    symbol.push_back(c);
  }
  map.insert({symbol, ch});
}

template <typename T>
void update_count(std::unordered_map<T, long long> &count, const T &c, long long cnt)
{
  auto it_count = count.find(c);
  if (it_count == count.end())
    count.insert({c, cnt});
  else
    it_count->second += cnt;
}

void find_poly(const std::unordered_map<std::string, char> &map,
               const std::unordered_map<std::string, long long> &i_polymer,
               const std::unordered_map<char, long long> &i_count, int steps)
{
  long long max_ch = INT64_MIN;
  long long min_ch = INT64_MAX;
  std::unordered_map<std::string, long long> polymer(i_polymer);
  std::unordered_map<char, long long> count(i_count);

  while (steps != 0)
  {
    steps--;
    std::unordered_map<std::string, long long> n_polymer;
    for (const auto &kv : polymer)
    {
      auto it = map.find(kv.first);
      if (it != map.end())
      {
        auto cnt = kv.second;

        update_count(count, it->second, cnt);
        update_count(n_polymer, std::string() + kv.first.at(0) + it->second, cnt);
        update_count(n_polymer, std::string() + it->second + kv.first.at(1), cnt);
      }
    }
    polymer = n_polymer;
  }

  for (const auto &kp : count)
  {
    if (kp.second > max_ch)
      max_ch = kp.second;
    if (kp.second < min_ch)
      min_ch = kp.second;
  }

  std::cout << (max_ch - min_ch) << "\n";
}

void make_polymer(const std::string &row, std::unordered_map<std::string, long long> &polymer, std::unordered_map<char, long long> &count)
{
  update_count(count, *row.begin(), 1);

  for (auto it = row.begin(); it != row.end();)
  {
    std::string str;
    str.push_back(*it);
    ++it;

    if (it == row.end())
      break;

    update_count(count, *it, 1);
    str.push_back(*it);

    auto p_it = polymer.find(str);
    if (p_it != polymer.end())
      p_it->second++;
    else
      polymer.insert({str, 1});
  }
}

void solve(const std::string &fileName)
{
  std::unordered_map<std::string, char> map;
  std::unordered_map<std::string, long long> polymer;
  std::unordered_map<char, long long> count;

  std::ifstream in(fileName);
  std::string row;
  bool is_map = false;
  while (!in.eof())
  {
    std::getline(in, row);
    if (row.empty())
    {
      is_map = true;
      continue;
    }
    if (is_map)
      split(row, map);
    else
      make_polymer(row, polymer, count);
  }

  // part 1
  find_poly(map, polymer, count, 10);
  // part 2
  find_poly(map, polymer, count, 40);
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}