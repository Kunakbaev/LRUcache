#pragma once

#include <map>
#include <unordered_set>
#include <stack>
#include <vector>
#include <list>
#include <unordered_map>

#include "../common/listWithTrack.hpp"
#include "../webPage/include/webPageStruct.hpp"
#include "../3dPartyModules/LoggerLib/include/logLib.hpp"

// original algorithm paper: https://www.vldb.org/conf/1994/P439.PDF
template <typename T, typename KeyT = int>
class lru2q_cache_t {
 private:
  // = new_pages_capacity / hot_pages_capacity
  static constexpr double NEW_ELEMS_CAPACITY_RATIOS   = 0.2;
  static constexpr double GHOST_ELEMS_CAPACITY_RATIOS = 0.5;
  const size_t max_cache_sz_ = 0;
  // const size_t new_elems_cap_;
  // const size_t hot_elems_cap_;
  // const size_t ghost_elems_cap_;

  list_with_track_t<T, KeyT> hot_elems_list_   = {};
  list_with_track_t<T, KeyT> new_elems_list_   = {};
  list_with_track_t<T, KeyT> ghost_elems_list_ = {};

  // enum class cache_hit_type_t {
  //   HIT  = true,
  //   MISS = false,
  // };

 public:

  lru2q_cache_t(size_t max_cache_sz)
      : max_cache_sz_(max_cache_sz),
      new_elems_list_(static_cast<size_t>(NEW_ELEMS_CAPACITY_RATIOS * 
                                         static_cast<double>(max_cache_sz))),
      hot_elems_list_(static_cast<size_t>((1 - NEW_ELEMS_CAPACITY_RATIOS) * 
                                         static_cast<double>(max_cache_sz))),
      // size of ghost buffer is how many keys would fit in 50% of cache capacity
      ghost_elems_list_(static_cast<size_t>(GHOST_ELEMS_CAPACITY_RATIOS *
                                            static_cast<double>(max_cache_sz))
                       * sizeof(T) / sizeof(KeyT)) {}

  // bool isCacheHit(T& element, KeyT key) {
  //   // std::cout << "key : " << key << " hot elems pos : ";
  //   // for (auto [ind, _] : hot_elems_pos_) {
  //   //   std::cout << ind << " ";
  //   // }
  //   // std::cout << std::endl;
  //   // std::cout << "new elems pos : ";
  //   // for (auto [ind, _] : new_elems_pos_) {
  //   //   std::cout << ind << " ";
  //   // }
  //   // std::cout << std::endl;

  //   auto hit = hot_elems_pos_.find(key);
  //   if (hit != hot_elems_pos_.end()) {
  //     auto elem_it = hit->second;
  //     element = *elem_it;

  //     // transfer found element to the beginning
  //     // by shifting other elements to the right
  //     if (elem_it != hot_elems_list_.begin()) {
  //       hot_elems_list_.splice(hot_elems_list_.begin(), hot_elems_list_,
  //         elem_it, std::next(elem_it));
  //     }

  //     return true;
  //   }

  //   auto hit2 = new_elems_pos_.find(key);
  //   if (hit2 != new_elems_pos_.end()) {
  //     T promoted_elem = *hit2->second;
  //     element = promoted_elem;

  //     // // Remove from new list
  //     // new_elems_list_.erase(hit2->second);
  //     // new_elems_pos_.erase(key);

  //     // add_elem2_hot_elems(key, promoted_elem);

  //     return true;
  //   }

  //   return false;
  // }

  // void add_elem2_hot_elems(KeyT key, const T& element) {
  //   // make space in hot elems if needed
  //   if (is_hot_elems_full()) {
  //     KeyT evicted = hot_elems_list_.back().index;
  //     LOG_DEBUG_VARS(evicted);
  //     hot_elems_list_.pop_back();
  //     hot_elems_pos_.erase(evicted);
  //   }

  //   hot_elems_list_.push_front(element);
  //   hot_elems_pos_[key] = hot_elems_list_.begin();
  // }

  // // returns true on cache hit and false on cache miss
  // template <typename F>
  // bool lookup_update(T& element, KeyT key, F slow_get_page) {
  //   if (isCacheHit(element, key)) {
  //     // LOG_WARNING("HIT");
  //     return true;
  //   }

  //   // LOG_WARNING("MISS");

  //   assert(new_elems_list_.size() == new_elems_pos_.size());
  //   if (is_new_elems_full() && !new_elems_list_.empty()) {
  //     // TODO: is it okay that I rely on structure of 
  //     // elements that I store?
  //     // std::cout << "new elems list size : " << new_elems_list_.size() << std::endl;
  //     KeyT evicted_elem_key = new_elems_list_.front().index;
  //     // std::cout << "evicted : " << evicted_elem_key << std::endl;
  //     new_elems_list_.pop_front();
  //     ghost_keys_buff_.insert(evicted_elem_key);
  //     new_elems_pos_.erase(evicted_elem_key);
  //     // std::cout << "1after new elems pos : ";
  //     // for (auto [ind, _] : new_elems_pos_) {
  //     //   std::cout << ind << " ";
  //     // }
  //     // std::cout << std::endl;
  //   }

  //   slow_get_page(key, element);
  //   auto ghost = ghost_keys_buff_.find(key);
  //   if (ghost != ghost_keys_buff_.end()) {
  //     ghost_keys_buff_.erase(ghost);
  //     add_elem2_hot_elems(key, element);
  //     return false;
  //   } else {
  //     new_elems_list_.push_back(element);
  //     new_elems_pos_[key] = --new_elems_list_.end();
  //     // std::cout << "2after new elems pos : ";
  //     // for (auto [ind, _] : new_elems_pos_) {
  //     //   std::cout << ind << " ";
  //     // }
  //     // std::cout << std::endl;
  //     return false;
  //   }
  // }

  template <typename F>
  void reclaim_for_x(T& element, KeyT key, F slow_get_page) {
    slow_get_page(key, element);

    // // if there's already free slot for new element
    // if (!hot_elems_list_.is_full()) {
    //   LOG_DEBUG("hot free");
    //   //hot_elems_list_.add2head(element, key);
    //   //key2element_entity[key] = element;
    //   return;
    // }
    // if (!new_elems_list_.is_full()) {
    //   LOG_DEBUG("new free");
    //   //new_elems_list_.add2head(element, key);
    //   return;
    // }

    // otherwise, we need to evict something
    // in order to free space for new element
    if (new_elems_list_.is_full()) {
      T evicted = new_elems_list_.get_tail_elem();
      new_elems_list_.make_space_for_new_elem();
      ghost_elems_list_.add2head(evicted, evicted.index);
    } else {
      // hot_elems_list_.add2head(element, key);
      hot_elems_list_.make_space_for_new_elem();
    }
  }

  // returns true on cache hit and false on cache miss
  template <typename F>
  bool lookup_update(T& element, KeyT key, F slow_get_page) {
    if (hot_elems_list_.is_element_present(key)) {
      hot_elems_list_.move_elem_to_the_head(key);
      element = hot_elems_list_.get_link2element(key);
      return true;
    }

    if (ghost_elems_list_.is_element_present(key)) {
      reclaim_for_x(element, key, slow_get_page);
      hot_elems_list_.add2head(element, key);
      new_elems_list_.erase_element(key);
      return false;
    } else if (new_elems_list_.is_element_present(key)) {
      element = new_elems_list_.get_link2element(key);
      return true;
    } else {
      reclaim_for_x(element, key, slow_get_page);
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
