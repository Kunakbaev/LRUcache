#pragma once

#include "../tests_structs.hpp"

namespace check_correctness::lru2q_cache_tests {
  static const CacheTestConf fromPresentation = {
    .test_name = "from_presentation",
    .cache_size = 4,
    .queries = {
      {false, 1},
      {false, 2},
      {false, 3},
      {false, 4},
      {false, 1},
      {false, 2},
      {false, 5},
      {true,  1},
      {true,  2},
      {false, 4},
      {false, 3},
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
      {false, 1},
      {false, 2},
      {false, 3},
      {true,  3},
      {true,  3},
      {false, 1}
    }
  };

  static const CacheTestConf cap3_1 = {
    .test_name = "cap3_1",
    .cache_size = 3,
    .queries = {
      {false, 1},  // miss, add to Ain (ghost queue)
      {false, 2},  // miss, add to Ain (ghost queue)
      {false, 3},  // miss, add to Ain (ghost queue)
      {false, 1},  // miss, found in ghost, promote to Amain
      {false, 4},  // miss, add to Ain (evicts from ghost)
      {true,  1},  // hit, found in Amain
      {false, 2},  // miss, found in ghost, promote to Amain
      {false, 5}   // miss, add to Ain (evicts from ghost)
    }
  };

  static const CacheTestConf cap4_1 = {
    .test_name = "cap4_1",
    .cache_size = 4,
    .queries = {
      {false, 1},  // miss, add to Ain
      {false, 2},  // miss, add to Ain
      {false, 3},  // miss, add to Ain
      {false, 4},  // miss, add to Ain
      {false, 1},  // miss, found in ghost, promote to Amain
      {false, 5},  // miss, add to Ain (evicts from ghost)
      {true,  1},  // hit, found in Amain
      {false, 2},  // miss, found in ghost, promote to Amain
      {false, 6},  // miss, add to Ain (evicts from ghost)
      {true,  2}   // hit, found in Amain
    }
  };

  static const CacheTestConf cap5_1 = {
    .test_name = "cap5_1",
    .cache_size = 3,
    .queries = {
      {false, 1},  // miss, add to Ain
      {false, 2},  // miss, add to Ain
      {false, 3},  // miss, add to Ain
      {false, 4},  // miss, add to Ain
      {false, 5},  // miss, add to Ain
      {false, 1},  // miss, found in ghost, promote to Amain
      {false, 6},  // miss, add to Ain (evicts from ghost)
      {true,  1},  // hit, found in Amain
      {false, 2},  // miss, found in ghost, promote to Amain
      {false, 7},  // miss, add to Ain (evicts from ghost)
      {true,  2},  // hit, found in Amain
      {false, 3},  // miss, found in ghost, promote to Amain
      {true,  3}   // hit, found in Amain
    }
  };

  static const CacheTestConf cap6_1 = {
    .test_name = "cap6_1",
    .cache_size = 6,
    .queries = {
      {false, 1},  // miss, add to Ain
      {false, 2},  // miss, add to Ain
      {false, 3},  // miss, add to Ain
      {false, 4},  // miss, add to Ain
      {false, 5},  // miss, add to Ain
      {false, 6},  // miss, add to Ain
      {false, 1},  // miss, found in ghost, promote to Amain
      {false, 7},  // miss, add to Ain (evicts from ghost)
      {true,  1},  // hit, found in Amain
      {false, 2},  // miss, found in ghost, promote to Amain
      {false, 8},  // miss, add to Ain (evicts from ghost)
      {true,  2},  // hit, found in Amain
      {false, 3},  // miss, found in ghost, promote to Amain
      {false, 9},  // miss, add to Ain (evicts from ghost)
      {true,  3},  // hit, found in Amain
      {false, 1}   // miss
    }
  };

  static const CacheTestConf all_fit_big_cap = {
    .test_name = "all_fit_big_cap",
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
