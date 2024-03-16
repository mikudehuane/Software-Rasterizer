#include "Buffer.h"

#include <array>
#include <fstream>
#include <iostream>

namespace
{

// -1~1 -> 0~255
uint8_t DepthToColor(const float value)
{
    return static_cast<uint8_t>(
        std::max(0, std::min(255, static_cast<int>((value + 1.0f) * 128.0f)))
    );
}

// 0~1 -> 0~255
uint8_t ToColor(const float value)
{
	return static_cast<uint8_t>(
        std::max(0, std::min(255, static_cast<int>(value * 256.0f)))
	);
}

#pragma pack(push, 1)
// ReSharper disable CppDeclaratorNeverUsed
struct BmpHeader {
    uint16_t signature; // BM (0x42, 0x4D)
    uint32_t fileSize; // File size in bytes
    uint16_t reserved1; // Reserved (0)
    uint16_t reserved2; // Reserved (0)
    uint32_t dataOffset; // Offset to image data in bytes
    uint32_t headerSize; // Header size in bytes (40)
    uint32_t width; // Image width in pixels
    uint32_t height; // Image height in pixels
    uint16_t planes; // Number of color planes (1)
    uint16_t bitDepth; // Bits per pixel (24)
    uint32_t compression; // Compression type (0 for uncompressed)
    uint32_t imageSize; // Image size in bytes (can be 0 for uncompressed)
    uint32_t xPixelsPerMeter; // Horizontal resolution in pixels/meter (can be 0)
    uint32_t yPixelsPerMeter; // Vertical resolution in pixels/meter (can be 0)
    uint32_t colorsUsed; // Number of colors in the palette (can be 0 for full color images)
    uint32_t colorsImportant; // Number of important colors (can be 0)
};
// ReSharper restore CppDeclaratorNeverUsed
#pragma pack(pop)
	
}

namespace Islander
{

void DepthBuffer::DumpBmp(const std::string& path) const
{
    constexpr int nChannel = 3;
    const int rowPadding = (4 - (m_Width * 3) % 4) % 4; // BMP requires rows to be padded to a multiple of 4 bytes

    std::ofstream bmpFile(path, std::ios::binary);
    if (!bmpFile) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return;
    }

    const uint32_t fileSize = static_cast<uint32_t>(
        sizeof(BmpHeader) + static_cast<size_t>(m_Width * nChannel + rowPadding) * m_Height
	);
    BmpHeader header = {
        .signature = 0x4D42, // BM
        .fileSize = fileSize,
        .reserved1 = 0,
        .reserved2 = 0,
        .dataOffset = sizeof(BmpHeader),
        .headerSize = 40,
        .width = static_cast<uint32_t>(m_Width),
        .height = static_cast<uint32_t>(m_Height),
        .planes = 1,
        .bitDepth = 24,
        .compression = 0,
        .imageSize = 0,
        .xPixelsPerMeter = 0,
        .yPixelsPerMeter = 0,
        .colorsUsed = 0,
        .colorsImportant = 0
    };

    bmpFile.write(reinterpret_cast<char*>(&header), sizeof(header));

	constexpr std::array<char,3> padding = { 0, 0, 0 };
    // i: high -> top; j: high -> right
    const float* data = m_Data.data();
    for (int i = 0; i < m_Height; ++i)
    {
        for (int j = 0; j < m_Width; ++j)
        {
            const char color = static_cast<char>(DepthToColor(*data));
            std::array pixel = { color, color, color };
            bmpFile.write(pixel.data(), nChannel);
            ++data;
        }
        bmpFile.write(padding.data(), rowPadding); // Padding
    }
}

Color::Color(const Vec3& color)
	: r(ToColor(color.x))
    , g(ToColor(color.y))
    , b(ToColor(color.z))
{
}
}

