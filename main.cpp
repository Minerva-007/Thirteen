#define THIRTEEN_IMPLEMENTATION // This define must be set in one cpp file to create the implementation.
#include "thirteen.h"

int main()
{
    unsigned char* pixels = Thirteen::Init(500, 500);
    if (!pixels)
        return 1;

    unsigned int frameIndex = 0;
    Thirteen::SetVSync(false);
    // Go until window is closed or escape is pressed
    while (Thirteen::Render() && !Thirteen::GetKey(VK_ESCAPE))
    {
        // Write to pixels (RGBA format, 4 bytes per pixel)
        for (int i = 0; i < 500 * 500 * 4; i += 4)
        {
            pixels[i + 0] = 255; // Red
            pixels[i + 1] = (unsigned char)(frameIndex);   // Green
            pixels[i + 2] = (unsigned char)(frameIndex / 2);   // Blue
            pixels[i + 3] = 255; // Alpha
        }
        frameIndex++;
    }

    Thirteen::Shutdown();
    return 0;
}
