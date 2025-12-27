#include "resources.hpp"

using namespace notex;
using namespace std::string_literals;

ResHandle::ResHandle(const std::string& filename) :m_handle(filename, std::ios::in | std::ios::binary) {
    if (!m_handle.is_open()) {
        throw std::runtime_error("Can't open file "s + filename);
    }
    m_type_id = readU32();
    m_data_size = readU32();
}

int32_t ResHandle::readI32() {
    int32_t data;
    m_handle.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint32_t ResHandle::readU32() {
    uint32_t data;
    m_handle.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint16_t ResHandle::readU16() {
    uint16_t data;
    m_handle.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint8_t ResHandle::readU8() {
    uint8_t data;
    m_handle.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

void ResHandle::readBlock(void* target, size_t size) {
    m_handle.read(reinterpret_cast<char*>(target), size);
}
