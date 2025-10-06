#pragma once

#include <map>
#include <unordered_set>
#include <stack>
#include <vector>
#include <list>
#include <unordered_map>

#include "listWithTrack.hpp"
#include "webPageStruct.hpp"
#include "logLib.hpp"

// original algorithm paper: https://www.vldb.org/conf/1994/P439.PDF
template <typename T, typename KeyT = int>
class lru2q_cache_t {
 private:
  struct just_page_index_t {
    std::size_t index;
    just_page_index_t(std::size_t ind) : index(ind) {}
  };

  // = new elems list size / max_cache_sz
  static constexpr double NEW_ELEMS_CAPACITY_RATIOS   = 0.8;
  static constexpr double GHOST_ELEMS_CAPACITY_RATIOS = 0.5;
  const size_t max_cache_sz_ = 0;

  list_with_track_t<T,                 KeyT> new_elems_list_   = {};
  list_with_track_t<T,                 KeyT> hot_elems_list_   = {};
  list_with_track_t<just_page_index_t, KeyT> ghost_elems_list_ = {};

 private:
  [[nodiscard]] std::size_t get_new_elems_list_cap() const {
    return static_cast<size_t>(NEW_ELEMS_CAPACITY_RATIOS * 
           static_cast<double>(max_cache_sz_));
  }

  [[nodiscard]] std::size_t get_ghost_elems_list_cap() const {
    // size of ghost buffer is how many keys would fit in 50% of cache capacity
    return static_cast<size_t>(GHOST_ELEMS_CAPACITY_RATIOS *
           static_cast<double>(max_cache_sz_)) * sizeof(T) / sizeof(KeyT);
  }

 public:
  lru2q_cache_t(size_t max_cache_sz)
      : max_cache_sz_(max_cache_sz),
      new_elems_list_(get_new_elems_list_cap()),
      hot_elems_list_(max_cache_sz_ - get_new_elems_list_cap()),
      ghost_elems_list_(get_ghost_elems_list_cap()) {}

  template <typename F>
  bool lookup_update(T& element, KeyT key, F slow_get_page) {
    if (hot_elems_list_.is_element_present(key)) {
      hot_elems_list_.move_elem_to_the_head(key);
      element = hot_elems_list_.get_link2element(key);
      return true;
    }

    if (new_elems_list_.is_element_present(key)) {
      element = new_elems_list_.get_link2element(key);
      return true;
    }

    slow_get_page(key, element);
    if (ghost_elems_list_.is_element_present(key)) {
      hot_elems_list_.add2head(element, key);
      ghost_elems_list_.erase_element(key);
      return false;
    } else {
      new_elems_list_.add2head(element, key);
      return false;
    }
  }

#ifdef _DEBUG
  void dump_cache() const {
    std::cout << "-----------------------\n2qcache: " << std::endl;
    std::cout << "hot elems: ";
    hot_elems_list_.print_inner_list_contents();
    std::cout << std::endl;
    std::cout << "new elems: ";
    new_elems_list_.print_inner_list_contents();
    std::cout << std::endl;
    std::cout << "ghost keys: ";
    ghost_elems_list_.print_inner_list_contents();
    std::cout << std::endl;
  }
#endif
};
