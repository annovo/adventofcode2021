#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

static std::map<std::set<char>, int> m = {{{'c', 'f'}, 1}, {{'a', 'b', 'c', 'e', 'f', 'g'}, 0}, {{'a', 'c', 'd', 'e', 'g'}, 2}, {{'a', 'c', 'd', 'f', 'g'}, 3}, {{'b', 'c', 'd', 'f'}, 4}, {{'a', 'b', 'd', 'f', 'g'}, 5}, {{'a', 'b', 'd', 'e', 'f', 'g'}, 6}, {{'a', 'c', 'f'}, 7}, {{'a', 'b', 'c', 'd', 'e', 'f', 'g'}, 8}, {{'a', 'b', 'c', 'd', 'f', 'g'}, 9}};

void split(const std::string &row, std::vector<std::vector<std::string>> &array_patterns, std::vector<std::vector<std::string>> &array_outputs)
{
  std::string symbol;
  std::vector<std::string> current_array_pattern;
  std::vector<std::string> current_array_output;
  bool next = false;
  for (const char c : row)
  {
    if (c == ' ')
    {
      if (!symbol.empty())
      {
        std::sort(symbol.begin(), symbol.end());
        if (next)
          current_array_output.push_back(symbol);
        else
          current_array_pattern.push_back(symbol);
      }
      symbol = "";
    }
    else if (c == '|')
      next = true;
    else
      symbol.push_back(c);
  }
  if (!symbol.empty())
  {
    std::sort(symbol.begin(), symbol.end());
    current_array_output.push_back(symbol);
  }

  array_patterns.push_back(current_array_pattern);
  array_outputs.push_back(current_array_output);
}

int calculate_first(const std::vector<std::vector<std::string>> &input_outputs)
{
  int sum = 0;
  for (const auto &output : input_outputs)
  {
    for (const auto &s : output)
    {
      int size = s.size();
      if (size == 2 || size == 4 || size == 3 || size == 7)
        sum++;
    }
  }
  return sum;
}

std::set<char> to_set(const std::string &str, std::unordered_map<char, char> &wire)
{
  std::set<char> s;
  for (const auto &c : str)
  {
    s.insert(wire.find(c)->second);
  }
  return s;
}

bool map_pattern(const std::vector<std::string> &input_patterns, std::unordered_map<char, char> &wire, const std::unordered_set<char> &vars, int str_ind, int char_ind)
{
  if (char_ind >= input_patterns[str_ind].size())
  {
    if (m.find(to_set(input_patterns[str_ind], wire)) != m.end())
    {
      str_ind++;
      char_ind = 0;
    }
    else
      return false;
  }

  if (str_ind >= input_patterns.size())
    return true;

  auto ch = input_patterns[str_ind].at(char_ind);
  if (wire.find(ch) != wire.end())
  {
    return map_pattern(input_patterns, wire, vars, str_ind, char_ind + 1);
  }

  auto n_vars = vars;
  for (const auto &k : vars)
  {
    wire.insert({ch, k});
    n_vars.erase(k);
    if (!map_pattern(input_patterns, wire, n_vars, str_ind, char_ind + 1))
      wire.erase(ch);
    else
      return true;
    n_vars.insert(k);
  }
  return false;
}

bool compare(std::string &s1, std::string &s2)
{
  return s1.size() < s2.size();
}

std::unordered_map<std::string, int> update_map(const std::vector<std::string> &input_patterns, std::unordered_map<char, char> &wire)
{
  std::unordered_map<std::string, int> r_m;
  const std::unordered_set<char> vars({'a', 'b', 'c', 'd', 'e', 'f', 'g'});

  auto array = input_patterns;
  std::sort(array.begin(), array.end(), compare);

  map_pattern(array, wire, vars, 0, 0);

  for (const auto &s : input_patterns)
  {
    r_m.insert({s, m.find(to_set(s, wire))->second});
  }
  return r_m;
}

int calculate_second(const std::vector<std::vector<std::string>> &input_patterns, const std::vector<std::vector<std::string>> &input_outputs)
{
  int sum = 0;
  for (size_t i = 0; i < input_patterns.size(); i++)
  {
    std::unordered_map<char, char> wire;
    auto pattern_to_num = update_map(input_patterns[i], wire);
    int t_sum = 0;
    for (const auto &s : input_outputs[i])
    {
      auto it = pattern_to_num.find(s);
      if (it != pattern_to_num.end())
      {
        t_sum = t_sum * 10 + it->second;
      }
    }
    sum += t_sum;
  }
  return sum;
}

void solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<std::vector<std::string>> input_patterns;
  std::vector<std::vector<std::string>> input_outputs;
  while (!in.eof())
  {
    std::getline(in, row);
    split(row, input_patterns, input_outputs);
  }
  std::cout << calculate_first(input_outputs) << "-" << calculate_second(input_patterns, input_outputs) << "\n";
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}