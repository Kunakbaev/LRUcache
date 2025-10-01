#pragma once

#include <list>
#include <unordered_map>

#include "logLib.hpp"
#include "webPageStruct.hpp"
#include "listWithTrack.hpp"

template <typename T, typename KeyT = int>
class cache_t {
 private:
  const std::size_t max_cache_sz_ = 0;
  list_with_track_t<T, KeyT> list_;
  // std::list<T> cache_ = {};

  // using ListIt = typename std::list<T>::iterator;
  // std::unordered_map<KeyT, ListIt> hash_ = {};

 public:
  cache_t(std::size_t max_cache_sz) : list_(max_cache_sz) {}

  // returns true on cache hit and false on cache miss
  template <typename F>
  bool lookup_update(T& element, KeyT key, F slow_get_page) {
    if (!list_.is_element_present(key)) { // not found
      // LOG_DEBUG("page not found");
      if (list_.is_full()) {
        // LOG_DEBUG("cache is full...");
        list_.remove_tail();
      }

      // fallback to slow method
      slow_get_page(key, element);
      list_.add2head(element, key);
      return false;
    }

    list_.move_elem_to_the_head(key);

    return true;
  }

#ifdef _DEBUG
  void dump_cache() const {
    LOG_DEBUG("Cache dump: ");
    list_.print_inner_list_contents();
    std::cout << std::endl;
  }
#endif
};
