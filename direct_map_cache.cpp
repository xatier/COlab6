#include <iostream>
#include <cstdio>


struct cache_content {
    bool v;
    unsigned int tag;
    // unsigned int data[16];
};

// my own log2
int log2 (int n) {
    int m = 0;
    while (n >>= 1) ++m;
    return m;
}

const int K = 1024;

void simulate (int cache_size, int block_size) {
    unsigned int tag, index, x;

    int offset_bit = log2(block_size);
    int index_bit  = log2(cache_size / block_size);
    int line       = cache_size >> (offset_bit);

    cache_content *cache = new cache_content[line];
    std::cout << "cache line:" << line << std::endl;

    for (int j = 0;j < line; j++)
        cache[j].v = false;


    FILE *fp = std::fopen("test.txt", "r");
    if (!fp) {
        std::cerr << "test file open error!" << std::endl;
    }

    while (std::fscanf(fp, "%x", &x) != EOF) {
        std::cout << std::hex << x << " ";

        index = (x>>offset_bit) & (line-1);
        tag   = x >> (index_bit+offset_bit);

        if (cache[index].v && cache[index].tag == tag) {
            cache[index].v   = true;            // hit
        }
        else {
            cache[index].v   = true;            // miss
            cache[index].tag = tag;
        }
    }

    std::fclose(fp);

    delete [] cache;
}

int main (void) {
    // Let us simulate 4KB cache with 16B blocks
    simulate(4*K, 16);
    return 0;
}