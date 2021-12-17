#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

const int id_v_bits = 3;
const int val_group_bits = 5;

struct Packet
{
  int index_start;
  int index_end;
  int id;
  int version;
  int64_t val;
  int length_type = -1;
  std::vector<Packet> p_arr;
};

const std::string hex_char_to_bin(char c)
{
  switch (toupper(c))
  {
  case '0':
    return "0000";
  case '1':
    return "0001";
  case '2':
    return "0010";
  case '3':
    return "0011";
  case '4':
    return "0100";
  case '5':
    return "0101";
  case '6':
    return "0110";
  case '7':
    return "0111";
  case '8':
    return "1000";
  case '9':
    return "1001";
  case 'A':
    return "1010";
  case 'B':
    return "1011";
  case 'C':
    return "1100";
  case 'D':
    return "1101";
  case 'E':
    return "1110";
  case 'F':
    return "1111";
  default:
    return "";
  }
}

void split(const std::string &row, std::string &input)
{
  for (char c : row)
    input += hex_char_to_bin(c);
}

int64_t calculate_binary(const std::string &input, int64_t v, const int index, const int step)
{
  for (int j = index; j < input.size() && j < index + step; j++)
  {
    v = (v << 1) + (input.at(j) - '0');
  }
  return v;
}

Packet find_packets(const std::string &input, const int index, std::vector<Packet> &packets)
{
  int n_index = index;
  Packet p;
  const int version = calculate_binary(input, 0, n_index, id_v_bits);
  n_index += id_v_bits;
  const int id = calculate_binary(input, 0, n_index, id_v_bits);
  n_index += id_v_bits;

  if (id == 4)
  {
    int64_t val = 0;
    while (input.at(n_index) == '1')
    {
      val = calculate_binary(input, val, n_index + 1, val_group_bits - 1);
      n_index += val_group_bits;
    }
    val = calculate_binary(input, val, n_index + 1, val_group_bits - 1);
    n_index += val_group_bits;
    p = {index, n_index, id, version, val};
    packets.push_back(p);
    return p;
  }
  else
  {
    std::vector<Packet> p_arr;
    int type = input.at(n_index++) - '0';
    const int bits = type == 0 ? 15 : 11;
    int length = 0;
    if (type == 1)
    {
      int pack_num = calculate_binary(input, 0, n_index, bits);
      n_index += bits;

      for (int i = 0; i < pack_num; i++)
      {
        p = find_packets(input, n_index, packets);
        n_index = p.index_end;
        p_arr.push_back(p);
      }
    }
    else
    {
      length = calculate_binary(input, 0, n_index, bits);
      n_index += bits;
      length += n_index;
      while (n_index < length)
      {
        p = find_packets(input, n_index, packets);
        n_index = p.index_end;
        p_arr.push_back(p);
      }
    }
    p = {index,
         n_index,
         id,
         version,
         0,
         type,
         p_arr};
    packets.push_back(p);
  }
  return p;
}

int64_t find_sum(const Packet &packet)
{
  int64_t r = 0;
  bool is_true = false;

  switch (packet.id)
  {
  case 0:
    for (const auto &p : packet.p_arr)
    {
      r += find_sum(p);
    }
    break;
  case 1:
    r = 1;
    for (const auto &p : packet.p_arr)
    {
      r *= find_sum(p);
    }
    break;
  case 2:
    r = INT64_MAX;
    for (const auto &p : packet.p_arr)
    {
      r = std::min(find_sum(p), r);
    }
    break;
  case 3:
    r = -1;
    for (const auto &p : packet.p_arr)
    {
      r = std::max(find_sum(p), r);
    }
    break;
  case 4:
    r = packet.val;
    break;
  case 5:
    r = -1;
    for (const auto &p : packet.p_arr)
    {
      int64_t t = find_sum(p);
      if (r == -1)
      {
        r = t;
        continue;
      }
      is_true = t < r ? true : false;
    }
    r = is_true ? 1 : 0;
    break;
  case 6:
    r = -1;
    for (const auto &p : packet.p_arr)
    {
      int64_t t = find_sum(p);
      if (r == -1)
      {
        r = t;
        continue;
      }
      is_true = t > r ? true : false;
    }
    r = is_true ? 1 : 0;
    break;
  case 7:
    r = -1;
    for (const auto &p : packet.p_arr)
    {
      int64_t t = find_sum(p);
      if (r == -1)
      {
        r = t;
        continue;
      }
      is_true = t == r ? true : false;
    }
    r = is_true ? 1 : 0;
    break;
  default:
    return 0;
  }
  return r;
}

void parse_packets(const std::string &input)
{
  std::vector<Packet> packets;
  int versions = 0;

  Packet p = find_packets(input, 0, packets);

  // part 1
  for (const auto &p : packets)
  {
    versions += p.version;
  }
  std::cout << versions << "\n";

  // part 2
  std::cout << find_sum(p) << "\n";
}

void solve(const std::string &fileName)
{
  std::ifstream in(fileName);
  std::string row;
  std::string input;
  while (!in.eof())
  {
    std::getline(in, row);
    split(row, input);
  }
  parse_packets(input);
}

int main(int argc, char *argv[])
{
  solve(argv[1]);
}