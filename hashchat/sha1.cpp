#include "sha1.h"
#include <string.h>

uint cycle_shift_left(uint val, int bit_count) {
    return (val << bit_count | val >> (32 - bit_count));
}

uint bring_to_human_view(uint val) {
    return  ((val & 0x000000FF) << 24) |
        ((val & 0x0000FF00) << 8) |
        ((val & 0x00FF0000) >> 8) |
        ((val & 0xFF000000) >> 24);
}

uint* sha1(char* message, uint msize_bytes) {
    //�������������
    uint A = H[0];
    uint B = H[1];
    uint C = H[2];
    uint D = H[3];
    uint E = H[4];

    // ������� ������ ����� ������
    uint totalBlockCount = msize_bytes / one_block_size_bytes;

    // �������, ������� ���� �����, ����� ��������� ��������� ����
    uint needAdditionalBytes =
        one_block_size_bytes - (msize_bytes - totalBlockCount * one_block_size_bytes);

    if (needAdditionalBytes < 8) {
        totalBlockCount += 2;
        needAdditionalBytes += one_block_size_bytes;
    }
    else {
        totalBlockCount += 1;
    }

    // ������ ������������ �� ���� �������� ���������
    uint extendedMessageSize = msize_bytes + needAdditionalBytes;

    // �������� ����� ����� � �������� � ���� ��������
    unsigned char* newMessage = new unsigned char[extendedMessageSize];
    memcpy(newMessage, message, msize_bytes);

    // ������ ��� ������ '1', ��������� ��������
    newMessage[msize_bytes] = 0x80;
    memset(newMessage + msize_bytes + 1, 0, needAdditionalBytes - 1);

    // ������ ����� ��������� ��������� � �����
    uint* ptr_to_size = (uint*)(newMessage + extendedMessageSize - 4);
    *ptr_to_size = bring_to_human_view(msize_bytes * 8);

    ExpendBlock exp_block;
    //������ �������
    for (int i = 0; i < totalBlockCount; i++) {

        // ����� ������� ���� � ��������� ���
        unsigned char* cur_p = newMessage + one_block_size_bytes * i;
        Block block = (Block)cur_p;

        // ������ 16 4�������� �����
        for (int j = 0; j < one_block_size_uints; j++) {
            exp_block[j] = bring_to_human_view(block[j]);
        }
        // ��������� 64...
        for (int j = one_block_size_uints; j < block_expend_size_uints; j++) {
            exp_block[j] =
                exp_block[j - 3] ^
                exp_block[j - 8] ^
                exp_block[j - 14] ^
                exp_block[j - 16];
            exp_block[j] = cycle_shift_left(exp_block[j], 1);
        }

        // ������������� 
        uint a = H[0];
        uint b = H[1];
        uint c = H[2];
        uint d = H[3];
        uint e = H[4];

        // �������������
        for (int j = 0; j < block_expend_size_uints; j++) {
            uint f;
            uint k;
            // � ����������� �� ������ ������� ��-�������
            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            // �������������
            uint temp = cycle_shift_left(a, 5) + f + e + k + exp_block[j];
            e = d;
            d = c;
            c = cycle_shift_left(b, 30);
            b = a;
            a = temp;
        }
        // �������������
        A = A + a;
        B = B + b;
        C = C + c;
        D = D + d;
        E = E + e;
    }

    // A,B,C,D,E �������� ��������� 32� ������������� ������������ ����
    uint* digest = new uint[5];
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = E;

    // ������ �� �����
    delete[] newMessage;
    return digest;
}