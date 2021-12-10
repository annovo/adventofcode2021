#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

static const std::unordered_map<char, int> score_map({{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}});
static const std::unordered_map<char, int> second_score_map({{'(', 1}, {'[', 2}, {'{', 3}, {'<', 4}});
static const std::unordered_map<char, char> braces({{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}});

void split(const std::string &row, std::vector<char> &array)
{
  for (char c : row)
    array.push_back(c);
}

char missing_character(const std::vector<char> &input, std::stack<char> &stack)
{
  for (const char &c : input)
  {
    if (score_map.find(c) == score_map.end())
      stack.push(c);
    else
    {
      const char p_c = stack.top();
      if (p_c != braces.find(c)->second)
        return c;
      stack.pop();
    }
  }
  return ' ';
}

long complete_line(std::stack<char> &stack)
{
  long sum = 0;
  while (!stack.empty())
  {
    sum = sum * 5 + second_score_map.find(stack.top())->second;
    stack.pop();
  }
  return sum;
}

void find_score(const std::vector<std::vector<char>> &input)
{
  const size_t m = input.size();
  std::vector<long> scores;
  long sum = 0;
  for (size_t i = 0; i < m; i++)
  {
    std::stack<char> stack;
    const auto m_it = score_map.find(missing_character(input[i], stack));

    if (m_it != score_map.end())
      sum += m_it->second; // part 1
    else
      scores.push_back(complete_line(stack)); // part 2
  }

  std::sort(scores.begin(), scores.end());
  std::cout << sum << "-" << scores[scores.size() / 2] << "\n";
}

void solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<std::vector<char>> input;
  while (!in.eof())
  {
    std::vector<char> v;
    std::getline(in, row);
    split(row, v);
    input.push_back(v);
  }
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}