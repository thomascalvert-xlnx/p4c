#include <core.p4>
#include <ebpf_model.p4>

#define FORTY 40

extern IntComparator {
    IntComparator(int reference, bool invert);

    // Returns val minus reference, clamped to [-128, 127].
    // result is set to: (val==reference) XOR invert
    int<8> doCompare(in bit<16> val, out bool result);
}

struct Headers_t {}

parser prs(packet_in p, out Headers_t headers) {
    IntComparator(FORTY + 4, false) myExt;

    state start {
        myExt.doCompare(123, _);
        transition accept;
    }
}

control ctl(inout Headers_t headers, out bool pass) {
    IntComparator(FORTY + 2, true) myExt;

    apply {
        bit<16> val = 0xffff;
        int<8> ret = myExt.doCompare(val, pass);
        if (ret == 0) {
            pass = true;
        }
    }
}

ebpfFilter(prs(), ctl()) main;
