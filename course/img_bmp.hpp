#include <string>
#include <cstdio>
#include <cstring>
#include <istream>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace BMP
{
bool Debug;
typedef char byte;
typedef unsigned int _size_t;
const byte Small = 0;
const byte Big = 1;

class iBufferedBytes;
class FileHeader;
class BMPHeader;
class Image;
class Matrix;

class iBufferedBytes
{
    static const int MAX_LENGTH = 16384;
    std::istream *data;
    byte tmp[MAX_LENGTH + 100];
    int length, now;

  public:
    iBufferedBytes(std::string filename) : length(0), now(0)
    {
        data = new std::ifstream(filename, std::ifstream::binary);
    }
    void reload()
    {
        data->read(tmp, MAX_LENGTH);
        now = 0;
        length = data->gcount();
    }
    void copy(byte *st, _size_t l)
    {
        while (l > length - now)
        {
            memcpy(st, tmp + now, length - now);
            l -= length - now;
            st += length - now;
            reload();
        }
        memcpy(st, tmp + now, l);
        now += l;
        return;
    }
    ~iBufferedBytes()
    {
        delete data;
    }
};

void ByteRead(const iBufferedBytes &data) { return; }
template <typename T, typename... Args>
void ByteRead(iBufferedBytes &data, T now, Args... rest)
{
    data.copy((byte *)now, sizeof(*now));
    ByteRead(data, rest...);
    return;
}

class oBufferedBytes
{
    static const int MAX_LENGTH = 16384;
    std::ofstream *data;
    byte tmp[MAX_LENGTH + 100];
    int now;

  public:
    oBufferedBytes(std::string filename) : now(0)
    {
        data = new std::ofstream(filename, std::ofstream::binary);
    }
    void flush()
    {
        data->write(tmp, now);
        now = 0;
    }
    void write(byte *st, _size_t l)
    {
        while (now + l >= MAX_LENGTH)
        {
            memcpy(tmp + now, st, MAX_LENGTH - now);
            l -= MAX_LENGTH - now;
            st += MAX_LENGTH - now;
            now = MAX_LENGTH;
            flush();
        }
        memcpy(tmp + now, st, l);
        now += l;
        return;
    }
    ~oBufferedBytes()
    {
        flush();
        data->flush();
        data->close();
        delete data;
    }
};

void ByteWrite(const oBufferedBytes &data) { return; }
template <typename T, typename... Args>
void ByteWrite(oBufferedBytes &data, T now, Args... rest)
{
    data.write((byte *)now, sizeof(*now));
    ByteWrite(data, rest...);
    return;
}

class FileHeader
{
    unsigned short FileType;
    unsigned int bfSize;
    unsigned short bfReserved[2];
    unsigned int Bits2Data;

  public:
    FileHeader() = default;
    void load(iBufferedBytes &data)
    {
        ByteRead(data, &FileType, &bfSize, &bfReserved[0], &bfReserved[1], &Bits2Data);
    }
    void write(oBufferedBytes &data)
    {
        ByteWrite(data, &FileType, &bfSize, &bfReserved[0], &bfReserved[1], &Bits2Data);
    }
    void changeSize(_size_t a)
    {
        bfSize = a + 14;
    }
    _size_t size() { return 14u; }
    _size_t bits2data() { return Bits2Data; }
};

class BMPHeader
{
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsMeter;
    int biYPelsMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
    byte *unknown;

  public:
    friend class Matrix;
    BMPHeader() = default;
    void load(iBufferedBytes &data)
    {
        ByteRead(data, &biSize, &biWidth, &biHeight, &biPlanes,
                 &biBitCount, &biCompression, &biSizeImage, &biXPelsMeter,
                 &biYPelsMeter, &biClrUsed, &biClrImportant);
        if (biSize > 40u)
        {
            unknown = new byte[biSize - 40u];
            data.copy(unknown, biSize - 40u);
        }
        return;
    }
    void write(oBufferedBytes &data)
    {
        ByteWrite(data, &biSize, &biWidth, &biHeight, &biPlanes,
                  &biBitCount, &biCompression, &biSizeImage, &biXPelsMeter,
                  &biYPelsMeter, &biClrUsed, &biClrImportant);
        data.write(unknown, biSize - 40u);
        return;
    }
    _size_t get_pH() { return (biHeight < 0) ? -biHeight : biHeight; }
    _size_t get_pW() { return (biWidth < 0) ? -biWidth : biWidth; }
    _size_t get_pB() { return biBitCount; }
    void rotate()
    {
        std::swap(biHeight, biWidth);
        std::swap(biXPelsMeter, biYPelsMeter);
    }
    _size_t size()
    {
        return biSize;
    }
};

class Matrix
{
    byte **array;
    _size_t MaxX, MaxY, width;
    _size_t realY;

  public:
    Matrix() = default;
    Matrix(int x, int y, int w) : MaxX(x), MaxY(y), width(w), realY(((MaxY * width + 7) / 8 + 3) / 4 * 4)
    {
        // std::cout << x << " * " << y << " real: " << realY << std::endl;
        array = new byte *[MaxX];
        for (int i = 0; i < x; ++i)
        {
            array[i] = new byte[realY];
        }
    };
    void load(iBufferedBytes &data)
    {
        for (int i = 0; i < MaxX; ++i)
        {
            // printf("i=%d\n",i);
            // if (i==483) Debug=true;
            data.copy(array[i], realY);
        }
        // puts("xxxxxxxx");
    }
    void write(oBufferedBytes &data)
    {
        // printf("x=%u,y=%u\n", MaxX, realY);
        for (int i = 0; i < MaxX; ++i)
        {
            // printf("i=%d\n",i);
            data.write(array[i], realY);
        }
    }
    void rotate(const BMPHeader *bmpH)
    {
        byte **tmp = new byte *[MaxY];
        int tp = ((MaxX * width + 7) / 8 + 3) / 4 * 4;
        int X = (bmpH->biHeight < 0) ? bmpH->biHeight - 1 : 0;
        int aX = (X == 0) ? 1 : -1;
        for (int i = 0; i < MaxY; ++i)
        {
            tmp[i] = new byte[tp];
            if (bmpH->biHeight < 0)
                for (int j = 0; j < MaxX; ++j)
                {
                    memcpy(tmp[i] + j * width / 8, array[MaxX - j - 1] + i * width / 8, width / 8);
                }
            else
                for (int j = 0; j < MaxX; ++j)
                {
                    memcpy(tmp[i] + j * width / 8, array[j] + (MaxY - i - 1) * width / 8, width / 8);
                }
            for (int j = MaxX * width; j < tp; ++j)
                tmp[i][j] = 0;
        }
        for (int i = 0; i < MaxX; ++i)
            delete array[i];
        delete array;
        array = tmp;
        std::swap(MaxX, MaxY);
        realY = tp;
        return;
    }
    _size_t size() { return MaxX * realY; }
    ~Matrix()
    {
        for (int i = 0; i < MaxX; ++i)
            delete array[i];
        delete array;
    }
};

class Image
{
    FileHeader fileH;
    BMPHeader bmpH;
    Matrix *colM;

  public:
    Image() = default;
    void load(std::string filename)
    {
        iBufferedBytes *tmp = new iBufferedBytes(filename);
        fileH.load(*tmp);
        bmpH.load(*tmp);
        colM = new Matrix(bmpH.get_pH(), bmpH.get_pW(), bmpH.get_pB());
        colM->load(*tmp);
    }
    ~Image()
    {
        delete colM;
    }
    void rotate()
    {
        colM->rotate(&bmpH);
        bmpH.rotate();
        fileH.changeSize(bmpH.size() + colM->size());
    }
    void write(std::string name)
    {
        oBufferedBytes data(name);
        fileH.write(data);
        bmpH.write(data);
        colM->write(data);
        return;
    }
};
}