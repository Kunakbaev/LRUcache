#pragma once

#include <map>
#include <unordered_set>
#include <stack>
#include <vector>
#include <list>
#include <unordered_map>

#include "../3dPartyModules/LoggerLib/include/logLib.hpp"
#include "../webPage/include/webPageStruct.hpp"

template <typename T, typename KeyT = int>
class lru2q_cache_t {
 private:
  // = new_pages_capacity / hot_pages_capacity
  static constexpr double NEW_ELEMS_CAPACITY_RATIOS = 0.25;
  const size_t max_cache_sz_ = 0;
  const size_t new_elems_cap_;
  const size_t hot_elems_cap_;

  using ListIt = typename std::list<T>::iterator;
  std::list<T> hot_elems_list_ = {};
  std::list<T> new_elems_list_ = {};
  std::unordered_map<KeyT, ListIt> hot_elems_pos_ = {};
  std::unordered_map<KeyT, ListIt> new_elems_pos_ = {};
  std::unordered_set<KeyT> ghost_keys_buff_ = {};

 public:
  lru2q_cache_t(size_t max_cache_sz)
      : max_cache_sz_(max_cache_sz),
      new_elems_cap_(static_cast<size_t>(NEW_ELEMS_CAPACITY_RATIOS * 
                                         static_cast<double>(max_cache_sz))),
      hot_elems_cap_(max_cache_sz - new_elems_cap_)
  {
    setLoggingLevel(DEBUG);
    LOG_DEBUG_VARS(new_elems_cap_, hot_elems_cap_);
  }

  bool is_new_elems_full() const {
    return new_elems_list_.size() >= new_elems_cap_;
  }

  bool is_hot_elems_full() const {
    return hot_elems_list_.size() >= hot_elems_cap_;
  }

  bool isCacheHit(T& element, KeyT key) {
    // std::cout << "key : " << key << " hot elems pos : ";
    // for (auto [ind, _] : hot_elems_pos_) {
    //   std::cout << ind << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "new elems pos : ";
    // for (auto [ind, _] : new_elems_pos_) {
    //   std::cout << ind << " ";
    // }
    // std::cout << std::endl;

    auto hit = hot_elems_pos_.find(key);
    if (hit != hot_elems_pos_.end()) {
      auto elem_it = hit->second;
      element = *elem_it;

      // transfer found element to the beginning
      // by shifting other elements to the right
      if (elem_it != hot_elems_list_.begin()) {
        hot_elems_list_.splice(hot_elems_list_.begin(), hot_elems_list_,
          elem_it, std::next(elem_it));
      }

      return true;
    }

    auto hit2 = new_elems_pos_.find(key);
    if (hit2 != new_elems_pos_.end()) {
      T promoted_elem = *hit2->second;
      element = promoted_elem;

      // Remove from new list
      new_elems_list_.erase(hit2->second);
      new_elems_pos_.erase(key);

      add_elem2_hot_elems(key, promoted_elem);

      return true;
    }

    return false;
  }

  void add_elem2_hot_elems(KeyT key, const T& element) {
    // make space in hot elems if needed
    if (is_hot_elems_full()) {
      KeyT evicted = hot_elems_list_.back().index;
      LOG_DEBUG_VARS(evicted);
      hot_elems_list_.pop_back();
      hot_elems_pos_.erase(evicted);
    }

    hot_elems_list_.push_front(element);
    hot_elems_pos_[key] = hot_elems_list_.begin();
  }

  // returns true on cache hit and false on cache miss
  template <typename F>
  bool lookup_update(T& element, KeyT key, F slow_get_page) {
    if (isCacheHit(element, key)) {
      // LOG_WARNING("HIT");
      return true;
    }

    // LOG_WARNING("MISS");

    assert(new_elems_list_.size() == new_elems_pos_.size());
    if (is_new_elems_full() && !new_elems_list_.empty()) {
      // TODO: is it okay that I rely on structure of 
      // elements that I store?
      // std::cout << "new elems list size : " << new_elems_list_.size() << std::endl;
      KeyT evicted_elem_key = new_elems_list_.front().index;
      // std::cout << "evicted : " << evicted_elem_key << std::endl;
      new_elems_list_.pop_front();
      ghost_keys_buff_.insert(evicted_elem_key);
      new_elems_pos_.erase(evicted_elem_key);
      // std::cout << "1after new elems pos : ";
      // for (auto [ind, _] : new_elems_pos_) {
      //   std::cout << ind << " ";
      // }
      // std::cout << std::endl;
    }

    slow_get_page(key, element);
    auto ghost = ghost_keys_buff_.find(key);
    if (ghost != ghost_keys_buff_.end()) {
      ghost_keys_buff_.erase(ghost);
      add_elem2_hot_elems(key, element);
      return false;
    } else {
      new_elems_list_.push_back(element);
      new_elems_pos_[key] = --new_elems_list_.end();
      // std::cout << "2after new elems pos : ";
      // for (auto [ind, _] : new_elems_pos_) {
      //   std::cout << ind << " ";
      // }
      // std::cout << std::endl;
      return false;
    }
  }

#ifdef _DEBUG
  void dump_cache() const {
    std::cout << "-----------------------\n2qcache: " << std::endl;
    std::cout << "hot elems: ";
    for (auto& it : hot_elems_list_) {
      std::cout << it.index << " ";
    }
    std::cout << std::endl;
    std::cout << "new elems: ";
    for (auto& it : new_elems_list_) {
      std::cout << it.index << " ";
    }
    std::cout << std::endl;
    std::cout << "ghost keys: ";
    for (auto& it : ghost_keys_buff_) {
      std::cout << it << " ";
    }
    std::cout << std::endl;
  }
#endif
};
