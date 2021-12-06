#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

static const int DAYS = 80;
static const int MORE_DAYS = 256;
static const size_t SIZE = 9;

void split(const std::string &row, long *array)
{
  for (char c : row)
  {
    if (isdigit(c))
      array[c - '0']++;
  }
}

long find_fish(long *input, int days)
{
  long array[SIZE];
  std::copy(input, input + SIZE, array);
  long sum = 0;
  int start = 0;
  while (start < days)
  {
    long new_fish = array[0];
    long prev = array[0];

    for (int i = SIZE - 1; i >= 0; i--)
    {
      long temp = array[i];
      array[i] = prev;
      prev = temp;
    }

    array[SIZE - 3] += new_fish;
    start++;
  }

  for (size_t i = 0; i < SIZE; i++)
    sum += array[i];

  return sum;
}

std::pair<long, long> solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  long input[SIZE] = {0};
  while (!in.eof())
  {
    std::getline(in, row);
    split(row, input);
  }
  return std::make_pair(find_fish(input, DAYS), find_fish(input, MORE_DAYS));
}

int main(int argc, char *argv[])
{
  std::pair<long, long> ans = solve(argv[1]);
  std::cout << ans.first << "-" << ans.second << "\n";
}