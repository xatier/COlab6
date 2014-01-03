#include <iostream>
#include <cstdio>


struct cache_content {
    unsigned int tag;
    unsigned int age;
    // unsigned int data[16];
};

// my own log2
int log2 (int n) {
    int m = 0;
    while (n >>= 1) ++m;
    return m;
}

const int K = 1024;

void simulate (int ways, int cache_size, int block_size) {
    unsigned int tag, index, x;

    int offset_bit = log2(block_size);
    int index_bit  = log2(cache_size / ways / block_size);
    int line       = cache_size >> (offset_bit);

    int no_of_access;
    int no_of_miss;

    cache_content *cache = new cache_content[line];
    std::cout << "ways:" << ways << "    ";
    std::cout << "cache_size:" << cache_size << "    ";
    //std::cout << "block_size:" << block_size << std::endl;
    //std::cout << "cache line:" << line << "    ";
    //std::cout << "offset_bit:" << offset_bit << "    ";
    //std::cout << "index_bit:"  << index_bit << std::endl;

    for (int j = 0; j < line; j++) {
        cache[j].tag = cache[j].age = 0;
    }


    FILE *fp = std::fopen("LU.txt", "r");
    //FILE *fp = std::fopen("RADIX.txt", "r");
    if (!fp) {
        std::cerr << "test file open error!" << std::endl;
        return;
    }


    no_of_access = no_of_miss = 0;
    int age_now = 0;
    while (std::fscanf(fp, "%x", &x) != EOF) {
        no_of_access++;
        age_now++;

        index = (x>>offset_bit) & ((line/ways)-1);
        tag   = x >> (index_bit+offset_bit);

        int hit = false;
        // go through all ways to find if hit
        for (int i = 0; i < ways; i++) {
            if (cache[index*ways+i].age && cache[index*ways+i].tag == tag) {
                cache[index*ways+i].age = age_now;
                hit = true;
                break;
            }
        }

        if (!hit) {
            // no hit means miss QQ
            no_of_miss++;
            int min = 55556666, idx = 0;
            for (int i = 0; i < ways; i++) {
                if (cache[index*ways+i].age == 0) {            // clean
                    idx = i;
                    break;
                }
                else if (cache[index*ways+i].age < min) {
                    min = cache[index*ways+i].age;
                    idx = i;
                }
            }
            cache[index*ways+idx].age= age_now;
            cache[index*ways+idx].tag = tag;
        }
    }

    if (fp) {
        std::fclose(fp);
    }

    delete [] cache;

    std::cout << "miss rate:" << (no_of_miss/(double)no_of_access) << std::endl;
    std::cout << "===========" << std::endl;

}

int main (void) {

    simulate(1,  1*K, 32);
    simulate(1,  2*K, 32);
    simulate(1,  4*K, 32);
    simulate(1,  8*K, 32);
    simulate(1, 16*K, 32);
    simulate(1, 32*K, 32);

    simulate(2,  1*K, 32);
    simulate(2,  2*K, 32);
    simulate(2,  4*K, 32);
    simulate(2,  8*K, 32);
    simulate(2, 16*K, 32);
    simulate(2, 32*K, 32);

    simulate(4,  1*K, 32);
    simulate(4,  2*K, 32);
    simulate(4,  4*K, 32);
    simulate(4,  8*K, 32);
    simulate(4, 16*K, 32);
    simulate(4, 32*K, 32);

    simulate(8,  1*K, 32);
    simulate(8,  2*K, 32);
    simulate(8,  4*K, 32);
    simulate(8,  8*K, 32);
    simulate(8, 16*K, 32);
    simulate(8, 32*K, 32);

    return 0;
}
