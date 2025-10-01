#pragma once

#include <list>
#include <unordered_map>

#include "logLib.hpp"

template <typename T, typename KeyT = int>
class list_with_track_t {
 private:
  const std::size_t capacity_ = 0;
  std::list<T> list_ = {};

  using ListIt = typename std::list<T>::iterator;
  std::unordered_map<KeyT, ListIt> hash_ = {};

 public:
  list_with_track_t(std::size_t capacity)
    : capacity_(std::max(capacity, static_cast<std::size_t>(1))) {
    // LOG_DEBUG_VARS(capacity_);
  }

  [[nodiscard]] std::size_t get_cap() const { return capacity_; }

  [[nodiscard]] bool is_full() const {
    // LOG_DEBUG_VARS(list_.size(), capacity_);
    return list_.size() == capacity_;
  }

  // if there's no space, tail element will be evicted
  void remove_tail() {
    if (!is_full()) return;

    // delete from tail
    // LOG_DEBUG("del");
    hash_.erase(list_.back().index);
    list_.pop_back();
  }

  // adds new element, if no space is left, it will evict tail element
  void add2head(T& element, KeyT key) {
    remove_tail();

    list_.push_front(element);
    hash_[key] = list_.begin();
  }

  void erase_element(KeyT key) {
    auto it = hash_.find(key);
    //assert(it != hash_.end());
    if (it == hash_.end()) { // TODO:
      return;
    }

    ListIt elem_it = it->second;
    list_.erase(elem_it);
    hash_.erase(key);
  }

  void move_elem_to_the_head(KeyT key) {
    auto hit = hash_.find(key);
    assert(hit != hash_.end());

    ListIt elem_it = hit->second;
    if (elem_it != list_.begin()) {
      list_.splice(list_.begin(), list_,
        elem_it, std::next(elem_it));
    }
  }

  [[nodiscard]] bool is_element_present(KeyT key) const {
    return hash_.find(key) != hash_.end();
  }

  [[nodiscard]] T& get_link2element(KeyT key) const {
    auto it = hash_.find(key);
    assert(it != hash_.end());
    return *it->second;
  }

  [[nodiscard]] T get_tail_elem() const {
    return list_.back();
  }

#ifdef _DEBUG
  void print_inner_list_contents() const {
    for (auto& it : list_) {
      std::cout << it.index << " ";
    }
  }
#endif
};
