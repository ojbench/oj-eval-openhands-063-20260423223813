
#include <iostream>
#include <cassert>
#include "MemoryRiver.hpp"

struct Node {
    int id;
    char data[64];
    bool operator==(const Node& other) const {
        if (id != other.id) return false;
        for (int i = 0; i < 64; ++i) if (data[i] != other.data[i]) return false;
        return true;
    }
};

int main() {
    MemoryRiver<Node, 2> mr("test_file.bin");
    mr.initialise();

    int val;
    mr.get_info(val, 1);
    assert(val == 0);
    mr.write_info(100, 1);
    mr.get_info(val, 1);
    assert(val == 100);

    Node n1 = {1, "node1"};
    Node n2 = {2, "node2"};
    Node n3 = {3, "node3"};

    int idx1 = mr.write(n1);
    int idx2 = mr.write(n2);
    int idx3 = mr.write(n3);

    Node r1, r2, r3;
    mr.read(r1, idx1);
    mr.read(r2, idx2);
    mr.read(r3, idx3);

    assert(r1 == n1);
    assert(r2 == n2);
    assert(r3 == n3);

    mr.Delete(idx2);
    Node n4 = {4, "node4"};
    int idx4 = mr.write(n4);
    assert(idx4 == idx2); // Should reuse idx2

    Node r4;
    mr.read(r4, idx4);
    assert(r4 == n4);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
