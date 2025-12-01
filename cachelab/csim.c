#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cachelab.h"
typedef unsigned long long ull;

int s = 0, E = 0, b = 0;
int hits = 0, misses = 0, evicts = 0;
int verbose = 0;

struct Cache {
    int valid;
    ull time;
    ull mark;
} cache[32][8];

void prt() {
    struct Cache* p = NULL;
    for (int i = 1; i < 3; i++) {
        printf("set %d:\n", i);
        for (int j = 0; j < E; j++) {
            p = &cache[i][j];
            printf(" valid:%d, mark: %lld, time: %lld\n", p->valid, p->mark,
                   p->time);
        }
    }
}

void acc(ull addr, int size) {
    if (size <= 0)
        return;

    static ull timecnt = 0;

    prt();

    ull bias = addr % (1ull << b);
    ull set = (addr >> b) % (1ull << s);
    ull mark = addr >> (s + b);
    printf("(0x%llx + %d) bias: %lld, set: %lld, mark: %lld ", addr, size, bias,
           set, mark);
    printf("time: %lld ", timecnt);

    struct Cache* p = NULL;

    // check hit
    for (int i = 0; i < E; i++) {
        p = &cache[set][i];
        if (p->valid && p->mark == mark) {
            printf("[Hit]\n");
            hits++;
            p->time = timecnt++;
            acc(addr + (1ull << b), size - ((1ull << b) - bias));
            return;
        }
    }

    // miss
    printf("[Miss]\n");
    misses++;
    for (int i = 0; i < E; i++) {
        p = &cache[set][i];
        if (!p->valid) {
            p->valid = 1;
            p->time = timecnt++;
            p->mark = mark;
            acc(addr + (1ull << b), size - ((1ull << b) - bias));
            return;
        }
    }

    // need evict
    evicts++;
    struct Cache* evi = NULL;
    for (int i = 0; i < E; i++) {
        p = &cache[set][i];
        if (evi == NULL || p->time < evi->time) {
            evi = p;
        }
    }
    printf("[Evict: %lld -> %lld] \n", evi->mark, mark);
    evi->time = timecnt++;
    evi->mark = mark;
    acc(addr + (1ull << b), size - ((1ull << b) - bias));
}

void read_trace(char* trace_file) {
    FILE* fp = fopen(trace_file, "r");
    if (!fp) {
        printf("Error: cannot open trace file %s\n", trace_file);
        return;
    }

    char op;
    ull addr;
    int size;
    char buf[256];

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (buf[0] == 'I')
            continue;

        if (sscanf(buf, " %c %llx,%d", &op, &addr, &size) == 3) {
            if (op == 'M') {
                acc(addr, size);
                acc(addr, size);
            } else {
                acc(addr, size);
            }
        }
    }

    fclose(fp);
}

int main(int argc, char** argv) {
    int opt;
    char* trace_file = NULL;
    while ((opt = getopt(argc, argv, "s:E:b:t:v")) != -1) {
        switch (opt) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                printf(
                    "Usage: ./csim [-v] -s <num> -E <num> -b <num> -t "
                    "<file>\n");
                return 1;
        }
    }

    if (!s || !E || !b || trace_file == NULL) {
        printf("Usage: ./csim [-v] -s <num> -E <num> -b <num> -t <file>\n");
        return 1;
    }

    read_trace(trace_file);

    printSummary(hits, misses, evicts);
    return 0;
}
