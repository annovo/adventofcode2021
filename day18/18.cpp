#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>

struct CustomPair
{
  int val;

  CustomPair *first;
  CustomPair *second;
  CustomPair *left;
  CustomPair *right;

  CustomPair *left_most;
  CustomPair *right_most;

  CustomPair() : val(0), first(0), second(0), left(0), right(0), left_most(0), right_most(0) {}
  //   ~CustomPair()
  //   {
  //     if (first)
  //       delete first;
  //     if (second)
  //       delete second;
  //     if (left)
  //       delete left;
  //     if (right)
  //       delete right;

  //     if (left_most)
  //       delete left_most;
  //     if (right_most)
  //       delete right_most;
  //   }
};

int split(const std::string &row, CustomPair &p, int index, std::vector<CustomPair *> &neighbors)
{
  if (index >= row.size())
    return index;

  bool is_first = true;
  while (index < row.size() && row.at(index) != ']')
  {
    char ch = row.at(index);
    if (std::isdigit(ch))
    {
      CustomPair *pair = new CustomPair();
      pair->val = ch - '0';
      neighbors.push_back(pair);
      if (is_first)
        p.first = pair;
      else
        p.second = pair;
      is_first = false;
    }
    else if (ch == '[')
    {

      CustomPair *n_p = new CustomPair();
      index = split(row, *n_p, index + 1, neighbors);
      if (is_first)
        p.first = n_p;
      else
        p.second = n_p;
      is_first = false;
    }
    index++;
  }
  return index;
}

void get_val(const CustomPair &p)
{
  if (p.first == nullptr && p.second == nullptr)
  {
    std::cout << p.val;
    return;
  }
  std::cout << "[";
  get_val(*p.first);
  std::cout << ",";
  get_val(*p.second);
  std::cout << "]";
}

bool explode(CustomPair *pair, const int lvl)
{
  if (pair->first == nullptr && pair->second == nullptr)
  {
    return true;
  }

  if (lvl > 4)
  {
    pair->val = 0;
    if (pair->first->left != nullptr)
    {
      pair->first->left->val += pair->first->val;
      pair->first->left->right = pair;
      pair->left = pair->first->left;
    }

    if (pair->second->right != nullptr)
    {
      pair->second->right->val += pair->second->val;
      pair->second->right->left = pair;
      pair->right = pair->second->right;
    }

    pair->first = nullptr;
    pair->second = nullptr;
    return false;
  }

  return explode(pair->first, lvl + 1) && explode(pair->second, lvl + 1);
}

bool less_than(CustomPair &pair)
{
  if (pair.first == nullptr && pair.second == nullptr)
  {
    if (pair.val < 10)
      return true;

    CustomPair *first = new CustomPair();
    first->val = pair.val / 2;

    CustomPair *second = new CustomPair();
    second->val = pair.val - first->val;

    if (pair.right != nullptr)
    {
      CustomPair *right = pair.right;
      right->left = second;
      second->right = right;
    }

    if (pair.left != nullptr)
    {
      CustomPair *left = pair.left;
      left->right = first;
      first->left = left;
    }

    first->right = second;
    second->left = first;

    pair.first = first;
    pair.second = second;

    return false;
  }

  return less_than(*pair.first) && less_than(*pair.second);
}

void check_pair(CustomPair *pair)
{
  bool fine = false;
  while (!fine)
  {
    fine = explode(pair, 1);
    if (fine)
      fine = less_than(*pair);
  }
}

CustomPair *find_left(CustomPair *p)
{
  if (p->first == nullptr && p->second == nullptr)
    return p;
  return find_left(p->first);
}

CustomPair *find_right(CustomPair *p)
{
  if (p->first == nullptr && p->second == nullptr)
    return p;
  return find_right(p->second);
}

CustomPair *copy_pair(CustomPair *f, std::vector<CustomPair *> &neighbours)
{
  if (f == nullptr)
    return nullptr;
  if (f->first == nullptr && f->second == nullptr)
  {
    CustomPair *ptr_n = new CustomPair();
    ptr_n->val = f->val;
    neighbours.push_back(ptr_n);
    return ptr_n;
  }
  CustomPair *ptr = new CustomPair();
  ptr->first = copy_pair(f->first, neighbours);
  ptr->second = copy_pair(f->second, neighbours);
  return ptr;
}

void add_neighbors(const std::vector<CustomPair *> &neighbours)
{
  for (int i = 0; i < neighbours.size(); i++)
  {
    if (i > 0)
      neighbours[i]->left = neighbours[i - 1];
    if (i < neighbours.size() - 1)
      neighbours[i]->right = neighbours[i + 1];
  }
}

CustomPair *add_one_pair(CustomPair *f, CustomPair *s)
{
  CustomPair *ptr = new CustomPair();
  std::vector<CustomPair *> neighbours;
  CustomPair *ptr_f = copy_pair(f, neighbours);
  add_neighbors(neighbours);
  ptr_f->left_most = neighbours[0];
  ptr_f->right_most = neighbours[neighbours.size() - 1];

  neighbours = {};
  CustomPair *ptr_s = copy_pair(s, neighbours);
  add_neighbors(neighbours);
  ptr_s->left_most = neighbours[0];
  ptr_s->right_most = neighbours[neighbours.size() - 1];

  ptr->first = ptr_f;
  ptr->second = ptr_s;

  ptr->left_most = ptr_f->left_most;
  ptr->right_most = ptr_s->right_most;

  ptr_f->right_most->right = ptr_s->left_most;
  ptr_s->left_most->left = ptr_f->right_most;

  check_pair(ptr);

  ptr->left_most = find_left(ptr);
  ptr->right_most = find_right(ptr);
  return ptr;
}

CustomPair *add_pairs(const std::vector<CustomPair *> &input)
{
  CustomPair *first_p = input[0];
  for (size_t i = 1; i < input.size(); i++)
  {
    first_p = add_one_pair(first_p, input[i]);
  }
  return first_p;
}

int64_t calculate(CustomPair *pair)
{
  if (pair->first == nullptr && pair->second == nullptr)
    return pair->val;

  return 3 * calculate(pair->first) + 2 * calculate(pair->second);
}

int64_t find_max(const std::vector<CustomPair *> &input)
{
  int64_t res = 0;
  for (size_t i = 0; i < input.size(); i++)
  {
    CustomPair *first_p = input[i];
    for (size_t j = i + 1; j < input.size(); j++)
    {
      CustomPair *second_p = input[j];

      int64_t t = calculate(add_one_pair(second_p, first_p));
      int64_t t2 = calculate(add_one_pair(first_p, second_p));

      res = std::max(res, t);
      res = std::max(res, t2);
    }
  }
  return res;
}

void solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::vector<CustomPair *> input;
  while (!in.eof())
  {
    std::getline(in, row);
    CustomPair *p = new CustomPair();

    std::vector<CustomPair *> neighbours;
    split(row, *p, 1, neighbours);
    add_neighbors(neighbours);

    p->left_most = neighbours[0];
    p->right_most = neighbours[neighbours.size() - 1];
    input.push_back(p);
  }
  // part 1
  CustomPair *res = add_pairs(input);
  std::cout << calculate(res) << "\n";

  // part 2
  std::cout << find_max(input);
  for (auto p : input)
    delete p;
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}