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

void simulate (int ways, int cache_size, int block_size,
               int l2_cache_size, int l2_block_size) {
    unsigned int tag, index, x;
    unsigned int l2_tag, l2_index;

    int hitcnt = 0;
    int l2_hitcnt = 0;
    int offset_bit = log2(block_size);
    int index_bit  = log2(cache_size / ways / block_size);
    int line       = cache_size >> (offset_bit);

    // L2 using 4-way
    int l2_offset_bit = log2(l2_block_size);
    int l2_index_bit  = log2(l2_cache_size / 4 / l2_block_size);
    int l2_line       = l2_cache_size >> (l2_offset_bit);

    int no_of_access;
    int no_of_miss;

    cache_content *cache = new cache_content[line];
    cache_content *l2_cache = new cache_content[l2_line];
    std::cout << "ways:" << ways << "    ";
    std::cout << "cache_size:" << cache_size << "    ";
    std::cout << "block_size:" << block_size << std::endl;
    std::cout << "L2 ways:" << "4" << "    ";
    std::cout << "L2 cache_size:" << l2_cache_size << "    ";
    std::cout << "L2 block_size:"  << l2_block_size << std::endl;

    for (int j = 0; j < line; j++) {
        cache[j].tag = cache[j].age = 0;
    }

    for (int j = 0; j < l2_line; j++) {
        l2_cache[j].tag = l2_cache[j].age = 0;
    }


    //FILE *fp = std::fopen("LU.txt", "r");
    FILE *fp = std::fopen("RADIX.txt", "r");
    //FILE *fp = std::fopen("DCACHE.txt", "r");
    if (!fp) {
        std::cerr << "test file open error!" << std::endl;
        return;
    }


    no_of_access = no_of_miss = 0;

    // age for LRU algorithm
    int age_now = 0;
    while (std::fscanf(fp, "%x", &x) != EOF) {
        no_of_access++;
        age_now++;

        // consider cache[] as a 2-dimension array
        // where cache[index*ways + i] == cache[index][i]
        index = (x>>offset_bit) & ((line/ways)-1);
        tag   = x >> (index_bit+offset_bit);

        l2_index = (x>>l2_offset_bit) & ((l2_line/4)-1);
        l2_tag   = x >> (l2_index_bit+l2_offset_bit);

        int hit = false;
        int l2_hit = false;
        // go through all ways to find if hit
        for (int i = 0; i < ways; i++) {
            // update age when we have a non-clean box and match our tag
            if (cache[index*ways+i].age && cache[index*ways+i].tag == tag) {
                cache[index*ways+i].age = age_now;
                hitcnt++;
                hit = true;
                break;
            }
        }

        if (!hit) {

            for (int i = 0; i < 4; i++) {
                // update age when we have a non-clean box and match our tag
                if (l2_cache[l2_index*4+i].age && l2_cache[l2_index*4+i].tag == l2_tag) {
                    l2_cache[l2_index*4+i].age = age_now;
                    l2_hitcnt++;
                    l2_hit = true;

                    // missed in L1 but found in L2, update L1
                    cache[index*ways+i].age= age_now;
                    cache[index*ways+i].tag = tag;
                    break;
                }
            }

            if (!l2_hit) {

                // no hit means miss QQ
                no_of_miss++;
                int min = 55556666, idx = 0;
                for (int i = 0; i < ways; i++) {
                    // pick up a clean one
                    if (cache[index*ways+i].age == 0) {
                        idx = i;
                        break;
                    }
                    // or find the 'youngest' one
                    else if (cache[index*ways+i].age < min) {
                        min = cache[index*ways+i].age;
                        idx = i;
                    }
                }
                // replace this block
                cache[index*ways+idx].age= age_now;
                cache[index*ways+idx].tag = tag;
                l2_cache[l2_index*4+idx].age= age_now;
                l2_cache[l2_index*4+idx].tag = l2_tag;
            }
        }
    }

    if (fp) {
        std::fclose(fp);
    }

    delete [] cache;

    double l1missrate = ((double)(l2_hitcnt+no_of_miss))/((double)(hitcnt+l2_hitcnt+no_of_miss));
    double l2missrate = ((double)no_of_miss)/((double)(l2_hitcnt+no_of_miss));
    std::cout << "miss rate:" << (no_of_miss/(double)no_of_access) << std::endl;
    std::cout << "AMAT:" << (1 + l1missrate*(10 + l2missrate*100)) << std::endl;
    std::cout << "===========" << std::endl;

}

int main (void) {
    simulate(4,  1*K, 4, 4*K, 16);
    simulate(4,  1*K, 4, 16*K, 16);
    simulate(4,  1*K, 4, 64*K, 16);
    simulate(4,  1*K, 4, 256*K, 16);

    simulate(4,  1*K, 4, 4*K, 32);
    simulate(4,  1*K, 4, 16*K, 32);
    simulate(4,  1*K, 4, 64*K, 32);
    simulate(4,  1*K, 4, 256*K, 32);

    simulate(4,  1*K, 4, 4*K, 64);
    simulate(4,  1*K, 4, 16*K, 64);
    simulate(4,  1*K, 4, 64*K, 64);
    simulate(4,  1*K, 4, 256*K, 64);

    simulate(4,  1*K, 4, 4*K, 128);
    simulate(4,  1*K, 4, 16*K, 128);
    simulate(4,  1*K, 4, 64*K, 128);
    simulate(4,  1*K, 4, 256*K, 128);

    simulate(4,  1*K, 4, 4*K, 256);
    simulate(4,  1*K, 4, 16*K, 256);
    simulate(4,  1*K, 4, 64*K, 256);
    simulate(4,  1*K, 4, 256*K, 256);

    return 0;
}
