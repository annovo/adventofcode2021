#include <string>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<int> split(const std::string &row)
{
  std::vector<int> fields{};
  for (char c : row)
  {
    fields.push_back(c - '0');
  }
  return fields;
}

//part 1
int findConsumption(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<int> nums;
  int sum = 0;
  while (!in.eof())
  {
    std::getline(in, row);
    if (in.bad() || in.fail() || row.empty())
      break;

    const std::vector<int> splitted = split(row);
    if (nums.size() == 0)
    {
      nums = splitted;
      continue;
    }

    for (int i = 0; i < nums.size(); i++)
    {
      nums[i] += splitted[i];
    }
    sum++;
  }

  int gamma = 0;
  int epsilon = 0;
  int len = nums.size() - 1;
  for (int i = len; i >= 0; i--)
  {
    if (sum - nums[i] < nums[i])
    {
      gamma |= (1 << (len - i));
    }
    else
    {
      epsilon |= (1 << (len - i));
    }
  }
  return gamma * epsilon;
}

//part 2
int calculateBinary(const std::vector<int> &v)
{
  int r = 0;
  int len = v.size() - 1;
  for (int i = 0; i <= len; i++)
  {
    if (v[i] == 1)
      r |= (1 << (len - i));
  }
  return r;
}

int filter(const std::vector<std::vector<int>> &nums, int p, bool reverse)
{
  if (nums.size() == 1 || p >= nums[0].size())
    return calculateBinary(nums[0]);

  int counter = 1;
  int curr = nums[0][p];
  std::vector<std::vector<int>> temp;
  for (size_t j = 1; j < nums.size(); j++)
  {
    if (curr == nums[j][p])
      counter++;
    else
      counter--;

    if (counter < 0)
    {
      curr = nums[j][p];
      counter = 1;
    }
  }

  if (counter == 0)
  {
    curr = reverse ? 0 : 1;
  }
  else
  {
    curr = reverse ? 1 - curr : curr;
  }

  for (size_t i = 0; i < nums.size(); i++)
  {
    if (nums[i][p] == curr)
      temp.push_back(nums[i]);
  }

  return filter(temp, p + 1, reverse);
}

int findOxyAndCO(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<std::vector<int>> nums;

  while (!in.eof())
  {
    std::getline(in, row);
    if (in.bad() || in.fail() || row.empty())
      break;

    nums.push_back(split(row));
  }

  return filter(nums, 0, true) * filter(nums, 0, false);
}

int main(int argc, char *argv[])
{
  std::cout << findConsumption(argv[1]) << "\n"
            << findOxyAndCO(argv[1]) << "\n";
}