# LRUcache
 
## 1. Overview

In this project, I've created my own implementations of different cache types: LRU cache, 2q cache (improvement of standard LRU cache) and "perfect" offline cache (Belady's algorithm), I've called it "prophecy" cache (because it's like it can see the future - what elements will come next).

## 2. Task description

### 2.1 General cache idea

First of all, let's discuss in simple words why do we need caches in the first place. Often, we need to load some pieces of data (for example on computer, load block of data from memory), but the problem is that it may take quite a while to load chunk of data and moreover, we can't even save all data, as it will require an enormous amount of space, we can allow only saving a small percent of it. So, here's an idea, if we request same data over and over again, i.e. use it frequently, than we can save it locally, otherwise if we use some chunk of data rarely than there's no need to save it and waste precious resources.

### 2.2

For this project, we were presented with simple task. Consider the following: you have a browser and a user that tries to load pages. But, once again, there are too many pages to save them all locally and it takes a while to load even one. So idea is too come up with some caching structure (or algorithm), that will maximize hit rate.

**Input**:
* cache size - maximal number of pages that we can store locally
* number of queries - number of pages that user will try to open (load) today (or during some fixed period of time).
* queries - natural indexes, that represent page identificators.

How penalty (or score) is counted: when need index from query is not presented in cache structure (this is called **cache miss**) that we need to load this pages (this takes time, so this we don't want this), otherwise we can very fast get loaded page as it's already've been saved previously (this event is called **cache hit**), this is good, so we add +1 to score (number of hits).

## 3. Cache types

Brief cache types description:
* [**LRU cache**](https://en.wikipedia.org/wiki/Cache_replacement_policies#Least_Recently_Used_(LRU)) - one of the most simple and straight forward strategies. If new page is present we simply return it's content. Otherwise, if there's still place in cache we can simply add new page. If that's not the case, then we evict (remove) page that has not been used for the longest time. I.e. we simply store linked list of pages, ones at the head (front) are most frequently used, if we use some page, than we move it to the front.
* [**Prophecy cache**](https://en.wikipedia.org/wiki/Cache_replacement_policies#B%C3%A9l%C3%A1dy's_algorithm) - as we can see all queries at once, we can come up with a very efficient (in terms of hit rate) algorithm. Each time we want to add new page and cache is full, we will simply remove page with the highest next usage index (i.e. the one, that will be used later than all the others). In order to maintain structure, I've used set of pairs, first element is index of next use for page, and second is actual page content.
* [**2q cache**](https://arpitbhayani.me/blogs/2q-cache/) - it's an improvement of standard LRU cache algorithm. It uses 3 queues, hot one (75% of cache capacity), new one (25% of capacity) and ghost one (it contains only indexes - meta information). Maybe you'll find interesting original [**paper**](https://www.vldb.org/conf/1994/P439.PDF).

## 4. Project structure

### 4.1 How to run?

Everything is very simple, you just need to run one script, that will install **myLogLib** submodule, create **bin** directory for *cmake* and show help message (with all targets available).

```bash
$ ./init_repo.sh
```

<details>
<summary>init_repo.sh script</summary>

```bash
#! /bin/bash

# ---------------------   PREPARING LOG LIB   -----------------------

set -e

echo "Repository initialization script"

# Проверяем что мы в git репозитории
if [ ! -d ".git" ]; then
  echo "Error: This is not a git repository"
  exit 1
fi

echo "Loading logger lib submodule..."

git submodule init
git submodule update

# Проверяем что нужный подмодуль загрузился
if [ -d "3dPartyModules/LoggerLib" ]; then
  echo "✓ LoggerLib submodule initialized successfully"
else
  echo "✗ LoggerLib submodule failed to initialize"
  exit 1
fi

echo "LoggerLib submodule ready!"

# ---------------------   PREPARING CMAKE   -----------------------

echo "Preparing cmake..."

mkdir -p bin # no error if it exists
cd bin
cmake ..
cd ..
cmake --build bin --target show_help_msg


```

</details>

Maybe you will need to make it executable:

```
$ chmod +x ./init_repo.sh
```

### 4.2 Dirs

```bash
tree -d -L 1
.
├── 3dPartyModules
├── bin
├── cacheRealizations
├── common
├── compare_hit_perf
├── tests
├── usecase
└── webPage
```

* **cacheRealizations** dir contains realizations of 3 cache types: LRU, 2q and "prophecy" (Belady algorithm) caches.
* **tests** - contains tests of different kinds. There are scripts which use *gtests*, they check that each cache type works correctly. In addition, there are scripts that check how fast each implementation works - loaded performance of some sort.
* **compare_hit_perf** - there's program that compares cache efficiencies of different caches. It requires prophecy cache to always have the biggest number of hits, as it's most optimal and can see all queries at once. In addition it shows table of hit performances of each cache type for every test case.
* **usecase/cacheDriver** - there's a usecase of cache, this is simply an implementation of described task, I've used it for debug purposes and quick tests. For an input it expects to get cache size and number of queries, then it reads indexes of pages in stated quantity.
* **usecase/usecaseForTests** - same as previous folder, but these scripts don't produce any additional input prompts.
* **3dpartyModules** - folder with my logger lib (I didn't use any other external modules).
* **webPage** - structure of web page, it also contains function *slow_get_page* that behaves like we are loading some heavy data, so there's a simple *sleep(1)* statement in it.

## Final thoughts

Caching is used in many fields of computer science to day, so it's important to know basic concepts like: how they work, what types there are, what are pros and cons of each one, how do they compare and etc. I think that LRU cache is good and simple algorithm, but often it's not optimal, for example if we have some state of hot elements in cache and the we read a long sequence of new elements (exceeding cache capacity), for example whole database read, we will remove all valuable element from cache and we will need to start over. 2qcache on the other hand, is more robust to one time gets and is more balanced on other types of input. "Prophecy" cache is some kind of ideal, unreachable caching algorithm, it's very good, yes, but unfortunately we can't use it most of the times, as it requires whole sequence of queries in advance.