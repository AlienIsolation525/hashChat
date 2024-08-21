#include "chat.h"
#include "iostream"

Chat::Chat() {

    data_count = 0;
    data = nullptr;

    allocNewMem(8);
}
void Chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    uint* digest = sha1(_pass, pass_length);
    addinner(_login, digest);
}
void Chat::unreg(char _login[LOGINLENGTH]) {
    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(_login, i * i);
        if (data[index].status == CellStatus::free)
            return;
        else if (data[index].status == CellStatus::engaged
            && !memcmp(_login, data[index].login, LOGINLENGTH))
            break;
    }
    if (i >= mem_size) return;

    data[index].status = CellStatus::deleted;
}
bool Chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {

    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(_login, i * i);
        if (data[index].status == CellStatus::free)
            return false;
        else if (data[index].status == CellStatus::engaged
            && !memcmp(_login, data[index].login, LOGINLENGTH))
            break;
    }
    if (i >= mem_size) return false;
    std::cout << "Propbs count: " << i + 1 << std::endl;

    uint* digest = sha1(_pass, pass_length);

    bool cmpHashes = !memcmp(
        data[index].pass_sha1_hash,
        digest,
        SHA1HASHLENGTHBYTES);
    delete[] digest;

    return cmpHashes;
}

void Chat::addinner(char login[LOGINLENGTH], uint* digest) {
    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(login, i * i);
        if (data[index].status == CellStatus::free)
            break;
    }
    if (i >= mem_size)
    {
        resize();
        addinner(login, digest);
    }
    else {
        data[index] = AuthData(login, digest);
        data_count++;
    }
}

void Chat::allocNewMem(int newMemSize) {
    mem_size = newMemSize;
    data = new AuthData[mem_size];
}

int Chat::hash_func(char login[LOGINLENGTH], int step) {
    long sum = 0;
    for (int i = 0; i < LOGINLENGTH; i++) {
        sum += login[i];
    }
    return (hf_multiply(sum) + step) % mem_size;
}

int Chat::hf_multiply(int val) {
    const double A = 0.7;
    return int(mem_size * (A * val - int(A * val)));
}

void Chat::resize() {
    AuthData* save = data;
    int save_ms = mem_size;

    allocNewMem(mem_size * 2);
    data_count = 0;

    for (int i = 0; i < save_ms; i++) {
        AuthData& old_data = save[i];
        if (old_data.status == CellStatus::engaged) {

            uint* sha_hash_copy = new uint[SHA1HASHLENGTHUINTS];
            memcpy(sha_hash_copy, old_data.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            addinner(old_data.login, sha_hash_copy);
        }
    }

    delete[] save;
}