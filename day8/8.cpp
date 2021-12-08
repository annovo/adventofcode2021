#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

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

void find_sixth(const std::vector<std::string> &size_six, std::unordered_map<std::string, int> &pattern_to_num,
                std::unordered_map<int, std::string> &num_to_pattern)
{
  for (const auto &str : size_six)
  {
    auto six = num_to_pattern.find(6);
    if (six == num_to_pattern.end())
    {
      auto one = num_to_pattern.find(1)->second;
      for (const auto &c : one)
      {
        auto found = str.find(c);
        if (found == std::string::npos)
        {
          num_to_pattern.insert({6, str});
          pattern_to_num.insert({str, 6});
          break;
        }
      }
    }

    auto nine = num_to_pattern.find(9);
    if (nine == num_to_pattern.end())
    {
      auto four = num_to_pattern.find(4)->second;
      bool isNine = true;
      for (const auto &c : four)
      {
        auto found = str.find(c);
        if (found == std::string::npos)
        {
          isNine = false;
          break;
        }
      }
      if (isNine)
      {
        num_to_pattern.insert({9, str});
        pattern_to_num.insert({str, 9});
      }
    }
  }

  for (const auto &s : size_six)
  {
    if (pattern_to_num.find(s) == pattern_to_num.end())
    {
      num_to_pattern.insert({0, s});
      pattern_to_num.insert({s, 0});
      break;
    }
  }
}
std::string calculate_four_minus_one(const std::unordered_map<int, std::string> &num_to_pattern)
{
  std::string r = "";
  auto one = num_to_pattern.find(1)->second;
  auto four = num_to_pattern.find(4)->second;
  for (const auto &c : four)
  {
    if (c != one.at(0) && c != one.at(1))
      r += c;
  }
  return r;
}

void find_fifth(const std::vector<std::string> &size_five, std::unordered_map<std::string, int> &pattern_to_num,
                std::unordered_map<int, std::string> &num_to_pattern)
{
  std::string four_minus_one = calculate_four_minus_one(num_to_pattern);

  for (const auto &str : size_five)
  {
    auto three = num_to_pattern.find(3);
    if (three == num_to_pattern.end())
    {
      auto one = num_to_pattern.find(1)->second;
      bool isThree = true;
      for (const auto &c : one)
      {
        auto found = str.find(c);
        if (found == std::string::npos)
        {
          isThree = false;
          break;
        }
      }
      if (isThree)
      {
        num_to_pattern.insert({3, str});
        pattern_to_num.insert({str, 3});
      }
    }

    auto five = num_to_pattern.find(5);
    if (five == num_to_pattern.end())
    {
      bool isFive = true;
      for (const auto &c : four_minus_one)
      {
        auto found = str.find(c);
        if (found == std::string::npos)
        {
          isFive = false;
          break;
        }
      }
      if (isFive)
      {
        num_to_pattern.insert({5, str});
        pattern_to_num.insert({str, 5});
      }
    }
  }

  for (const auto &s : size_five)
  {
    if (pattern_to_num.find(s) == pattern_to_num.end())
    {
      num_to_pattern.insert({2, s});
      pattern_to_num.insert({s, 2});
      break;
    }
  }
}

std::unordered_map<std::string, int> update_map(const std::vector<std::string> &input_patterns)
{
  std::unordered_map<std::string, int> pattern_to_num;
  std::unordered_map<int, std::string> num_to_pattern;
  std::vector<std::string> size_six;
  std::vector<std::string> size_five;

  // find unique
  for (const auto &s : input_patterns)
  {
    switch (s.size())
    {
    case 2:
      pattern_to_num.insert({s, 1});
      num_to_pattern.insert({1, s});
      break;
    case 4:
      pattern_to_num.insert({s, 4});
      num_to_pattern.insert({4, s});
      break;
    case 3:
      pattern_to_num.insert({s, 7});
      num_to_pattern.insert({7, s});
      break;
    case 7:
      pattern_to_num.insert({s, 8});
      num_to_pattern.insert({8, s});
      break;
    case 5:
      size_five.push_back(s);
      break;
    case 6:
      size_six.push_back(s);
      break;
    default:
      break;
    }
  }

  // find nine & six
  find_sixth(size_six, pattern_to_num, num_to_pattern);
  find_fifth(size_five, pattern_to_num, num_to_pattern);
  return pattern_to_num;
}

int calculate_second(const std::vector<std::vector<std::string>> &input_patterns, const std::vector<std::vector<std::string>> &input_outputs)
{
  int sum = 0;
  for (size_t i = 0; i < input_patterns.size(); i++)
  {
    auto pattern_to_num = update_map(input_patterns[i]);
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