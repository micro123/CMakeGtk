#include "crypto_tool.hpp"
#include <openssl/aes.h>
#include <cassert>

static inline size_t aligned_to_n(size_t base, size_t n)
{
    return ((base - 1) / n + 1) * n;
}

#define CBC_IVEC "1234567890abcdef"

struct aes_buffer {
    const unsigned char magic[4] {'a','E','s','\x7F'};
    const unsigned int  encrypted_size;
    const unsigned int  origin_size;
    unsigned char       data[0];

    aes_buffer(size_t len, size_t aligned): encrypted_size(aligned), origin_size(len) {}

    static aes_buffer* create(size_t source_length) {
        size_t alloc_size = sizeof(aes_buffer) + aligned_to_n(source_length, 16);
        auto  *buffer = calloc(alloc_size, 1);
        return new (buffer)aes_buffer(source_length, alloc_size - sizeof(aes_buffer));
    }

    bool check() const {
        if (magic[0] == 'a' && magic[1] == 'E' && magic[2] == 's' && magic[3] == '\x7F')
        {
            return encrypted_size >= origin_size;
        }
        return false;
    }

    void destroy() {
        this->~aes_buffer();
        free(this);
    }

    operator std::string() const {
        auto charp = (const char*)this;
        return std::string{charp, charp + sizeof(*this) + encrypted_size};
    }
};

std::string EncryptDataAes256(const std::string& input, const std::string &password)
{
    assert(!password.empty() && password.length() <= 32);
    AES_KEY key;
    std::string aligned_key = password;
    while (aligned_key.length() < 32) {
        aligned_key.append(password.data(), std::min(32 - aligned_key.length(), password.length()));
    }
    AES_set_encrypt_key((const unsigned char*)aligned_key.data(), 256, &key);

    unsigned char ivec[] = CBC_IVEC;
    auto buffer = aes_buffer::create(input.length());
    AES_cbc_encrypt((const unsigned char*)input.data(), buffer->data, input.length(), &key, ivec, AES_ENCRYPT);
    std::string output = *buffer;
    buffer->destroy();
    return output;
}

bool        DecryptDataAes256(const std::string& input, const std::string &password, std::string &output)
{
    const aes_buffer *buffer = (const aes_buffer*)input.data();
    if (!buffer->check())
        return false;

    assert(!password.empty() && password.length() <= 32);
    AES_KEY key;
    std::string aligned_key = password;
    while (aligned_key.length() < 32) {
        aligned_key.append(password.data(), std::min(32 - aligned_key.length(), password.length()));
    }
    AES_set_decrypt_key((const unsigned char*)aligned_key.data(), 256, &key);

    unsigned char ivec[] = CBC_IVEC;
    output.resize(buffer->origin_size);
    AES_cbc_encrypt(buffer->data, (unsigned char*)&output[0], output.length(), &key, ivec, AES_DECRYPT);
    return true;
}
