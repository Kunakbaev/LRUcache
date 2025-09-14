// #include "../include/LRUcacheStruct.hpp"

// template <typename F>
// bool cache_t::lookup_update(KetT key, F slow_get_page) {
//   auto hit = hash._find(key);
//   if (hit == hash_.end()) { // not found
//     if (is_full()) {
//       hash_.erase(cache_.back().index);
//       cache_.pop_back();
//     }
//     // fallback to slow method
//     cache_.push_front(slow_get_page(key));
//     hash_[key] = cache_.begin();
//     return false;
//   }

//   auto elem_it = hit->second;
//   if (elem_it != cache_.begin()) {
//     cache_.splice(cache.begin(), cache_,
//       elem_it, std::next(elem_it));
//   }

//   return true;
// }
