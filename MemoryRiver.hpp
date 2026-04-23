
#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <string>

using std::string;
using std::fstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;
    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        fstream file;
        file.open(file_name, std::ios::out | std::ios::binary);
        int tmp = 0;
        // We reserve (info_len + 1) ints. 
        // The first one is for the free list head.
        // The next info_len are for the user.
        for (int i = 0; i < info_len + 1; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        fstream file;
        file.open(file_name, std::ios::in | std::ios::binary);
        if (!file) return;
        file.seekg(n * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void write_info(int tmp, int n) {
        if (n > info_len) return;
        fstream file;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return;
        file.seekp(n * sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    int write(T &t) {
        fstream file;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return -1;
        
        int head;
        file.seekg(0);
        file.read(reinterpret_cast<char *>(&head), sizeof(int));

        int index;
        if (head == 0) {
            // No free blocks, append to end
            file.seekp(0, std::ios::end);
            index = file.tellp();
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        } else {
            // Use a free block
            index = head;
            int next_head;
            file.seekg(index);
            file.read(reinterpret_cast<char *>(&next_head), sizeof(int));
            
            // Update head
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&next_head), sizeof(int));
            
            // Write T at index
            file.seekp(index);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        }
        file.close();
        return index;
    }

    void update(T &t, const int index) {
        fstream file;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return;
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void read(T &t, const int index) {
        fstream file;
        file.open(file_name, std::ios::in | std::ios::binary);
        if (!file) return;
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void Delete(int index) {
        fstream file;
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return;
        
        int head;
        file.seekg(0);
        file.read(reinterpret_cast<char *>(&head), sizeof(int));
        
        // Current index becomes the new head
        // Store old head at index
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&head), sizeof(int));
        
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&index), sizeof(int));
        
        file.close();
    }
};

#endif
