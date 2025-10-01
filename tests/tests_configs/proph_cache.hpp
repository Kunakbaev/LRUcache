#pragma once

#include "../tests_structs.hpp"

namespace prophecy_cache_tests {
  static const CacheTestConf fromPresentation = {
    .test_name = "from_presentation",
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
      {true,  3},
      {true,  4}
    }
  };

  static const CacheTestConf cacheSize1 = {
    .test_name = "cache_size1",
    .cache_size = 1,
    .queries = {
      {false, 1},
      {false, 2},
      {false, 3},
      {true,  1},
      {false, 2},
      {false, 3},
      {true,  3},
      {true,  3},
      {false, 1}
    }
  };

  static const CacheTestConf period3_cap3 = {
    .test_name = "period3_cap3",
    .cache_size = 3,
    .queries = {
      {false, 1},
      {false, 2},
      {false, 3},
      {true,  1},
      {true,  2},
      {true,  3},
      {true,  1},
      {true,  2},
      {true,  3}
    }
  };

  static const CacheTestConf period3_cap2 = {
    .test_name = "period3_cap2",
    .cache_size = 2,
    .queries = {
      {false, 1},
      {false, 2},
      {false, 3},
      {true,  1},
      {true,  2},
      {false, 3},
      {true,  1},
      {true,  2},
      {false, 3}
    }
  };

  static const CacheTestConf reverse = {
    .test_name = "reverse",
    .cache_size = 3,
    .queries = {
      {false, 1}, // 1
      {false, 2}, // 1 2
      {false, 3}, // 1 2 3
      {false, 4}, // 2 3 4
      {true,  4}, //
      {true,  3},
      {true,  2},
      {false,  1}
    }
  };
}
