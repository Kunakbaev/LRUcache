#pragma once

#include <iostream>
#include <string>

const size_t MAX_PAGE_INDEX = 1024;

// class page_hasher_t;

class page_t {
 private:
  std::size_t size_  = 0;         // page size
  std::string data_  = "";   // page data

 public:
  std::size_t index = 0;         // page index: 1, 2, ... n

  page_t() = default;
  page_t(size_t size, const std::string& data, std::size_t index2)
    : size_(size), data_(data), index(index2) {}

//  public:
//   void set_page_index(size_t new_index) {
//     index = new_index;
//   }

  // friend class page_hasher_t;
};

// class page_hasher_t {
//  private:
//   static const uint64_t STR_HASH_BASE = 253;
//   static const uint64_t STR_HASH_MOD  = 1e9 + 7;

//  public:
//   uint64_t get_str_hash(const std::string& str) const {
//     uint64_t hash = 0;
//     for (auto ch : str) {
//       hash *= STR_HASH_BASE;
//       hash %= STR_HASH_MOD;
//       hash += static_cast<uint64_t>(ch + 1);
//       hash %= STR_HASH_MOD;
//     }

//     return hash;
//   }
//   uint64_t operator()(const page_t& page) const {
//     uint64_t str_hash = get_str_hash(page.data);
//     uint64_t hash = (page.index << 32) ^ (page.size) ^ 0xfde122a21910aeULL;
//     hash ^= str_hash;

//     return hash;
//   }
// };

void slow_get_page(std::size_t index, page_t& page);
