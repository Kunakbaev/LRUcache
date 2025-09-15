#pragma once

#include <iostream>

template<std::size_t CacheSize>
class CacheTest : public testing::TestWithParam<std::pair<bool, std::size_t>> {};
