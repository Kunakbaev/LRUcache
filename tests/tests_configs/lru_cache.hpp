#pragma once

#include "../tests_structs.hpp"

namespace lru_cache_tests {
  static const CacheTestConf fromPresentation = {
    .cache_size = 4,
    .queries = {
      {false, 1},
      {false, 2},
      {false, 3},
      {false, 4},
      {true,  1},
      {true,  2},
      {false, 5},
      {true,  1},
      {true,  2},
      {true,  4},
      {false, 3},
      {true,  4}
    }
  };

  static const CacheTestConf cacheSize1 = {
    .cache_size = 1,
    .queries = {
      {false, 1},
      {false, 2},
      {false, 3},
      {false, 1},
      {false, 2},
      {false, 3},
      {true,  3},
      {true,  3},
      {false, 1}
    }
  };

  static const CacheTestConf cap3_1 = {
    .cache_size = 3,
    .queries = {
      {false, 1},
      {false, 2},
      {false, 3},
      {true,  1},
      {true,  1},
      {true,  2}, // 2, 1, 3
      {false, 4}, // 4, 2, 1
      {false, 3}, // 3, 4, 2
      {false, 1}  // 1, 3, 4
    }
  };

  static const CacheTestConf cap3_2 = {
    .cache_size = 3,
    .queries = {
      {false, 1},
      {true,  1},
      {true,  1}, // 1
      {false, 2}, // 2, 1
      {true,  2},
      {true,  2},
      {true,  1}, // 1, 2
      {false, 3}, // 3, 1, 2
      {true,  3},
      {true,  3}, // 3, 1, 2
      {false, 4}, // 4, 3, 1
      {false, 2}, // 2, 4, 3
      {false, 1}
    }
  };

  static const CacheTestConf all_fit_big_cap = {
    .cache_size = 100,
    .queries = {
      {false, 1},
      {true,  1},
      {true,  1},
      {false, 2},
      {true,  2},
      {true,  2},
      {true,  1},
      {false, 3},
      {true,  3},
      {true,  3},
      {false, 4},
      {true,  2},
      {true,  1},
      {false, 10},
      {false, 92},
      {false, 42},
      {false, 56},
      {true,  10},
      {true,  1},
      {true,  3},
      {false, 32},
      {false, 7},
      {false, 21},
      {true, 92},
      {true, 21}
    }
  };
}
